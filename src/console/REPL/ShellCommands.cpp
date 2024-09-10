#include "../../core/Logger.h"
#include "../../core/OutputManager.h"
#include "../../core/ResultCollector.h"
#include "../../core/Statistics/StatisticsHelper.h"
#include "Shell.h"
#include <iostream>

static int help_callback(ShellContext &ctx, const std::vector<std::string> &argv)
{
    for (auto &i : Shell::command_map)
    {
        std::cout << '\t' << i.first << '\t' << i.second.first << '\n';
    }
    return 0;
}
static int quit_callback(ShellContext &ctx, const std::vector<std::string> &argv)
{
    return 1;
}

static const std::vector<std::pair<std::string, std::string>> info_commands = {
    {"help", "print help info"},
    {"analyzer-opt", "print analyzer settings"},
    {"output-opt", "print output settings"},
    {"checker", "print checker settings"},
    {"src", "print source files that will be analyzed"},
    {"available-src", "print source files that can be analyzed"}};
static int info_callback(ShellContext &ctx, const std::vector<std::string> &argv)
{
    if (argv.size() < 1)
    {
        std::cout << "\tNo command. Type 'info help' for command infomation.\n";
        return 0;
    }
    if (argv[0] == "help")
    {
        for (const auto &i : info_commands)
        {
            std::cout << '\t' << i.first << '\t' << i.second << '\n';
        }
    }
    else if (argv[0] == "analyzer-opt")
    {
        ctx.analyzer_options.dump(std::cout);
    }
    else if (argv[0] == "output-opt")
    {
        ctx.output_options.dump(std::cout);
    }
    else if (argv[0] == "checker")
    {
        ctx.instance.dump_checkers(std::cout);
    }
    else if (argv[0] == "src")
    {
        auto srcs = ctx.instance.get_sources(ctx.sources, ctx.analyzer_options);
        for (const auto &i : srcs)
        {
            std::cout << '\t' << i << '\n';
        }
    }
    else if (argv[0] == "available-src")
    {
        auto srcs = ctx.instance.get_available_sources(ctx.sources, ctx.analyzer_options);
        for (const auto &i : srcs)
        {
            std::cout << '\t' << i << '\n';
        }
    }
    else
    {
        std::cout << "\tUnknown command. Type 'info help' for command infomation.\n";
    }
    return 0;
}
static const std::vector<std::pair<std::string, std::string>> set_commands = {
    {"help", "print help info"},
    {"debug-output-path", "set debug output path"},
    {"compilation-database", "set compilation database path"},
    {"output-format", "set output formats"},
    {"output-path", "set output path"}};
static int set_callback(ShellContext &ctx, const std::vector<std::string> &argv)
{
    if (argv.size() < 1)
    {
        std::cout << "\tNo command. Type 'set help' for command infomation.\n";
        return 0;
    }
    if (argv[0] == "help")
    {
        for (const auto &i : set_commands)
        {
            std::cout << '\t' << i.first << '\t' << i.second << '\n';
        }
    }
    else if (argv[0] == "debug-output-path")
    {
        if (argv.size() < 2)
        {
            std::cout << "\tEmpty path.\n";
            return 0;
        }
        ctx.analyzer_options.debug_output_path = argv[1];
    }
    else if (argv[0] == "compilation-database")
    {
        if (argv.size() < 2)
        {
            std::cout << "\tEmpty path.\n";
            return 0;
        }
        ctx.analyzer_options.compilation_database_path = argv[1];
    }
    else if (argv[0] == "output-format")
    {
        if (argv.size() < 2)
        {
            std::cout << "\tEmpty formats.\n";
            return 0;
        }
        for (int i = 1; i < argv.size(); ++i)
        {
            ctx.output_options.add_output_format(argv[i]);
        }
    }
    else if (argv[0] == "output-path")
    {
        if (argv.size() < 2)
        {
            std::cout << "\tEmpty path.\n";
            return 0;
        }
        ctx.output_options.output_path = argv[1];
    }
    else
    {
        std::cout << "\tUnknown command. Type 'set help' for command infomation.\n";
    }
    return 0;
}
static const std::vector<std::pair<std::string, std::string>> enable_disable_commands = {
    {"help", "print help info"},
    {"gmode", "enable/disable god mode"},
    {"analyzer-stats", "enable/disable analyzer stats"},
    {"debug", "enable/disable debug mode"},
    {"debug-analysis-progress", "enable/disable analysis progress"},
    {"debug-exploded-graph", "enable/disable exploded graph"},
    {"debug-trim-exploded-graph", "enable/disable trimed exploded graph"},
    {"output-time", "enable/disable time output"},
    {"output-checker", "enable/disable checker output"},
    {"output-description", "enable/disable description output"},
    {"output-stats", "enable/disable stats output"}};
static int enable_callback(ShellContext &ctx, const std::vector<std::string> &argv)
{
    if (argv.size() < 1)
    {
        std::cout << "\tNo command. Type 'enable help' for command infomation.\n";
        return 0;
    }
    if (argv[0] == "help")
    {
        for (const auto &i : enable_disable_commands)
        {
            std::cout << '\t' << i.first << '\t' << i.second << '\n';
        }
    }
    else if (argv[0] == "gmode")
    {
        ctx.analyzer_options.enable_god_mode = 1;
    }
    else if (argv[0] == "analyzer-stats")
    {
        ctx.analyzer_options.enable_stats = 1;
    }
    else if (argv[0] == "debug")
    {
        ctx.analyzer_options.enable_debug_mode = 1;
    }
    else if (argv[0] == "debug-analysis-progress")
    {
        ctx.analyzer_options.show_analysis_progress = 1;
        ctx.analyzer_options.enable_debug_mode = 1;
    }
    else if (argv[0] == "debug-exploded-graph")
    {
        ctx.analyzer_options.enable_exploded_graph = 1;
        ctx.analyzer_options.enable_debug_mode = 1;
    }
    else if (argv[0] == "debug-trim-exploded-graph")
    {
        ctx.analyzer_options.trim_exploded_graph = 1;
        ctx.analyzer_options.enable_exploded_graph = 1;
        ctx.analyzer_options.enable_debug_mode = 1;
    }
    else if (argv[0] == "output-time")
    {
        ctx.output_options.output_time = 1;
    }
    else if (argv[0] == "output-checker")
    {
        ctx.output_options.output_checker = 1;
    }
    else if (argv[0] == "output-description")
    {
        ctx.output_options.output_description = 1;
    }
    else if (argv[0] == "output-stats")
    {
        ctx.output_options.output_stats = 1;
    }
    else
    {
        if (!ctx.instance.change_checker_state(argv[0], true))
        {
            std::cout << "\tUnknown command. Type 'enable help' for command infomation.\n";
        }
    }
    return 0;
}
static int disable_callback(ShellContext &ctx, const std::vector<std::string> &argv)
{
    if (argv.size() < 1)
    {
        std::cout << "\tNo command. Type 'disable help' for command infomation.\n";
        return 0;
    }
    if (argv[0] == "help")
    {
        for (const auto &i : enable_disable_commands)
        {
            std::cout << '\t' << i.first << '\t' << i.second << '\n';
        }
    }
    else if (argv[0] == "gmode")
    {
        ctx.analyzer_options.enable_god_mode = 0;
    }
    else if (argv[0] == "analyzer-stats")
    {
        ctx.analyzer_options.enable_stats = 0;
    }
    else if (argv[0] == "debug")
    {
        ctx.analyzer_options.enable_debug_mode = 0;
    }
    else if (argv[0] == "debug-analysis-progress")
    {
        ctx.analyzer_options.show_analysis_progress = 0;
    }
    else if (argv[0] == "debug-exploded-graph")
    {
        ctx.analyzer_options.enable_exploded_graph = 0;
    }
    else if (argv[0] == "debug-trim-exploded-graph")
    {
        ctx.analyzer_options.trim_exploded_graph = 0;
    }
    else if (argv[0] == "output-time")
    {
        ctx.output_options.output_time = 0;
    }
    else if (argv[0] == "output-checker")
    {
        ctx.output_options.output_checker = 0;
    }
    else if (argv[0] == "output-description")
    {
        ctx.output_options.output_description = 0;
    }
    else if (argv[0] == "output-stats")
    {
        ctx.output_options.output_stats = 0;
    }
    else
    {
        if (!ctx.instance.change_checker_state(argv[0], false))
        {
            std::cout << "\tUnknown command. Type 'disable help' for command infomation.\n";
        }
    }
    return 0;
}
static int analyze_callback(ShellContext &ctx, const std::vector<std::string> &argv)
{
    if (argv.size() == 0)
    {
        ctx.instance.run(ctx.sources, ctx.analyzer_options, ctx.output_options);
    }
    else if (argv.size() == 1)
    {
        std::vector<std::string> src;
        src.push_back(argv[0]);
        ctx.instance.run(src, ctx.analyzer_options, ctx.output_options);
    }
    else
    {
        std::cout << "\tToo many input files.\n";
        return 0;
    }
    return 0;
}
static const std::vector<std::pair<std::string, std::string>> reset_commands = {
    {"help", "print help info"},
    {"analyzer-opt", "reset analyzer settings"},
    {"output-opt", "reset output settings"},
    {"checker", "reset checkers"},
    {"src", "clear source list"}};
static int reset_callback(ShellContext &ctx, const std::vector<std::string> &argv)
{
    if (argv.size() < 1)
    {
        std::cout << "\tNo command. Type 'reset help' for command infomation.\n";
        return 0;
    }
    if (argv[0] == "help")
    {
        for (const auto &i : reset_commands)
        {
            std::cout << '\t' << i.first << '\t' << i.second << '\n';
        }
    }
    else if (argv[0] == "analyzer-opt")
    {
        ctx.analyzer_options.reset();
    }
    else if (argv[0] == "output-opt")
    {
        ctx.output_options.reset();
    }
    else if (argv[0] == "checker")
    {
        ctx.instance.reset_checkers();
    }
    else if (argv[0] == "src")
    {
        ctx.sources.clear();
    }
    else
    {
        std::cout << "\tUnknown command. Type 'reset help' for command infomation.\n";
    }
    return 0;
}
static int addsrc_callback(ShellContext &ctx, const std::vector<std::string> &argv)
{
    if (argv.size() < 1)
    {
        std::cout << "\tNo input.\n";
        return 0;
    }
    ctx.sources.push_back(argv[0]);
    return 0;
}
static int delsrc_callback(ShellContext &ctx, const std::vector<std::string> &argv)
{
    if (argv.size() < 1)
    {
        std::cout << "\tNo input.\n";
        return 0;
    }
    for (auto it = ctx.sources.begin(); it != ctx.sources.end(); ++it)
    {
        if (*it == argv[0])
        {
            ctx.sources.erase(it);
            break;
        }
    }
    return 0;
}
static const std::vector<std::pair<std::string, std::string>> show_commands = {
    {"help", "print help info"}, {"summary", "print analysis summary"}, {"all", "print all files' analysis result"}};

static void print_file_result(std::ostream &os, ShellContext &ctx, const FileStats &file_stats)
{
    static std::string delim("---------------------------------------");
    os << '\n' << file_stats._filename << ": " << file_stats.get_all_bug_num() << " bug(s)";
    if (ctx.analyzer_options.enable_stats)
    {
        os << " in " << file_stats.get_analysis_time() << " (s).\n";
    }
    else
        os << ".\n";
    os << delim << '\n';
    for (auto &i : file_stats.get_bug_stats_list())
    {
        for (auto &j : i.get_bug_desc_list())
        {
            os << "(" << j._checker_name << ") ";
            os << i._bug_type << " at (" << j._line_number << ':' << j._column_number << ")";
            os << ' ' << j._description;
            os << '\n';
        }
    }
}

static void print_summary(std::ostream &os, ShellContext &ctx)
{
    auto stats_helper = StatsHelper::instance();
    static std::string delim("---------------------------------------");
    os << '\n' << delim << "\nStats\n" << delim << '\n';
    os << "Analyzed " << stats_helper->get_all_file_num() << " file(s).\n";
    os << "Found " << stats_helper->get_all_bug_num() << " bug(s).\n";
    for (auto &i : stats_helper->get_all_bug_stats_list())
    {
        os << i.get_num() << ' ' << i._bug_type << "\n";
    }
    if (ctx.analyzer_options.enable_stats)
    {
        os << "Total time: " << stats_helper->get_all_analysis_time() << "(s)\n";
    }
    os << delim << '\n';
    for (auto &i : stats_helper->get_file_stats_list())
    {
        os << i._filename << ": " << i.get_all_bug_num() << " bug(s)";
        if (ctx.analyzer_options.enable_stats)
        {
            os << " in " << i.get_analysis_time() << " (s).\n";
        }
        else
            os << ".\n";
        for (auto &j : i.get_bug_stats_list())
        {
            os << '\t' << j.get_num() << ' ' << j._bug_type << "(s)\n";
        }
        os << '\n';
    }
}
static int show_callback(ShellContext &ctx, const std::vector<std::string> &argv)
{
    auto stats_helper = StatsHelper::instance();
    if (argv.size() < 1)
    {
        std::cout << "\tNo command. Type 'show help' for command infomation.\n";
        return 0;
    }
    if (argv[0] == "help")
    {
        for (const auto &i : show_commands)
        {
            std::cout << '\t' << i.first << '\t' << i.second << '\n';
        }
    }
    else if (argv[0] == "summary")
    {
        print_summary(std::cout, ctx);
    }
    else if (argv[0] == "all")
    {
        for (auto &i : stats_helper->get_file_stats_list())
        {
            print_file_result(std::cout, ctx, i);
        }
        print_summary(std::cout, ctx);
    }
    else
    {
        for (auto &i : stats_helper->get_file_stats_list())
        {
            if (i._filename == argv[0])
            {
                print_file_result(std::cout, ctx, i);
                return 0;
            }
        }
        std::cout << "\tUnknown command. Type 'show help' for command infomation.\n";
    }
    return 0;
}

const std::map<std::string, std::pair<std::string, Shell::CommandCallbackType>> Shell::command_map = {
    {"help", {"print help info", help_callback}},
    {"quit", {"quit REPL", quit_callback}},
    {"info", {"get current state infomation", info_callback}},
    {"set", {"set current state", set_callback}},
    {"enable", {"set current state", enable_callback}},
    {"disable", {"set current state", disable_callback}},
    {"analyze", {"run analysis", analyze_callback}},
    {"reset", {"reset state", reset_callback}},
    {"addsrc", {"add source file to analyze", addsrc_callback}},
    {"delsrc", {"delete source file from source list", delsrc_callback}},
    {"show", {"show the analysis result", show_callback}}};