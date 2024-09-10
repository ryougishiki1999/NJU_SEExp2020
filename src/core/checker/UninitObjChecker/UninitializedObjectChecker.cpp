#include "UninitializedObjectChecker.h"
#include "UninitObjCheckerUtil.h"
#include <clang/Analysis/PathDiagnostic.h>
#include <clang/StaticAnalyzer/Core/BugReporter/BugReporter.h>
#include <clang/StaticAnalyzer/Core/PathSensitive/ExplodedGraph.h>
#include <clang/StaticAnalyzer/Core/PathSensitive/ProgramStateTrait.h>

using namespace clang;
using namespace clang::ento;

/// We'll mark fields (and pointee of fields) that are confirmed to be
/// uninitialized as already analyzed.
REGISTER_SET_WITH_PROGRAMSTATE(AnalyzedRegions, const MemRegion *)

//===----------------------------------------------------------------------------
//                      Class UninitObjChecker Method Def
//===----------------------------------------------------------------------------
void UninitObjChecker::checkEndFunction(const ReturnStmt *RS, CheckerContext &Context) const
{

    const auto *ctor_decl = dyn_cast_or_null<CXXConstructorDecl>(Context.getLocationContext()->getDecl());

    if (!ctor_decl)
    {
        return;
    }

    if (!ctor_decl->isUserProvided())
    {
        return;
    }

    if (ctor_decl->getParent()->isUnion())
        return;

    // Avoids multiple reports
    if (will_obj_analyzed_later(ctor_decl, Context))
    {
        return;
    }

    const TypedValueRegion *region = get_constructed_region(ctor_decl, Context);

    if (!region)
    {
        return;
    }

    // core steps of UninitObjChecker
    FindUninitializedFields find(Context.getState(), region);

    std::pair<ProgramStateRef, const UninitFieldMap &> uninit_info = find.get_results();

    ProgramStateRef updated_state = uninit_info.first;
    const UninitFieldMap &uninit_fieldmap = uninit_info.second;

    if (uninit_fieldmap.empty())
    {
        Context.addTransition(updated_state);
        return;
    }

    ExplodedNode *node = Context.generateNonFatalErrorNode(updated_state);
    if (!node)
    {
        return;
    }

    PathDiagnosticLocation loc_used_for_uniqueing;
    const Stmt *call_site = Context.getStackFrame()->getCallSite();
    if (call_site)
    {
        loc_used_for_uniqueing =
            PathDiagnosticLocation::createBegin(call_site, Context.getSourceManager(), node->getLocationContext());
    }

    // For Plist consumers that don't support notes just yet, we'll convert notes
    // to warnings.

    // TODO: checker options... about ShouldConvertNoteToWarnings
    // for (const auto &pair : uninit_fieldmap)
    // {
    //     auto report = std::make_unique<PathSensitiveBugReport>(
    //         *_bt_uninit_field, pair.second, node, loc_used_for_uniqueing, node->getLocationContext()->getDecl());
    //     Context.emitReport(std::move(report));
    // }

    llvm::SmallString<100> warning_buf;
    llvm::raw_svector_ostream warning_os(warning_buf);

    warning_os << "Uninits Num:" << uninit_fieldmap.size() << " ; Uninitailized field"
               << ((uninit_fieldmap.size() == 1) ? " " : "s ") << "at the end of the constructor call";

    auto report = std::make_unique<PathSensitiveBugReport>(
        *_bt_uninit_field, warning_os.str(), node, loc_used_for_uniqueing, node->getLocationContext()->getDecl());

    for (const auto &pair : uninit_fieldmap)
    {
        report->addNote(pair.second, PathDiagnosticLocation::create(pair.first->getDecl(), Context.getSourceManager()));
    }
    Context.emitReport(std::move(report));
}

void UninitObjChecker::checkDeadSymbols(SymbolReaper &SR, CheckerContext &C) const
{
    ProgramStateRef state = C.getState();
    for (const MemRegion *r : state->get<AnalyzedRegions>())
    {
        if (!SR.isLiveRegion(r))
        {
            state = state->remove<AnalyzedRegions>(r);
        }
    }
}

//===----------------------------------------------------------------------------
//                      Utility Functions Def
//===----------------------------------------------------------------------------

const clang::ento::TypedValueRegion *get_constructed_region(const CXXConstructorDecl *ctor_decl,
                                                            CheckerContext &context)
{

    Loc this_loc = context.getSValBuilder().getCXXThis(ctor_decl, context.getStackFrame());

    SVal object_v = context.getState()->getSVal(this_loc);

    auto *r = object_v.getAsRegion()->getAs<TypedValueRegion>();

    if (r && !r->getValueType()->getAsCXXRecordDecl())
    {
        return nullptr;
    }

    return r;
}

bool will_obj_analyzed_later(const CXXConstructorDecl *ctor, CheckerContext &context)
{

    const TypedValueRegion *cur_region = get_constructed_region(ctor, context);

    if (!cur_region)
    {
        return false;
    }

    const LocationContext *lc = context.getLocationContext();

    while ((lc = lc->getParent()))
    {

        const auto *other_ctor = dyn_cast<CXXConstructorDecl>(lc->getDecl());

        if (!other_ctor)
        {
            continue;
        }

        const TypedValueRegion *other_region = get_constructed_region(other_ctor, context);

        if (!other_region)
        {
            continue;
        }

        // If the CurrRegion is a subregion of OtherRegion, it will be analyzed
        // during the analysis of OtherRegion.
        if (cur_region->isSubRegionOf(other_region))
        {
            return true;
        }
    }
    return false;
}

CSA_GENERATE_REGISTER_FUNCTION_DEF(UninitObjChecker)