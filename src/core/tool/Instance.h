#ifndef TOOL_INSTANCE_H
#define TOOL_INSTANCE_H

#include "../AnalyzerOptions.h"
#include "../Event.h"
#include "../OutputOptions.h"
#include <iostream>
/*! \brief HiSAInstance, like ClangTool

    the helper class if you want to use HiSA as library
*/
class HiSAInstance
{
  public:
    /*! \brief initialize global data instance needs, must be called before running
     */
    static void global_init();

  public:
    HiSAInstance() = default;
    HiSAInstance(const EventCallbackTable &tb);

    /*! \brief run HiSA as in commandline
        \param[in] argc argc(include software's name i.e. first argv)
        \param[in] argv argv(include software's name i.e. first argv)
     */
    int run(int argc, char *argv[]);
    /*! \brief run HiSA based on options and source files
        \param[in] srcs source files(you want to analyze)
        \param[in] analyzer_opts analyzer options
        \param[in] output_opts output options
     */
    int run(const std::vector<std::string> &srcs, const AnalyzerOptions &analyzer_opts,
            const OutputOptions &output_opts);

    // query functions
    /*! \sa StaticAnalyzer::get_available_sources
     */
    std::vector<std::string> get_available_sources(const std::vector<std::string> &srcs, const AnalyzerOptions &opts);
    /*! \sa StaticAnalyzer::get_sources
     */
    std::vector<std::string> get_sources(const std::vector<std::string> &srcs, const AnalyzerOptions &opts);
    /*! \sa CSACheckerRegistry::dump_checkers
     */
    void dump_checkers(std::ostream &os);
    /*! \sa CSACheckerRegistry::reset_checker_state
     */
    void reset_checkers();
    /*! \brief change the checker's enabled state to 'enabled'
        \param[in] name checker's name
        \param[in] enabled new state
        \return return false if specified checker not exists, otherwise return true
     */
    bool change_checker_state(const std::string &name, bool enabled);

  private:
    EventCallbackTable event_table;
};
#endif