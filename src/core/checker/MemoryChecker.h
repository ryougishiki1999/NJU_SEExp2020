#ifndef CHECKER_MEMORY_CHECKER_H
#define CHECKER_MEMORY_CHECKER_H

#include "../CSADriver.h"
#include <clang/StaticAnalyzer/Core/BugReporter/BugReporterVisitors.h>
#include <clang/StaticAnalyzer/Core/Checker.h>
#include <clang/StaticAnalyzer/Core/PathSensitive/CallEvent.h>
#include <string>
#include <unordered_map>
// The checker for memory management, check memory leak, use-after-free,
// double-free, inspired by clang Static Analyzer MallocChecker.

namespace clang
{
namespace ento
{
class AnalysisManager;
class BugReporter;
class BugType;
} // namespace ento
} // namespace clang

// This class represents the state of a memory region
class MemoryState
{
  public:
    enum State
    {
        Allocated,
        Deallocated,
        Unknown,
    };
    enum Function
    {
        Malloc,
        Free,
        CXXNew,
        CXXNewArray,
        CXXDelete,
        CXXDeleteArray
    };

  public:
    MemoryState(State s, Function func, clang::Stmt *stmt) : state(s), alloc_func(func), bind_stmt(stmt)
    {
    }

    bool operator==(const MemoryState &other) const
    {
        return state == other.state && alloc_func == other.alloc_func && bind_stmt == other.bind_stmt;
    }
    void Profile(llvm::FoldingSetNodeID &ID) const
    {
        ID.AddInteger(state);
        ID.AddInteger(alloc_func);
        ID.AddPointer(bind_stmt);
    }
    State state;
    Function alloc_func;
    clang::Stmt *bind_stmt;
};

class MemoryChecker
    : public clang::ento::Checker<clang::ento::check::NewAllocator, clang::ento::check::Location,
                                  clang::ento::check::DeadSymbols, /*clang::ento::check::PointerEscape,
                                  clang::ento::check::ConstPointerEscape,*/
                                  clang::ento::check::PreCall, clang::ento::check::PostCall,
                                  clang::ento::check::PostStmt<clang::CXXNewExpr>,
                                  clang::ento::check::PreStmt<clang::CXXDeleteExpr>, clang::ento::eval::Assume,
                                  clang::ento::check::PreStmt<clang::ReturnStmt>, clang::ento::check::EndFunction>
{

  public:
    void checkNewAllocator(const clang::CXXNewExpr *new_expr, clang::ento::SVal target,
                           clang::ento::CheckerContext &ctx) const;
    void checkLocation(clang::ento::SVal loc, bool is_load, const clang::Stmt *stmt,
                       clang::ento::CheckerContext &ctx) const;
    void checkDeadSymbols(clang::ento::SymbolReaper &sym_reaper, clang::ento::CheckerContext &ctx) const;
    void checkPreCall(const clang::ento::CallEvent &call, clang::ento::CheckerContext &ctx) const;
    void checkPostCall(const clang::ento::CallEvent &call, clang::ento::CheckerContext &ctx) const;
    void checkPostStmt(const clang::CXXNewExpr *new_expr, clang::ento::CheckerContext &ctx) const;
    void checkPreStmt(const clang::CXXDeleteExpr *delete_expr, clang::ento::CheckerContext &ctx) const;
    clang::ento::ProgramStateRef evalAssume(clang::ento::ProgramStateRef state, clang::ento::SVal cond,
                                            bool assumption) const;
    void checkPreStmt(const clang::ReturnStmt *return_stmt, clang::ento::CheckerContext &ctx) const;
    void checkEndFunction(const clang::ReturnStmt *return_stmt, clang::ento::CheckerContext &ctx) const;

  public:
    // helper
    bool is_deallocated(clang::ento::CheckerContext &ctx, clang::ento::SymbolRef memory) const;
    bool is_new_delete(const clang::FunctionDecl *decl) const;

    // handler
    clang::ento::ProgramStateRef register_allocation_memory_state(clang::ento::CheckerContext &ctx,
                                                                  const clang::Expr *e,
                                                                  clang::ento::ProgramStateRef state,
                                                                  MemoryState::Function alloc_func,
                                                                  clang::ento::SVal pointer) const;
    clang::ento::ProgramStateRef handle_allocation(clang::ento::CheckerContext &ctx, const clang::CallExpr *call_expr,
                                                   clang::ento::ProgramStateRef state,
                                                   MemoryState::Function alloc_func) const;
    clang::ento::ProgramStateRef handle_deallocation(clang::ento::CheckerContext &ctx, const clang::Expr *call_expr,
                                                     const clang::Expr *arg_expr,
                                                     clang::ento::ProgramStateRef state) const;
    void handle_new(clang::ento::CheckerContext &ctx, const clang::CXXNewExpr *expr, clang::ento::SVal memory,
                    MemoryState::Function alloc_func) const;
    void handle_return(const clang::ReturnStmt *return_stmt, clang::ento::CheckerContext &ctx) const;

    // checker
    void check_malloc(clang::ento::CheckerContext &ctx, const clang::CallExpr *expr,
                      clang::ento::ProgramStateRef state) const;
    void check_free(clang::ento::CheckerContext &ctx, const clang::CallExpr *expr,
                    clang::ento::ProgramStateRef state) const;
    void check_new_delete(clang::ento::CheckerContext &ctx, const clang::CallExpr *expr,
                          clang::ento::ProgramStateRef state) const;
    bool check_use_after_free(clang::ento::CheckerContext &ctx, clang::ento::SymbolRef sym,
                              const clang::Stmt *stmt) const;
    // reporter
    void report_memory_leak(clang::ento::CheckerContext &ctx, clang::ento::SymbolRef memory,
                            clang::ento::ExplodedNode *node) const;
    void report_double_free(clang::ento::CheckerContext &ctx, clang::SourceRange range,
                            clang::ento::SymbolRef memory) const;
    void report_use_after_free(clang::ento::CheckerContext &ctx, clang::SourceRange range,
                               clang::ento::SymbolRef memory) const;

  private:
    mutable std::unique_ptr<clang::ento::BugType> memory_leak_bug_type;
    mutable std::unique_ptr<clang::ento::BugType> double_free_bug_type;
    mutable std::unique_ptr<clang::ento::BugType> use_after_free_bug_type;

  public:
    using CheckFunctionType = std::function<void(const MemoryChecker *, clang::ento::CheckerContext &,
                                                 const clang::CallExpr *, clang::ento::ProgramStateRef)>;
    const clang::ento::CallDescriptionMap<CheckFunctionType> deallocate_function_map{
        {{"free", 1}, &MemoryChecker::check_free}};
    const clang::ento::CallDescriptionMap<CheckFunctionType> allocate_function_map{
        {{"malloc", 1}, &MemoryChecker::check_malloc}};
};
CSA_GENERATE_REGISTER_FUNCTION_DECL(MemoryChecker)
#endif