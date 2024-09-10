#ifndef CHECKER_ARRAYBOUND_CHECKER_H
#define CHECKER_ARRAYBOUND_CHECKER_H
#include "../CSADriver.h"
#include <clang/StaticAnalyzer/Checkers/BuiltinCheckerRegistration.h>
#include <clang/StaticAnalyzer/Core/BugReporter/BugType.h>
#include <clang/StaticAnalyzer/Core/Checker.h>
#include <clang/StaticAnalyzer/Core/PathSensitive/CheckerContext.h>
#include <clang/StaticAnalyzer/Core/PathSensitive/DynamicSize.h>
#include <clang/StaticAnalyzer/Core/PathSensitive/ExprEngine.h>

class ArrayBoundChecker : public clang::ento::Checker<clang::ento::check::Location>
{
    mutable std::unique_ptr<clang::ento::BuiltinBug> _builtin_ptr;

  public:
    void checkLocation(clang::ento::SVal left, bool is_load, const clang::Stmt *statement, clang::ento::CheckerContext &context) const;
};

CSA_GENERATE_REGISTER_FUNCTION_DECL(ArrayBoundChecker)

#endif
