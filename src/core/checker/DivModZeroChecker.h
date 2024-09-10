#pragma once
#include "../CSADriver.h"
#include <clang/StaticAnalyzer/Checkers/BuiltinCheckerRegistration.h>
#include <clang/StaticAnalyzer/Core/BugReporter/BugType.h>
#include <clang/StaticAnalyzer/Core/Checker.h>
#include <clang/StaticAnalyzer/Core/CheckerManager.h>
#include <clang/StaticAnalyzer/Core/PathSensitive/CheckerContext.h>

class DivModZeroChecker : public clang::ento::Checker<clang::ento::check::PreStmt<clang::BinaryOperator>>
{
    mutable std::unique_ptr<clang::ento::BuiltinBug> btin;
    void report_div_zero(const char *msg, clang::ento::ProgramStateRef state_zero, clang::ento::CheckerContext &ctx,
                         std::unique_ptr<clang::ento::BugReporterVisitor> visitor = nullptr) const;

  public:
    void checkPreStmt(const clang::BinaryOperator *bop, clang::ento::CheckerContext &ctx) const;
};

static const clang::Expr *getDenomExpr(const clang::ento::ExplodedNode *node)
{
    const clang::Stmt *stmt = node->getLocationAs<clang::PreStmt>()->getStmt();
    if (const auto *bexpr = clang::dyn_cast<clang::BinaryOperator>(stmt))
        return bexpr->getRHS();
    return nullptr;
}
CSA_GENERATE_REGISTER_FUNCTION_DECL(DivModZeroChecker)
