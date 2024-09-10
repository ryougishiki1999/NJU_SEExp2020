#ifndef CHECKER_DEREFERENCE_CHECKER_H
#define CHECKER_DEREFERENCE_CHECKER_H

#include "../CSADriver.h"

#include <clang/StaticAnalyzer/Core/BugReporter/BugType.h>
#include <clang/StaticAnalyzer/Core/Checker.h>
#include <clang/StaticAnalyzer/Core/CheckerManager.h>
#include <clang/StaticAnalyzer/Core/PathSensitive/CheckerContext.h>
#include <clang/StaticAnalyzer/Core/PathSensitive/CheckerHelpers.h>

#include <llvm/ADT/SmallString.h>
#include <llvm/Support/raw_ostream.h>

// The checker for  null pointer dereference,
// inspired by clang Static Analyzer DereferenceChecker.
class DereferenceChecker : public clang::ento::Checker<clang::ento::check::Location, clang::ento::check::Bind>
{
    mutable std::unique_ptr<clang::ento::BugType> direct;
    // reporter
    // Param state:program status information in symbolic execution flow
    // Param stmt:the statement detected a null pointer dereference error
    // Param ctx:the checker context infomation
    void report_direct_dereference(clang::ento::ProgramStateRef state, const clang::Stmt *stmt,
                                   clang::ento::CheckerContext &ctx) const;
    // helper
    // Param stmt:the statement detected a null pointer dereference error
    // Param is_bind:indicate if it is a bind stmt
    static const clang::Expr *get_dereference_expr(const clang::Stmt *stmt, bool is_bind = false);
    // Param os:input stream for describing information
    // Param ranges:source ranges for error description
    // Param expr:the expression detected a null pointer dereference error
    // Param state:program status information in symbolic execution flow
    // Param lctx:the location context infomation
    // Param loaded_from:indicate if it is loaded form a pointer or not
    static void add_deref_source(clang::raw_ostream &os, clang::SmallVectorImpl<clang::SourceRange> &ranges,
                          const clang::Expr *expr, const clang::ento::ProgramState *state,
                          const clang::LocationContext *lctx, bool loaded_from = false);
    // Param expr:expression waiting to be detected
    static bool is_decl_ref_expr_to_reference(const clang::Expr *expr);

  public:
    // Param location:location being accessed
    // Param is_load:indicate if it is loaded or not, true means read and false means write
    // Param stmt:the statement to be checked
    // Param ctx:the checker context infomation
    void checkLocation(clang::ento::SVal location, bool is_load, const clang::Stmt *stmt,
                       clang::ento::CheckerContext &ctx) const;
    // Param l:the location to be binded a value
    // Patam v:the value will bind on the location
    // Param stmt:the statement to be checked
    // Param ctx:the checker context infomation
    void checkBind(clang::ento::SVal l, clang::ento::SVal v, const clang::Stmt *stmt,
                   clang::ento::CheckerContext &ctx) const;
};

CSA_GENERATE_REGISTER_FUNCTION_DECL(DereferenceChecker)
#endif // !CHECKER_DEREFERENCE_CHECKER_H
