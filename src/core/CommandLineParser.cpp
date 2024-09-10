#include "CommandLineParser.h"
#include "CSADriver.h"
#include "Logger.h"
#include <iostream>

CommandLineParser::CommandLineParser(int c, char **v) : argc(c), argv(v), cur_arg_pos(0)
{
}
// This function is for callback to get their own args(if exist)
std::string CommandLineParser::get_next_arg()
{
    if (cur_arg_pos >= argc)
        return std::string();
    std::string arg = argv[cur_arg_pos];
    // if the arg is in command map, the callback shouldn't use it.
    if (command_map.find(arg) != command_map.end() || debug_command_map.find(arg) != debug_command_map.end() ||
        output_command_map.find(arg) != output_command_map.end())
        return std::string();
    cur_arg_pos += 1;
    return arg;
}

int CommandLineParser::parse()
{
    while (cur_arg_pos < argc)
    {
        std::string arg = argv[cur_arg_pos++];
        auto cmd = command_map.find(arg);
        if (cmd == command_map.end())
        {
            WARNING("Unknown command: %s, ignore it\n", arg.c_str());
        }
        else
        {
            int ret = cmd->second.second(*this);
            if (ret != 0)
                return ret;
        }
    }
    return 0;
}
int CommandLineParser::sub_parse(
    const std::unordered_map<std::string, std::pair<std::string, CommandCallbackType>> &cmd_map)
{
    while (cur_arg_pos < argc)
    {
        std::string arg = argv[cur_arg_pos++];
        auto cmd = cmd_map.find(arg);
        if (cmd != cmd_map.end())
        {
            int ret = cmd->second.second(*this);
            if (ret != 0)
                return ret;
        }
        else
        {
            --cur_arg_pos;
            return 0;
        }
    }
    return 0;
}

// callbacks
static int help_callback(CommandLineParser &parser)
{
    for (auto &i : parser.command_map)
    {
        std::cout << i.first << '\t' << i.second.first << '\n';
        if (i.first == "-debug")
        {
            for (auto &ii : parser.debug_command_map)
            {
                std::cout << ii.first << '\t' << ii.second.first << '\n';
            }
        }
        else if (i.first == "-output")
        {
            for (auto &ii : parser.output_command_map)
            {
                std::cout << ii.first << '\t' << ii.second.first << '\n';
            }
        }
    }
    return 1;
}
static int god_mode_callback(CommandLineParser &parser)
{
    parser.analyzer_options.enable_god_mode = 1;
    return 0;
}
static int debug_mode_callback(CommandLineParser &parser)
{
    parser.analyzer_options.enable_debug_mode = 1;
    return parser.sub_parse(parser.debug_command_map);
}

static int debug_output_path_callback(CommandLineParser &parser)
{
    std::string path = parser.get_next_arg();
    if (path.length() == 0)
    {
        ERROR("path is empty.\n");
        return -1;
    }
    parser.analyzer_options.debug_output_path = path;
    return 0;
}
static int debug_show_analysis_progress_callback(CommandLineParser &parser)
{
    parser.analyzer_options.show_analysis_progress = 1;
    return 0;
}
static int debug_enable_exploded_graph_callback(CommandLineParser &parser)
{
    parser.analyzer_options.enable_exploded_graph = 1;
    return 0;
}
static int debug_trim_exploded_graph_callback(CommandLineParser &parser)
{
    parser.analyzer_options.trim_exploded_graph = 1;
    return 0;
}
static int compilation_database_path_callback(CommandLineParser &parser)
{
    std::string p = parser.get_next_arg();
    if (p.length() == 0)
    {
        ERROR("path is empty.\n");
        return -1;
    }
    parser.analyzer_options.compilation_database_path = p;
    return 0;
}

static int input_file_callback(CommandLineParser &parser)
{
    while (1)
    {
        std::string filename = parser.get_next_arg();
        if (filename.length() == 0)
            break;
        parser.sources.push_back(filename);
    }
    if (parser.sources.size() == 0)
    {
        ERROR("Input filename is empty.\n");
        return -1;
    }
    return 0;
}
static int output_callback(CommandLineParser &parser)
{
    return parser.sub_parse(parser.output_command_map);
}
static int output_path_callback(CommandLineParser &parser)
{
    std::string p = parser.get_next_arg();
    if (p.length() == 0)
    {
        ERROR("path is empty.\n");
        return -1;
    }
    parser.output_options.output_path = p;
    return 0;
}
static int output_format_callback(CommandLineParser &parser)
{
    while (1)
    {
        std::string format = parser.get_next_arg();
        if (format.length() == 0)
            break;
        parser.output_options.add_output_format(format);
    }
    if (parser.output_options.output_formats.size() == 0)
    {
        ERROR("format is empty.\n");
        return -1;
    }
    return 0;
}
static int output_info_callback(CommandLineParser &parser)
{
    while (1)
    {
        std::string opt = parser.get_next_arg();
        if (opt.length() == 0)
            break;
        if (opt == "time")
        {
            parser.analyzer_options.enable_stats = 1;
            parser.output_options.output_time = 1;
        }
        else if (opt == "checker")
            parser.output_options.output_checker = 1;
        else if (opt == "desc")
            parser.output_options.output_description = 1;
        else if (opt == "stats")
            parser.output_options.output_stats = 1;
        else
        {
            ERROR("illegal argument %s\n", opt.c_str());
            return -1;
        }
    }
    return 0;
}
static int enable_checker_callback(CommandLineParser &parser)
{
    CSACheckerRegistry &csa_cr = CSACheckerRegistry::instance();
    // disable all checkers first
    for (auto &i : csa_cr.checkers)
    {
        i.second.enabled = false;
    }
    while (1)
    {
        std::string chker = parser.get_next_arg();
        if (chker.length() == 0)
            break;
        auto it = csa_cr.checkers.find(chker);
        if (it != csa_cr.checkers.end())
            it->second.enabled = true;
    }
    return 0;
}
static int checker_list_callback(CommandLineParser &parser)
{
    CSACheckerRegistry &csa_cr = CSACheckerRegistry::instance();
    for (auto &i : csa_cr.checkers)
    {
        std::cout << i.first << '\n';
    }
    return 1;
}

const std::unordered_map<std::string, std::pair<std::string, CommandLineParser::CommandCallbackType>>
    CommandLineParser::command_map = {
        {"-help", {"help info", help_callback}},
        {"-g", {"run static analyzer in the god mode", god_mode_callback}},
        {"-debug", {"run static analyzer in the debug mode", debug_mode_callback}},
        {"-input", {"set input file(s)", input_file_callback}},
        {"-compilation-database-path", {"the path of compile_commands.json", compilation_database_path_callback}},
        {"-output", {"output options", output_callback}},
        {"-enable-checker", {"enabled checkers", enable_checker_callback}},
        {"-checker-list", {"list available checkers", checker_list_callback}}};

const std::unordered_map<std::string, std::pair<std::string, CommandLineParser::CommandCallbackType>>
    CommandLineParser::debug_command_map = {
        {"--output-path",
         {"file path to which the debug ExplodedGraph output should be dumped", debug_output_path_callback}},
        {"--show-analysis-progress", {"show CSA progress", debug_show_analysis_progress_callback}},
        {"--exploded-graph", {"enable exploded graph", debug_enable_exploded_graph_callback}},
        {"--trim-exploded-graph",
         {"only dump bug path in exploded graph under the debug mode", debug_trim_exploded_graph_callback}}};

const std::unordered_map<std::string, std::pair<std::string, CommandLineParser::CommandCallbackType>>
    CommandLineParser::output_command_map = {
        {"--path", {"the path of output", output_path_callback}},
        {"--format", {"ouput format,now support text,console,json,html", output_format_callback}},
        {"--info", {"what info should output,now support time,checker,desc,stats", output_info_callback}}};