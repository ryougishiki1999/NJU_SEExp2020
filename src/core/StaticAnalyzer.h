#ifndef CORE_STATIC_ANALYZER_H
#define CORE_STATIC_ANALYZER_H
#include "AnalyzerOptions.h"
#include "Event.h"
#include <clang/Tooling/CommonOptionsParser.h>
#include <string>
#include <vector>
/*! \brief Analyzer

    main class of Analyzer, used to set ClangTool's commandline, pass arguments to and call ClangTool correctly
*/
class StaticAnalyzer
{
  public:
    StaticAnalyzer() = default;
    StaticAnalyzer(const EventCallbackTable &ev_tb);

    /*! \brief correctly set ClangTool's commandline based on our options and source files.
        \param[in] srcs vector of source files
        \param[in] opts our options
        \return the CommonOptionsParser which has parsed the arguments
    */
    clang::tooling::CommonOptionsParser process_argv(const std::vector<std::string> &srcs, const AnalyzerOptions &opts);

    /*! \brief run ClangTool based on our options and source files
        \param[in] srcs vector of source files
        \param[in] opts our options
        \return ClangTool's status code
    */
    int analyze(const std::vector<std::string> &srcs, const AnalyzerOptions &opts);
    /*! \brief get available source files(those can be analyzed) based on our options and source files
        \param[in] srcs vector of source files(those we want to analyze)
        \param[in] opts our options
        \return available source files
    */
    std::vector<std::string> get_available_sources(const std::vector<std::string> &srcs, const AnalyzerOptions &opts);
    /*! \brief get ource files(those will be analyzed) based on our options and source files
        \param[in] srcs vector of source files(those we want to analyze)
        \param[in] opts our options
        \return source files
    */
    std::vector<std::string> get_sources(const std::vector<std::string> &srcs, const AnalyzerOptions &opts);

  private:
    static llvm::cl::OptionCategory opt_category;
    EventCallbackTable event_table;
};
#endif