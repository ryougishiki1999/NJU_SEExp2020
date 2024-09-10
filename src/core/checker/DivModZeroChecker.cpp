#include "DivModZeroChecker.h"

using namespace clang;
using namespace ento;

void DivModZeroChecker::report_div_zero(const char *msg, ProgramStateRef state_zero, CheckerContext &ctx,
                                        std::unique_ptr<BugReporterVisitor> visitor) const
{
    if (ExplodedNode *node = ctx.generateErrorNode(state_zero))
    {
        if (!btin)
            btin.reset(new BuiltinBug(this, "Division or Mod by zero"));

        auto report = std::make_unique<PathSensitiveBugReport>(*btin, msg, node);
        report->addVisitor(std::move(visitor));
        bugreporter::trackExpressionValue(node, getDenomExpr(node), *report);
        ctx.emitReport(std::move(report));
    }
}

void DivModZeroChecker::checkPreStmt(const BinaryOperator *bop, CheckerContext &ctx) const
{
    BinaryOperator::Opcode op = bop->getOpcode();
    if (op != BO_Div && op != BO_Rem && op != BO_DivAssign && op != BO_RemAssign)
        return;

    if (!bop->getRHS()->getType()->isScalarType())
        return;

    SVal Denom = ctx.getSVal(bop->getRHS());
    Optional<DefinedSVal> dv = Denom.getAs<DefinedSVal>();

    // Divide-by-undefined handled in the generic checking for uses of undefined values.
    if (!dv)
        return;

    // Check for divide by zero.
    ConstraintManager &cm = ctx.getConstraintManager();
    ProgramStateRef state_not_zero, state_zero;
    std::tie(state_not_zero, state_zero) = cm.assumeDual(ctx.getState(), *dv);

    if (!state_not_zero)
    {
        assert(state_zero);
        report_div_zero("Division or Mod by zero", state_zero, ctx);
        return;
    }

    // If we get here, then the denom should not be zero. We abandon the implicit
    // zero denom case for now.
    ctx.addTransition(state_not_zero);
}
CSA_GENERATE_REGISTER_FUNCTION_DEF(DivModZeroChecker)