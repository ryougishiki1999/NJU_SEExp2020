#include "DereferenceChecker.h"

// get dereference expr from param stmt
const clang::Expr *DereferenceChecker::get_dereference_expr(const clang::Stmt *stmt, bool is_bind)
{
    const clang::Expr *ret_expr = nullptr;
    // get the original expr that syntactocally cause the load
    const clang::Expr *expr = clang::dyn_cast<clang::Expr>(stmt);
    if (expr)
    {
        ret_expr = expr->IgnoreParenLValueCasts();
    }
    // if bind expr(var = init),get init expr
    if (is_bind)
    {
        const clang::VarDecl *var;
        const clang::Expr *init;
        std::tie(var, init) = clang::ento::parseAssignment(stmt);
        if (var && init)
        {
            ret_expr = init;
        }
    }
    return ret_expr;
}

// add source for dereference expr
void DereferenceChecker::add_deref_source(clang::raw_ostream &os, clang::SmallVectorImpl<clang::SourceRange> &ranges,
                                          const clang::Expr *expr, const clang::ento::ProgramState *state,
                                          const clang::LocationContext *lctx, bool loaded_from)
{
    //wipe out parentheses and l value casts
    expr = expr->IgnoreParenLValueCasts();
    switch (expr->getStmtClass())
    {
        //when the class of error statement is a declation reference
        //expression, add the source infomation of the error variable
    case clang::Stmt::DeclRefExprClass: {
        const clang::DeclRefExpr *decl_ref_expr = 
            clang::cast<clang::DeclRefExpr>(expr);
        if (const clang::VarDecl *var_decl = 
            clang::dyn_cast<clang::VarDecl>(decl_ref_expr->getDecl()))
        {
            os << " (" << (loaded_from ? "loaded from" : "from") 
                << " variable '" << var_decl->getName() << "')";
            ranges.push_back(decl_ref_expr->getSourceRange());
        }
        break;
    }
        // when the class of error statement is a member expression, add 
        // the source infomation of the error field
    case clang::Stmt::MemberExprClass: {
        const clang::MemberExpr *member_expr = clang::cast<clang::MemberExpr>(expr);
        os << " (" << (loaded_from ? "loaded from" : "via") 
            << " field '" << member_expr->getMemberNameInfo() << "')";
        clang::SourceLocation l = member_expr->getMemberLoc();
        ranges.push_back(clang::SourceRange(l, l));
        break;
    }
    default:
        break;
    }
}

//return true when expr is a decl ref expr to a reference var
bool DereferenceChecker::is_decl_ref_expr_to_reference(const clang::Expr *expr)
{
    if (const auto *decl_ref_expr = clang::dyn_cast<clang::DeclRefExpr>(expr))
    {
        //get the declaration of the expr
        const clang::ValueDecl *decl = decl_ref_expr->getDecl();
        //get the type of the declaration
        const clang::QualType type = decl->getType();
        //if the type if reference type, return true
        return type->isReferenceType();
    }
    else
    {
        return false;
    }
}

// report direct null dereferenced
void DereferenceChecker::report_direct_dereference(clang::ento::ProgramStateRef state, const clang::Stmt *stmt,
                                                   clang::ento::CheckerContext &ctx) const
{
    // init direct
    if (!direct)
        direct.reset(new clang::ento::BugType(this, "Null-Pointer-Dereferenced", clang::ento::categories::MemoryError));
    // generate an error node
    clang::ento::ExplodedNode *node = ctx.generateErrorNode(state);
    if (!node)
        return;
    //add some infomation for report
    clang::SmallString<100> buf;
    llvm::raw_svector_ostream os(buf);
    clang::SmallVector<clang::SourceRange, 2> ranges;
    switch (stmt->getStmtClass())
    {
        // when the class of the error statement is array subscript expression
        // add the array access information for error description
    case clang::Stmt::ArraySubscriptExprClass: {
        os << "Array access";
        const clang::ArraySubscriptExpr *array_sub_expr = clang::cast<clang::ArraySubscriptExpr>(stmt);
        add_deref_source(os, ranges, array_sub_expr->getBase()->IgnoreParenCasts(), state.get(),
                         node->getLocationContext());
        os << " results in a null pointer dereference";
        break;
    }
        // when the class of the error statement is unary operator expression
        // add information for error description
    case clang::Stmt::UnaryOperatorClass: {
        os << "Dereference of null pointer";
        const clang::UnaryOperator *unary_operator = clang::cast<clang::UnaryOperator>(stmt);
        add_deref_source(os, ranges, unary_operator->getSubExpr()->IgnoreParens(), state.get(),
                         node->getLocationContext(), true);
        break;
    }
        // when the class of the error statement is member expression
        // add the field access information for error description
    case clang::Stmt::MemberExprClass: {
        const clang::MemberExpr *member_expr = clang::cast<clang::MemberExpr>(stmt);
        if (member_expr->isArrow() || is_decl_ref_expr_to_reference(member_expr->getBase()))
        {
            os << "Access to field '" << member_expr->getMemberNameInfo()
               << "' results in a dereference of a null pointer";
            add_deref_source(os, ranges, member_expr->getBase()->IgnoreParenCasts(), state.get(),
                             node->getLocationContext(), true);
        }
        break;
    }
    default:
        break;
    }
    // generate report
    auto report = std::make_unique<clang::ento::PathSensitiveBugReport>(
        *direct, buf.empty() ? "Dereference of null pointer" : clang::StringRef(buf), node);
    clang::ento::bugreporter::trackExpressionValue(node, clang::ento::bugreporter::getDerefExpr(stmt), *report);
    //add ranges for error report
    for (clang::SmallVectorImpl<clang::SourceRange>::iterator 
        i = ranges.begin(), e = ranges.end(); i != e; i++)
    {
        report->addRange(*i);
    }
    ctx.emitReport(std::move(report));
}
// trigger when a memory location's value is access(load or store)
void DereferenceChecker::checkLocation(clang::ento::SVal location, bool is_load, const clang::Stmt *stmt,
                                       clang::ento::CheckerContext &ctx) const
{
    // if the pointer is undefined
    if (location.isUndef())
        return;
    clang::ento::DefinedOrUnknownSVal loc = location.castAs<clang::ento::DefinedOrUnknownSVal>();
    if (!loc.getAs<clang::ento::Loc>())
        return;
    // get program state
    clang::ento::ProgramStateRef state = ctx.getState();
    clang::ento::ProgramStateRef not_null_state, null_state;
    // judge the value of location can be null or nonnull or both
    std::tie(not_null_state, null_state) = state->assume(loc);
    if (null_state)
    {
        // explicit NULL case
        if (!not_null_state)
        {
            const clang::Expr *expr = get_dereference_expr(stmt);
            report_direct_dereference(null_state, expr, ctx);
            return;
        }
        // the value of v is unknown(either null or nonnull),there maybe
        // an implicit null dereference. if report it, it results in a high
        // false alarm rate and blocks detection of other errors

    }
    // reach here means the pointer is not null
    ctx.addTransition(not_null_state);
}

// when binding to a reference, check if the value is known to be null
void DereferenceChecker::checkBind(clang::ento::SVal l, clang::ento::SVal v, const clang::Stmt *stmt,
                                   clang::ento::CheckerContext &ctx) const
{
    // if the value is undefined
    if (v.isUndef())
        return;
    const clang::ento::MemRegion *region = l.getAsRegion();
    const clang::ento::TypedValueRegion *typed_value_region =
        clang::dyn_cast_or_null<clang::ento::TypedValueRegion>(region);
    if (!typed_value_region)
        return;
    // check if l is a reference type
    if (!typed_value_region->getValueType()->isReferenceType())
        return;
    // get program state
    clang::ento::ProgramStateRef state = ctx.getState();
    clang::ento::ProgramStateRef not_null_state, null_state;
    // judge the value of location can be null or nonnull or both
    std::tie(not_null_state, null_state) = state->assume(v.castAs<clang::ento::DefinedOrUnknownSVal>());
    if (null_state)
    {
        // explicit NULL case
        if (!not_null_state)
        {
            const clang::Expr *expr = get_dereference_expr(stmt, true);
            report_direct_dereference(null_state, expr, ctx);
            return;
        }
        // the value of v is unknown(either null or nonnull),there maybe
        // an implicit null dereference. if report it, it results in a high
        // false alarm rate and blocks detection of other errors
    }
    //report the error when the l is used.like this:
    //int &b=*a;
    //if(a!=NULL) return;
    //int c=b; error!
    //to achieve this effect, do not transite the program state.
    ctx.addTransition(state, this);
}

CSA_GENERATE_REGISTER_FUNCTION_DEF(DereferenceChecker)