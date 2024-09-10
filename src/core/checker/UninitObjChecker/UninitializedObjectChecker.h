#ifndef CHECKER_UNINITOBJCHECKER_UNINITIALIZEDOBJECT_CHECKER_H
#define CHECKER_UNINITOBJCHECKER_UNINITIALIZEDOBJECT_CHECKER_H
#include "../../CSADriver.h"
#include <clang/AST/Decl.h>
#include <clang/AST/Stmt.h>
#include <clang/StaticAnalyzer/Core/BugReporter/BugType.h>
#include <clang/StaticAnalyzer/Core/Checker.h>
#include <clang/StaticAnalyzer/Core/PathSensitive/CheckerContext.h>
#include <clang/StaticAnalyzer/Core/PathSensitive/SymbolManager.h>

//===----------------------------------------------------------------------===//
// This file defines a checker that reports uninitialized fields in objects
// created after a constructor call.
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------------
//                      Uitility Functions Decl
//===----------------------------------------------------------------------------

/// Returns the region that was constructed by CtorDecl, or nullptr if that
/// isn't possible.
const clang::ento::TypedValueRegion *get_constructed_region(const clang::CXXConstructorDecl *ctor_decl,
                                                            clang::ento::CheckerContext &context);

/// Checks whether the object constructed by \p Ctor will be analyzed later
/// (e.g. if the object is a field of another object, in which case we'd check
/// it multiple times).
bool will_obj_analyzed_later(const clang::CXXConstructorDecl *ctor, clang::ento::CheckerContext &context);

//===----------------------------------------------------------------------------
//                      Class UninitObjChecker Decl
//===----------------------------------------------------------------------------

class UninitObjChecker : public clang::ento::Checker<clang::ento::check::EndFunction, clang::ento::check::DeadSymbols>
{
  public:
    // TODO: checker options

    UninitObjChecker() : _bt_uninit_field(new clang::ento::BuiltinBug(this, "Uninitialized fields in UOC checker"))
    {
    }

    void checkEndFunction(const clang::ReturnStmt *RS, clang::ento::CheckerContext &C) const;

    void checkDeadSymbols(clang::ento::SymbolReaper &SR, clang::ento::CheckerContext &C) const;

  private:
    std::unique_ptr<clang::ento::BuiltinBug> _bt_uninit_field;
};

CSA_GENERATE_REGISTER_FUNCTION_DECL(UninitObjChecker)

#endif