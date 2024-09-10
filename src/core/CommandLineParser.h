#ifndef CORE_COMMAND_LINE_PARSER_H
#define CORE_COMMAND_LINE_PARSER_H

#include "AnalyzerOptions.h"
#include "OutputOptions.h"
#include <string>
#include <unordered_map>
#include <vector>

/*! \brief CommandLineParser

    used to parse commandline to options, source file list.
*/
class CommandLineParser
{
  public:
    using CommandCallbackType = int (*)(CommandLineParser &);

  public:
    /*! \brief construct CommandLineParser based on argc and argv
        \param[in] c argc(not include software's name i.e. first argv)
        \param[in] v argv(not include software's name i.e. first argv)
    */
    CommandLineParser(int c, char **v);

  public:
    /*! \brief get next arg
        \return next arg, empty string if next arg is a command
    */
    std::string get_next_arg();
    /*! \brief parse argv
        \return if error, return -1, if no error but should terminate, return 1, otherwise return 0
    */
    int parse();
    /*! \brief parse argv
        \param[in] cmd_map besides the main commands, commands in cmd_map will also be parsed
        \return if error, return -1, if no error but should terminate, return 1, otherwise return 0
    */
    int sub_parse(const std::unordered_map<std::string, std::pair<std::string, CommandCallbackType>> &cmd_map);

  public:
    static const std::unordered_map<std::string, std::pair<std::string, CommandCallbackType>>
        command_map; //! main commands
    static const std::unordered_map<std::string, std::pair<std::string, CommandCallbackType>>
        debug_command_map; //! sub commands available under debug mode
    static const std::unordered_map<std::string, std::pair<std::string, CommandCallbackType>>
        output_command_map; //! sub commands available in output

  private:
    int argc;
    char **argv;
    int cur_arg_pos;

  public:
    AnalyzerOptions analyzer_options; //! result analyzer options
    OutputOptions output_options;     //! result output options
    std::vector<std::string> sources; //! result source file list
};
#endif