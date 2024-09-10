#include "ArrayBoundChecker.h"

using namespace clang;
using namespace ento;

void ArrayBoundChecker::checkLocation(SVal left, bool is_load, const Stmt *statement, CheckerContext &context) const
{
    // Check for out of bound array element access.
    const MemRegion *region = left.getAsRegion();

    if (!region)
    {
        return;
    }

    std::string region_string = region->getString(); // debug-test

    const ElementRegion *element_region = dyn_cast<ElementRegion>(region);
    const FieldRegion *field_region = dyn_cast<FieldRegion>(region);

    if (!element_region && !field_region)
    {
        return;
    }

    if (element_region)
    {
        // Get the index of the accessed element
        DefinedOrUnknownSVal index = element_region->getIndex().castAs<DefinedOrUnknownSVal>();

        // Zero index is always in bound, this also passes ElementRegions created for
        // pointer casts.
        if (index.isZeroConstant())
        {
            return;
        }

        ProgramStateRef state = context.getState();

        std::string element_region_super_region_string = element_region->getSuperRegion()->getString();
        // Get the size of the array.
        DefinedOrUnknownSVal element_count = getDynamicElementCount(
            state, element_region->getSuperRegion(), context.getSValBuilder(), element_region->getValueType());

        ProgramStateRef statement_in_bound = state->assumeInBound(index, element_count, true);

        ProgramStateRef statement_out_bound = state->assumeInBound(index, element_count, false);

        if (statement_out_bound && !statement_in_bound)
        {
            ExplodedNode *node = context.generateErrorNode(statement_out_bound);

            if (!node)
            {
                return;
            }

            if (!_builtin_ptr)
            {
                _builtin_ptr.reset(new BuiltinBug(this, "out-of-bound array access",
                                                  "Access out-of-bound array element (buffer overflow)"));
            }

            auto report = std::make_unique<PathSensitiveBugReport>(*_builtin_ptr, _builtin_ptr->getDescription(), node);

            report->addRange(statement->getSourceRange());

            context.emitReport(std::move(report));

            return;
        }

        // Array bound check succeeded.  From this point forward the array bound
        // should always succeed.

        context.addTransition(statement_in_bound);
    }
    else if (field_region)
    {
        const MemRegion *r = field_region->getSuperRegion();
        if (!r)
        {
            return;
        }

        std::string r_string = r->getString();
        const ElementRegion *er = dyn_cast<ElementRegion>(r);

        if (!er)
        {
            return;
        }

        er->getIndex();
        DefinedOrUnknownSVal index = er->getIndex().castAs<DefinedOrUnknownSVal>();

        if (index.isZeroConstant())
        {
            return;
        }

        ProgramStateRef state = context.getState();

        DefinedOrUnknownSVal e_count =
            getDynamicElementCount(state, er->getSuperRegion(), context.getSValBuilder(), er->getValueType());

        ProgramStateRef st_in_bound = state->assumeInBound(index, e_count, true);
        ProgramStateRef st_out_bound = state->assumeInBound(index, e_count, false);

        if (st_out_bound && !st_in_bound)
        {
            ExplodedNode *node = context.generateErrorNode(st_out_bound);

            if (!node)
            {
                return;
            }

            if (!_builtin_ptr)
            {
                _builtin_ptr.reset(new BuiltinBug(this, "out-of-bound array access",
                                                  "Access out-of-bound array element (buffer overflow)"));
            }

            auto report = std::make_unique<PathSensitiveBugReport>(*_builtin_ptr, _builtin_ptr->getDescription(), node);

            report->addRange(statement->getSourceRange());

            context.emitReport(std::move(report));
            return;
        }
        context.addTransition(st_in_bound);
    }
}

CSA_GENERATE_REGISTER_FUNCTION_DEF(ArrayBoundChecker)