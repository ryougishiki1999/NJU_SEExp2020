#ifndef CORE_CSA_DRIVER_H
#define CORE_CSA_DRIVER_H

#include "AnalyzerOptions.h"
#include "Event.h"
#include "utility/Timer.h"
#include <clang/Analysis/PathDiagnostic.h>
#include <clang/StaticAnalyzer/Frontend/FrontendActions.h>
#include <clang/Tooling/Tooling.h>
#include <iostream>
#include <map>
#include <string>
#include <unordered_set>
#include <utility>
namespace clang::ento
{
class CheckerRegistry;
class CheckerManager;
} // namespace clang::ento

/*! \brief Singleton, manage all checkers' registry, injection. adapter of Clang CheckerRegistry
 */
class CSACheckerRegistry
{
  public:
    using InitFuncType = void (*)(clang::ento::CheckerManager &);
    class CSAChecker
    {
      public:
        CSAChecker(const std::string &n, CSACheckerRegistry::InitFuncType init_f, const std::string &desc,
                   bool en = true);

      public:
        const std::string name;                           //! checker's name
        const CSACheckerRegistry::InitFuncType init_func; //! checker's initialization function
        const std::string description;                    //! checker's description
        bool enabled;                                     //! is checker enabled?
    };

  public:
    CSACheckerRegistry(const CSACheckerRegistry &) = delete;
    CSACheckerRegistry &operator=(const CSACheckerRegistry &) = delete;
    static CSACheckerRegistry &instance();

    /*! \brief register a checker
        \param[in] name checker's name
        \param[in] init_function checker's initialization function
        \param[in] desc checker's description
    */
    void register_checker(const std::string &name, InitFuncType init_function, const std::string &desc);
    /*! \brief Clang CheckerRegistry required callback(register function of Clang CheckerRegistry), to add checkers in
       CSACheckerRegistry to Clang CheckerRegistry \param[in] cr Clang CheckerRegistry
     */
    void add_checkers_to_csa(clang::ento::CheckerRegistry &cr);
    /*! \brief inject checkers to Clang AnalyzerOptions, which make Clang detect our checkers
        \param[in] opts Clang AnalyzerOptions
     */
    void inject_checkers(clang::AnalyzerOptions *opts);
    /*! \brief dump the checkers' info to the output stream
        \param[in] os output stream
    */
    void dump_checkers(std::ostream &os);
    /*! \brief reset all checker's enabled state to default(enable)
     */
    void reset_checker_state();

  private:
    CSACheckerRegistry() = default;
    ~CSACheckerRegistry() = default;

  public:
    std::map<std::string, CSAChecker> checkers;
};
// helper macro
#define CSA_GENERATE_REGISTER_FUNCTION_DECL(_checker_) void csa_register_##_checker_(clang::ento::CheckerManager &);

#define CSA_GENERATE_REGISTER_FUNCTION_DEF(_checker_)                                                                  \
    void csa_register_##_checker_(clang::ento::CheckerManager &cm)                                                     \
    {                                                                                                                  \
        cm.registerChecker<_checker_>();                                                                               \
    }

#define CSA_REGISTER_FUNCTION_NAME(_checker_) csa_register_##_checker_

/*! \brief adapter of Clang FrontendAction, used to create Clang ASTConsumer, calculate analysis time and emit events
 */
class CSAAction : public clang::ento::AnalysisAction
{
  public:
    CSAAction(const AnalyzerOptions &opts, const EventCallbackTable &ev_tb);
    ~CSAAction();

  protected:
    std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(clang::CompilerInstance &CI, llvm::StringRef InFile) override;

  private:
    AnalyzerOptions options;
    EventCallbackTable event_table;
    std::string filename;
    Timer timer;
};
/*! \brief adapter of Clang FrontendActionFactory, used to forward our settings to the CSAAction
 */
class CSAFactory : public clang::tooling::FrontendActionFactory
{
  public:
    CSAFactory(const AnalyzerOptions &opts, const EventCallbackTable &ev_tb);
    std::unique_ptr<clang::FrontendAction> create() override;

  private:
    AnalyzerOptions options;
    EventCallbackTable event_table;
};
/*! \brief adapter of Clang PathDiagnosticConsumer, used to call ResultCollector when new diagnostics generated
 */
class CSADiagnostics : public clang::ento::PathDiagnosticConsumer
{
  public:
    CSADiagnostics();
    ~CSADiagnostics() override;

    llvm::StringRef getName() const override;

    void FlushDiagnosticsImpl(std::vector<const clang::ento::PathDiagnostic *> &diags, FilesMade *files_made) override;
    bool supportsCrossFileDiagnostics() const
    {
        return true;
    }
};
#endif