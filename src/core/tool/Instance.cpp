#include "Instance.h"
#include "../CSADriver.h"
#include "../CommandLineParser.h"
#include "../OutputManager.h"
#include "../ResultCollector.h"
#include "../StaticAnalyzer.h"
#include "../Statistics/StatisticsHelper.h"
#include "../checker/DereferenceChecker.h"
#include "../checker/DivModZeroChecker.h"
#include "../checker/MemoryChecker.h"
#include "../checker/UninitObjChecker/UninitializedObjectChecker.h"
#include "../checker/ArrayBoundChecker.h"

void HiSAInstance::global_init()
{
    // register checkers
    CSACheckerRegistry::instance().register_checker("MemoryChecker", CSA_REGISTER_FUNCTION_NAME(MemoryChecker),
                                                    "memory leak,double-free,use-after-free");
    CSACheckerRegistry::instance().register_checker("DivZeroChecker", CSA_REGISTER_FUNCTION_NAME(DivModZeroChecker),
                                                    "div zero,mod zero,div taint value");
    CSACheckerRegistry::instance().register_checker("UninitObjChecker", CSA_REGISTER_FUNCTION_NAME(UninitObjChecker),
                                                    "non-union,primitive,reference uninitialize");
    CSACheckerRegistry::instance().register_checker(
        "DereferenceChecker", CSA_REGISTER_FUNCTION_NAME(DereferenceChecker), "null-pointer-dereference");
    CSACheckerRegistry::instance().register_checker("ArrayBoundChecker", CSA_REGISTER_FUNCTION_NAME(ArrayBoundChecker),
                                                    "array index out-of-bound");
}
HiSAInstance::HiSAInstance(const EventCallbackTable &tb) : event_table(tb)
{
}

int HiSAInstance::run(int argc, char *argv[])
{
    // parse commands
    CommandLineParser cmdline_parser(argc - 1, &argv[1]);
    int retval = cmdline_parser.parse();
    if (retval != 0)
        return retval < 0 ? -1 : 0;
    // analyze
    return run(cmdline_parser.sources, cmdline_parser.analyzer_options, cmdline_parser.output_options);
}

int HiSAInstance::run(const std::vector<std::string> &srcs, const AnalyzerOptions &analyzer_opts,
                      const OutputOptions &output_opts)
{
    // reset data
    ResultCollector::instance().clear_diagnostics();
    ResultCollector::instance().clear_stats();
    StatsHelper::instance()->clear();

    // analyze
    StaticAnalyzer sa(event_table);
    int retval = sa.analyze(srcs, analyzer_opts);

    // output
    StatsHelper::instance()->collect_generate(ResultCollector::instance().get_diagnostics(),
                                              ResultCollector::instance().get_stats());

    OutputManager::output(output_opts);
    return retval;
}

std::vector<std::string> HiSAInstance::get_available_sources(const std::vector<std::string> &srcs,
                                                             const AnalyzerOptions &opts)
{
    StaticAnalyzer sa(event_table);
    return sa.get_available_sources(srcs, opts);
}
std::vector<std::string> HiSAInstance::get_sources(const std::vector<std::string> &srcs, const AnalyzerOptions &opts)
{
    StaticAnalyzer sa(event_table);
    return sa.get_sources(srcs, opts);
}

void HiSAInstance::dump_checkers(std::ostream &os)
{
    CSACheckerRegistry::instance().dump_checkers(std::cout);
}
void HiSAInstance::reset_checkers()
{
    CSACheckerRegistry::instance().reset_checker_state();
}
bool HiSAInstance::change_checker_state(const std::string &name, bool enabled)
{
    CSACheckerRegistry &csa_cr = CSACheckerRegistry::instance();
    auto it = csa_cr.checkers.find(name);
    if (it != csa_cr.checkers.end())
    {
        it->second.enabled = enabled;
        return true;
    }
    else
    {
        return false;
    }
}