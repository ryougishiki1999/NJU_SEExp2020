#include "REPL/Shell.h"
#include "../core/tool/Instance.h"
#include "../core/Logger.h"
#include <iostream>
#include <sstream>
void analyzing_file_callback(std::unique_ptr<Event> ev)
{
    AnalyzingFileEvent *aev = static_cast<AnalyzingFileEvent *>(ev.get());
    std::cout << "Analyzing " << aev->filename << '\n';
}
int main(int argc, char *argv[])
{
    // init
    HiSAInstance::global_init();
#ifdef _DEBUG
    // set log level
    Logger::filter_log_level = Logger::Debug;
    // temp commandline
    std::string temp_cmd;
    std::getline(std::cin, temp_cmd);
    std::vector<std::string> temp_argv_string;
    std::istringstream temp_ss(temp_cmd);
    while (!temp_ss.eof())
    {
        std::string s;
        temp_ss >> s;
        temp_argv_string.push_back(s);
    }
    std::vector<char *> temp_argv;
    for (auto i = 0; i < temp_argv_string.size(); ++i)
    {
        // ATTENTION: THIS HAS MEMORY BUG. BUT FOR NOW IT'S SAFE.
        // WE WILL USE THE main's argc,argv instead in future.
        temp_argv.push_back((char *)temp_argv_string[i].c_str());
    }
    argc = temp_argv.size();
    argv = temp_argv.data();
#else
    // set log level
    Logger::filter_log_level = Logger::Info;
#endif
    if (argc == 1)
    {
        Shell shell;
        shell.exec(std::cin);
        return 0;
    }
    EventCallbackTable event_table;
    event_table.set_callback(Event::OnAnalyzingFile, analyzing_file_callback);

    HiSAInstance instance(event_table);

    INFO("Running...\n");
    if (instance.run(argc, argv) != 0)
    {
        WARNING("0ops, clang error detected. Some files may not be analyzed because of it.\n");
    }

    return 0;
}