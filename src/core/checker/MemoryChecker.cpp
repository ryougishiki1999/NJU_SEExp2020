#include "MemoryChecker.h"
#include "../Logger.h"
#include <clang/StaticAnalyzer/Core/BugReporter/BugType.h>
#include <clang/StaticAnalyzer/Core/BugReporter/CommonBugCategories.h>
#include <clang/StaticAnalyzer/Core/PathSensitive/CallEvent.h>
#include <clang/StaticAnalyzer/Core/PathSensitive/CheckerContext.h>
#include <vector>
// The checker for memory management, check memory leak, use-after-free,
// double-free, inspired by clang Static Analyzer MallocChecker.
REGISTER_MAP_WITH_PROGRAMSTATE(MappedMemoryState, clang::ento::SymbolRef, MemoryState)

// MemoryChecker
void MemoryChecker::checkNewAllocator(const clang::CXXNewExpr *new_expr, clang::ento::SVal target,
                                      clang::ento::CheckerContext &ctx) const
{
    if (ctx.wasInlined)
        return;
    if (!new_expr->isArray())
        handle_new(ctx, new_expr, target, MemoryState::CXXNew);
    else
        handle_new(ctx, new_expr, target, MemoryState::CXXNewArray);
}
void MemoryChecker::checkLocation(clang::ento::SVal loc, bool is_load, const clang::Stmt *stmt,
                                  clang::ento::CheckerContext &ctx) const
{
    clang::ento::SymbolRef memory = loc.getLocSymbolInBase();
    if (memory)
        check_use_after_free(ctx, memory, stmt);
}
void MemoryChecker::checkDeadSymbols(clang::ento::SymbolReaper &sym_reaper, clang::ento::CheckerContext &ctx) const
{
    // check memory leak
    clang::ento::ProgramStateRef state = ctx.getState();
    MappedMemoryStateTy ms = state->get<MappedMemoryState>();
    MappedMemoryStateTy::Factory &ms_f = state->get_context<MappedMemoryState>();
    std::vector<clang::ento::SymbolRef> leak_memory;
    MappedMemoryStateTy new_ms = ms;
    for (auto i : new_ms)
    {
        if (sym_reaper.isDead(i.first))
        {
            if (i.second.state == MemoryState::Allocated)
                leak_memory.push_back(i.first);
            new_ms = ms_f.remove(new_ms, i.first);
        }
    }
    if (new_ms == ms)
        return;
    clang::ento::ExplodedNode *node = ctx.getPredecessor();
    if (!leak_memory.empty())
    {
        static clang::ento::CheckerProgramPointTag tag("MemoryChecker", "MemoryLeak");
        node = ctx.generateNonFatalErrorNode(ctx.getState(), &tag);
        if (node)
        {
            for (auto &i : leak_memory)
            {
                report_memory_leak(ctx, i, node);
            }
        }
    }
    ctx.addTransition(state->set<MappedMemoryState>(new_ms), node);
}
void MemoryChecker::checkPreCall(const clang::ento::CallEvent &call, clang::ento::CheckerContext &ctx) const
{
    // skip the destructor, since we check delete in postcall
    if (llvm::dyn_cast<clang::ento::CXXDestructorCall>(&call))
    {
        return;
    }
    // check whether call is free call to check double free in post call
    if (const clang::ento::AnyFunctionCall *c = llvm::dyn_cast<clang::ento::AnyFunctionCall>(&call))
    {
        const clang::FunctionDecl *decl = c->getDecl();
        if (!decl)
            return;
        if (deallocate_function_map.lookup(call))
        {
            return;
        }
        if (is_new_delete(decl))
        {
            return;
        }
    }
    // check callee
    if (const clang::ento::CXXInstanceCall *c = llvm::dyn_cast<clang::ento::CXXInstanceCall>(&call))
    {
        clang::ento::SymbolRef memory = c->getCXXThisVal().getAsSymbol();
        if (!memory || check_use_after_free(ctx, memory, c->getCXXThisExpr()))
            return;
    }
    // check args
    for (auto i = 0; i < call.getNumArgs(); ++i)
    {
        clang::ento::SVal arg = call.getArgSVal(i);
        if (arg.getAs<clang::ento::Loc>())
        {
            clang::ento::SymbolRef memory = arg.getAsSymbol();
            if (memory && check_use_after_free(ctx, memory, call.getArgExpr(i)))
                return;
        }
    }
}
void MemoryChecker::checkPostCall(const clang::ento::CallEvent &call, clang::ento::CheckerContext &ctx) const
{
    if (ctx.wasInlined)
        return;
    const clang::CallExpr *expr = llvm::dyn_cast_or_null<clang::CallExpr>(call.getOriginExpr());
    if (!expr)
        return;
    const clang::FunctionDecl *decl = ctx.getCalleeDecl(expr);
    if (!decl)
        return;
    if (const MemoryChecker::CheckFunctionType *func = allocate_function_map.lookup(call))
    {
        (*func)(this, ctx, expr, ctx.getState());
        return;
    }
    if (const MemoryChecker::CheckFunctionType *func = deallocate_function_map.lookup(call))
    {
        (*func)(this, ctx, expr, ctx.getState());
        return;
    }
    // for new/delete expression
    // e.g. void *p = operator new(sizeof(int));
    // operator delete(p);
    if (is_new_delete(llvm::cast<clang::FunctionDecl>(call.getDecl())))
    {
        check_new_delete(ctx, expr, ctx.getState());
        return;
    }
}
void MemoryChecker::checkPostStmt(const clang::CXXNewExpr *new_expr, clang::ento::CheckerContext &ctx) const
{
    if (!ctx.getAnalysisManager().getAnalyzerOptions().MayInlineCXXAllocator)
    {
        if (!new_expr->isArray())
            handle_new(ctx, new_expr, ctx.getSVal(new_expr), MemoryState::CXXNew);
        else
            handle_new(ctx, new_expr, ctx.getSVal(new_expr), MemoryState::CXXNewArray);
    }
}
void MemoryChecker::checkPreStmt(const clang::CXXDeleteExpr *delete_expr, clang::ento::CheckerContext &ctx) const
{
    if (!is_new_delete(delete_expr->getOperatorDelete()))
        return;

    clang::ento::ProgramStateRef state = ctx.getState();
    state = handle_deallocation(ctx, delete_expr, delete_expr->getArgument(), state);
    ctx.addTransition(state);
}
clang::ento::ProgramStateRef MemoryChecker::evalAssume(clang::ento::ProgramStateRef state, clang::ento::SVal cond,
                                                       bool assumption) const
{
    // this function used to remove NULL pointer from tracked memory state to reduce false positive of memory leak
    MappedMemoryStateTy memory = state->get<MappedMemoryState>();
    clang::ento::ConstraintManager &constraint_mgr = state->getConstraintManager();
    for (auto i = memory.begin(); i != memory.end(); ++i)
    {
        clang::ento::ConditionTruthVal isnull = constraint_mgr.isNull(state, i.getKey());
        if (isnull.isConstrainedTrue())
        {
            state = state->remove<MappedMemoryState>(i.getKey());
        }
    }
    return state;
}
void MemoryChecker::checkPreStmt(const clang::ReturnStmt *return_stmt, clang::ento::CheckerContext &ctx) const
{
    handle_return(return_stmt, ctx);
}
void MemoryChecker::checkEndFunction(const clang::ReturnStmt *return_stmt, clang::ento::CheckerContext &ctx) const
{
    handle_return(return_stmt, ctx);
}
bool MemoryChecker::is_deallocated(clang::ento::CheckerContext &ctx, clang::ento::SymbolRef memory) const
{
    const MemoryState *ms = ctx.getState()->get<MappedMemoryState>(memory);
    if (ms && ms->state == MemoryState::Deallocated)
    {
        return true;
    }
    return false;
}
bool MemoryChecker::is_new_delete(const clang::FunctionDecl *decl) const
{
    if (!decl)
        return false;
    clang::OverloadedOperatorKind k = decl->getOverloadedOperator();
    if (k != clang::OO_New && k != clang::OO_Array_New && k != clang::OO_Delete && k != clang::OO_Array_Delete)
        return false;
    clang::SourceLocation l = decl->getLocation();
    return !l.isValid() || decl->getASTContext().getSourceManager().isInSystemHeader(l);
}

clang::ento::ProgramStateRef MemoryChecker::register_allocation_memory_state(clang::ento::CheckerContext &ctx,
                                                                             const clang::Expr *expr,
                                                                             clang::ento::ProgramStateRef state,
                                                                             MemoryState::Function alloc_func,
                                                                             clang::ento::SVal memory) const
{
    if (!state)
        return nullptr;
    // not a location
    if (!memory.getAs<clang::ento::Loc>())
        return nullptr;
    // register it
    return state->set<MappedMemoryState>(
        memory.getAsLocSymbol(), MemoryState(MemoryState::Allocated, alloc_func, const_cast<clang::Expr *>(expr)));
}
clang::ento::ProgramStateRef MemoryChecker::handle_allocation(clang::ento::CheckerContext &ctx,
                                                              const clang::CallExpr *call_expr,
                                                              clang::ento::ProgramStateRef state,
                                                              MemoryState::Function alloc_func) const
{
    // track the allocation.
    // invalid state
    if (!state)
        return nullptr;
    // invalid return value
    if (!clang::ento::Loc::isLocType(call_expr->getType()))
        return nullptr;
    // bind effect to the allocation stmt
    // bind heap region symbolic value.
    clang::ento::SValBuilder &sb = ctx.getSValBuilder();
    auto loc_ctx = ctx.getPredecessor()->getLocationContext();
    clang::ento::DefinedSVal memory =
        sb.getConjuredHeapSymbolVal(call_expr, loc_ctx, ctx.blockCount()).castAs<clang::ento::DefinedSVal>();
    state = state->BindExpr(call_expr, ctx.getLocationContext(), memory);
    return register_allocation_memory_state(ctx, call_expr, state, alloc_func, ctx.getSVal(call_expr));
}
clang::ento::ProgramStateRef MemoryChecker::handle_deallocation(clang::ento::CheckerContext &ctx,
                                                                const clang::Expr *call_expr,
                                                                const clang::Expr *arg_expr,
                                                                clang::ento::ProgramStateRef state) const
{
    if (!state)
        return nullptr;
    clang::ento::SVal pointer = ctx.getSVal(arg_expr);
    // check region
    const clang::ento::MemRegion *region = pointer.getAsRegion();
    if (!region)
        return nullptr;
    region = region->StripCasts();
    const clang::ento::SymbolicRegion *sym_region =
        llvm::dyn_cast<clang::ento::SymbolicRegion>(region->getBaseRegion());
    if (!sym_region)
        return nullptr;
    clang::ento::SymbolRef memory = sym_region->getSymbol();
    auto memory_state = state->get<MappedMemoryState>(memory);
    if (memory_state)
    {
        if (memory_state->state == MemoryState::Deallocated)
        {
            report_double_free(ctx, call_expr->getSourceRange(), memory);
        }
        else
        {
            // TODO MAKE ILLEGAL <ALLOCATION,DEALLOCATION> PAIR AS MEMORY LEAK
        }
        // update memory state
        return state->set<MappedMemoryState>(
            memory, MemoryState(MemoryState::Deallocated, memory_state->alloc_func, memory_state->bind_stmt));
    }
    return nullptr;
}

void MemoryChecker::handle_new(clang::ento::CheckerContext &ctx, const clang::CXXNewExpr *expr,
                               clang::ento::SVal memory, MemoryState::Function alloc_func) const
{
    if (!is_new_delete(expr->getOperatorNew()))
        return;
    clang::ento::ProgramStateRef state = ctx.getState();
    state = register_allocation_memory_state(ctx, expr, state, alloc_func, memory);
    ctx.addTransition(state);
}
void MemoryChecker::handle_return(const clang::ReturnStmt *return_stmt, clang::ento::CheckerContext &ctx) const
{
    // return deallocated memory is use-after-free
    if (!return_stmt)
        return;
    const clang::Expr *expr = return_stmt->getRetValue();
    if (!expr)
        return;
    clang::ento::ProgramStateRef state = ctx.getState();
    clang::ento::SVal return_value = ctx.getSVal(expr);
    clang::ento::SymbolRef memory = return_value.getAsSymbol();
    if (memory)
    {
        check_use_after_free(ctx, memory, expr);
    }
}

void MemoryChecker::check_malloc(clang::ento::CheckerContext &ctx, const clang::CallExpr *expr,
                                 clang::ento::ProgramStateRef state) const
{
    state = handle_allocation(ctx, expr, state, MemoryState::Malloc);
    ctx.addTransition(state);
}
void MemoryChecker::check_free(clang::ento::CheckerContext &ctx, const clang::CallExpr *expr,
                               clang::ento::ProgramStateRef state) const
{
    state = handle_deallocation(ctx, expr, expr->getArg(0), state);
    ctx.addTransition(state);
}
void MemoryChecker::check_new_delete(clang::ento::CheckerContext &ctx, const clang::CallExpr *expr,
                                     clang::ento::ProgramStateRef state) const
{
    const clang::FunctionDecl *decl = ctx.getCalleeDecl(expr);
    switch (decl->getOverloadedOperator())
    {
    case clang::OO_New:
        state = handle_allocation(ctx, expr, state, MemoryState::CXXNew);
        break;
    case clang::OO_Array_New:
        state = handle_allocation(ctx, expr, state, MemoryState::CXXNewArray);
        break;
    case clang::OO_Delete:
        state = handle_deallocation(ctx, expr, expr->getArg(0), state);
        break;
    case clang::OO_Array_Delete:
        state = handle_deallocation(ctx, expr, expr->getArg(0), state);
        break;
    }

    ctx.addTransition(state);
}
bool MemoryChecker::check_use_after_free(clang::ento::CheckerContext &ctx, clang::ento::SymbolRef memory,
                                         const clang::Stmt *stmt) const
{
    if (is_deallocated(ctx, memory))
    {
        report_use_after_free(ctx, stmt->getSourceRange(), memory);
        return true;
    }
    return false;
}

void MemoryChecker::report_memory_leak(clang::ento::CheckerContext &ctx, clang::ento::SymbolRef memory,
                                       clang::ento::ExplodedNode *node) const
{
    if (!memory_leak_bug_type)
    {
        memory_leak_bug_type.reset(new clang::ento::BugType(this, "Memory Leak", clang::ento::categories::MemoryError));
    }
    // get leak allocation
    clang::ento::ExplodedNode *alloc_node = node;
    const clang::ento::MemRegion *region = nullptr;
    const clang::LocationContext *leak_loc_ctx = node->getLocationContext();
    for (auto n = node; n;)
    {
        clang::ento::ProgramStateRef s = n->getState();
        if (!s->get<MappedMemoryState>(memory))
            break;
        if (!region)
        {
            if (const clang::ento::MemRegion *mem_region = ctx.getLocationRegionIfPostStore(n))
            {
                if (s->getSVal(mem_region).getAsLocSymbol() == memory)
                {
                    const clang::ento::VarRegion *var_region =
                        mem_region->getBaseRegion()->getAs<clang::ento::VarRegion>();
                    if (!var_region || (var_region->getStackFrame() == leak_loc_ctx->getStackFrame()))
                        region = mem_region;
                }
            }
        }
        // backtracing
        const clang::LocationContext *cur_ctx = n->getLocationContext();
        if (cur_ctx == leak_loc_ctx || cur_ctx->isParentOf(leak_loc_ctx))
            alloc_node = n;
        if (n->pred_empty())
            n = nullptr;
        else
            n = *(n->pred_begin());
    }
    // report
    clang::ento::PathDiagnosticLocation loc;
    if (const clang::Stmt *s = alloc_node->getStmtForDiagnostics())
    {
        loc = clang::ento::PathDiagnosticLocation::createBegin(s, ctx.getSourceManager(),
                                                               alloc_node->getLocationContext());
    }
    std::string msg;
    llvm::raw_string_ostream os(msg);
    os << "Potential memory leak";
    if (region && region->canPrintPretty())
    {
        os << " at pointer ";
        region->printPretty(os);
        std::unique_ptr<clang::ento::PathSensitiveBugReport> report =
            std::make_unique<clang::ento::PathSensitiveBugReport>(*memory_leak_bug_type, os.str(), node, loc,
                                                                  alloc_node->getLocationContext()->getDecl());
        report->markInteresting(memory);
        ctx.emitReport(std::move(report));
    }
}

void MemoryChecker::report_double_free(clang::ento::CheckerContext &ctx, clang::SourceRange range,
                                       clang::ento::SymbolRef memory) const
{
    if (!double_free_bug_type)
    {
        double_free_bug_type.reset(new clang::ento::BugType(this, "Double-Free", clang::ento::categories::MemoryError));
    }
    if (clang::ento::ExplodedNode *node = ctx.generateErrorNode())
    {
        std::unique_ptr<clang::ento::PathSensitiveBugReport> report =
            std::make_unique<clang::ento::PathSensitiveBugReport>(*double_free_bug_type,
                                                                  "Attempt to free released memory", node);
        report->addRange(range);
        report->markInteresting(memory);
        ctx.emitReport(std::move(report));
    }
}
void MemoryChecker::report_use_after_free(clang::ento::CheckerContext &ctx, clang::SourceRange range,
                                          clang::ento::SymbolRef memory) const
{
    if (!use_after_free_bug_type)
    {
        use_after_free_bug_type.reset(
            new clang::ento::BugType(this, "Use-After-Free", clang::ento::categories::MemoryError));
    }
    if (clang::ento::ExplodedNode *node = ctx.generateErrorNode())
    {
        std::unique_ptr<clang::ento::PathSensitiveBugReport> report =
            std::make_unique<clang::ento::PathSensitiveBugReport>(*use_after_free_bug_type,
                                                                  "Attempt to use released memory", node);
        report->addRange(range);
        report->markInteresting(memory);
        ctx.emitReport(std::move(report));
    }
}
CSA_GENERATE_REGISTER_FUNCTION_DEF(MemoryChecker)