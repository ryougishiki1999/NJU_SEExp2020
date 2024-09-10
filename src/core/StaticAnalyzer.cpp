#include "StaticAnalyzer.h"
#include "CSADriver.h"
#include "Logger.h"
#include <clang/Frontend/FrontendActions.h>
#include <clang/StaticAnalyzer/Frontend/FrontendActions.h>
#include <clang/Tooling/Tooling.h>

llvm::cl::OptionCategory StaticAnalyzer::opt_category("hisa options");

StaticAnalyzer::StaticAnalyzer(const EventCallbackTable &ev_tb) : event_table(ev_tb)
{
}
clang::tooling::CommonOptionsParser StaticAnalyzer::process_argv(const std::vector<std::string> &srcs,
                                                                 const AnalyzerOptions &opts)
{
    std::vector<const char *> args;
    args.push_back("hisa");
    if (opts.compilation_database_path.length())
    {
        args.push_back("-p");
        args.push_back(opts.compilation_database_path.c_str());
    }
    // clang need at least 1 positional argument, let it happy
    if (srcs.size() == 0)
        args.push_back("placeholder");
    for (auto &src : srcs)
        args.push_back(src.c_str());
    if (opts.compilation_database_path.length() == 0)
        args.push_back("--");
    int argc = args.size();
    // clang init
    // llvm::cl::OptionCategory opt_category("hisa options");
    return clang::tooling::CommonOptionsParser(argc, args.data(), opt_category);
}

int StaticAnalyzer::analyze(const std::vector<std::string> &srcs, const AnalyzerOptions &opts)
{
    if (opts.compilation_database_path.length() == 0 && srcs.size() == 0)
    {
        ERROR("No input.\n");
        return 1;
    }
    clang::tooling::CommonOptionsParser opt_parser = process_argv(srcs, opts);

    clang::tooling::ClangTool tool(opt_parser.getCompilations(),
                                   srcs.size() == 0 ? opt_parser.getCompilations().getAllFiles() : srcs);
    // must clear args first, because it appends -fsyntax-only as default.
    tool.clearArgumentsAdjusters();
    tool.appendArgumentsAdjuster(clang::tooling::getClangStripOutputAdjuster());
    tool.appendArgumentsAdjuster(
        clang::tooling::getInsertArgumentAdjuster("--analyze", clang::tooling::ArgumentInsertPosition::BEGIN));

    // run
    std::unique_ptr<CSAFactory> factory = std::make_unique<CSAFactory>(opts, event_table);
    return tool.run(factory.get());
}

std::vector<std::string> StaticAnalyzer::get_available_sources(const std::vector<std::string> &srcs,
                                                               const AnalyzerOptions &opts)
{
    if (opts.compilation_database_path.length() == 0 && srcs.size() == 0)
    {
        ERROR("No input.\n");
        return std::vector<std::string>();
    }
    clang::tooling::CommonOptionsParser opt_parser = process_argv(srcs, opts);
    return opt_parser.getCompilations().getAllFiles();
}

std::vector<std::string> StaticAnalyzer::get_sources(const std::vector<std::string> &srcs, const AnalyzerOptions &opts)
{
    if (opts.compilation_database_path.length() == 0 && srcs.size() == 0)
    {
        ERROR("No input.\n");
        return std::vector<std::string>();
    }
    clang::tooling::CommonOptionsParser opt_parser = process_argv(srcs, opts);
    return srcs.size() == 0 ? opt_parser.getCompilations().getAllFiles() : srcs;
}