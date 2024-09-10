#include "CSADriver.h"
#include "Logger.h"
#include "ResultCollector.h"
#include <clang/Frontend/CompilerInstance.h>
#include <clang/StaticAnalyzer/Frontend/AnalysisConsumer.h>
#include <clang/StaticAnalyzer/Frontend/CheckerRegistry.h>
#include <vector>

static bool csa_should_register_function(const clang::ento::CheckerManager &)
{
    return true;
}

/////////////////////////////////////
// CSACheckerRegistry
/////////////////////////////////////
CSACheckerRegistry::CSAChecker::CSAChecker(const std::string &n, CSACheckerRegistry::InitFuncType init_f,
                                           const std::string &desc, bool en)
    : name(n), init_func(init_f), description(desc), enabled(en)
{
}
CSACheckerRegistry &CSACheckerRegistry::instance()
{
    static CSACheckerRegistry cr;
    return cr;
}
void CSACheckerRegistry::register_checker(const std::string &name, InitFuncType init_function, const std::string &desc)
{
    std::string full_name = "hisa." + name;
    /*DEBUG("Register Checker %s to CSADriver.\n", full_name.c_str());*/
    checkers.insert(std::make_pair(full_name, CSAChecker(full_name, init_function, desc)));
}
void CSACheckerRegistry::add_checkers_to_csa(clang::ento::CheckerRegistry &cr)
{
    for (auto &chk : checkers)
    {
        /*DEBUG("Add Checker %s to CSA.\n", chk.first.c_str());*/
        cr.addChecker(chk.second.init_func, csa_should_register_function, chk.first, chk.second.description, "", false);
    }
}
void CSACheckerRegistry::inject_checkers(clang::AnalyzerOptions *opts)
{
    for (auto &chk : checkers)
    {
        /*DEBUG("Inject Checker %s to CSA Instance.\n", chk.first.c_str());*/
        opts->CheckersAndPackages.push_back(std::make_pair(chk.first, chk.second.enabled));
    }
}
void CSACheckerRegistry::dump_checkers(std::ostream &os)
{
    os << "\tChecker\tDescription\tEnabled?\n";
    for (const auto &i : checkers)
    {
        os << '\t' << i.second.name << '\t' << i.second.description << '\t' << i.second.enabled << '\n';
    }
}
void CSACheckerRegistry::reset_checker_state()
{
    for (auto &i : checkers)
    {
        i.second.enabled = true;
    }
}

static void csa_checker_registry(clang::ento::CheckerRegistry &cr)
{
    CSACheckerRegistry::instance().add_checkers_to_csa(cr);
}
/////////////////////////////////////
// CSAAction
/////////////////////////////////////
CSAAction::CSAAction(const AnalyzerOptions &opts, const EventCallbackTable &ev_tb) : options(opts), event_table(ev_tb)
{
}
CSAAction::~CSAAction()
{
    if (options.enable_stats)
    {
        timer.stop();
        ResultCollector::instance().add_stats(Stats(filename, timer.used_time()));
    }
}
std::unique_ptr<clang::ASTConsumer> CSAAction::CreateASTConsumer(clang::CompilerInstance &CI, llvm::StringRef InFile)
{
    event_table.callback(std::make_unique<AnalyzingFileEvent>(InFile.str()));
    // std::cout << "Analyzing " << InFile.str() << '\n';
    /* INFO("Analyzing %s\n", InFile.str().c_str()); */
    // inject the analyzer options here
    clang::AnalyzerOptions *opts = CI.getAnalyzerOpts().get();
    // disable builtin checkers
    if (!options.enable_god_mode)
    {
        for (auto &chk : opts->CheckersAndPackages)
        {
            chk.second = false;
        }
        // inject our checkers
        CSACheckerRegistry::instance().inject_checkers(opts);
    }
    opts->maxBlockVisitOnPath = 11;
    // todo enable specified checkers in god mode
    if (options.enable_debug_mode)
    {
        DEBUG("Running in debug mode.\n");
        opts->AnalyzerDisplayProgress = options.show_analysis_progress;
        opts->visualizeExplodedGraphWithGraphViz = options.enable_exploded_graph;
        opts->DumpExplodedGraphTo = options.debug_output_path;
        opts->TrimGraph = options.trim_exploded_graph;
    }
    else
    {
        opts->AnalysisDiagOpt = clang::AnalysisDiagClients::PD_NONE;
    }
    // for debug stats
    if (options.enable_stats && options.enable_debug_mode)
        opts->PrintStats = 1;
    // create consumer
    std::unique_ptr<clang::ento::AnalysisASTConsumer> consumer = clang::ento::CreateAnalysisConsumer(CI);
    consumer->AddDiagnosticConsumer(new CSADiagnostics);
    if (!options.enable_god_mode)
        consumer->AddCheckerRegistrationFn(csa_checker_registry);
    // stats
    if (options.enable_stats)
    {
        filename = InFile.str();
        timer.start();
    }
    return consumer;
}
/////////////////////////////////////
// CSAFactory
/////////////////////////////////////
CSAFactory::CSAFactory(const AnalyzerOptions &opts, const EventCallbackTable &ev_tb) : options(opts), event_table(ev_tb)
{
}
std::unique_ptr<clang::FrontendAction> CSAFactory::create()
{
    return std::make_unique<CSAAction>(options, event_table);
}
/////////////////////////////////////
// CSADiagnostics
/////////////////////////////////////
CSADiagnostics::CSADiagnostics()
{
}
CSADiagnostics::~CSADiagnostics()
{
}

llvm::StringRef CSADiagnostics::getName() const
{
    return "CSADiagnostics";
}

void CSADiagnostics::FlushDiagnosticsImpl(std::vector<const clang::ento::PathDiagnostic *> &diags,
                                          FilesMade *files_made)
{
    for (auto &diag : diags)
    {
        /* clang::FullSourceLoc loc = diag->getLocation().asLocation();
        ResultCollector::instance().add_diagnostic(
            Diagnostic(diag->getCheckerName().str(), diag->getBugType().str(), loc.getManager().getFilename(loc).str(),
                       diag->getShortDescription().str(), loc.getSpellingLineNumber(), loc.getSpellingColumnNumber()));
      */
        clang::FullSourceLoc loc = diag->getLocation().asLocation().getExpansionLoc();
        ResultCollector::instance().add_diagnostic(Diagnostic(
            diag->getCheckerName().str(), diag->getBugType().str(), loc.getManager().getFilename(loc).str(),
            diag->getShortDescription().str(), loc.getExpansionLineNumber(), loc.getExpansionColumnNumber()));
    }
}