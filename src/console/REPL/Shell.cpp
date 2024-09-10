#include "Shell.h"
#include <sstream>
#include <string>
void Shell::exec(std::istream &is)
{
    std::string cmd_line;
    std::vector<std::string> argv;
    std::string command;
    ShellContext shell_context;
    std::cout << "Welcome to HiSA REPL\nType 'quit' to stop.\n";
    while (1)
    {
        std::cout << "HiSA>>> ";
        std::getline(is, cmd_line);
        std::istringstream ss(cmd_line);
        ss >> command;
        while (!ss.eof())
        {
            std::string s;
            ss >> s;
            argv.push_back(s);
        }
        auto cmd_it = command_map.find(command);
        if (cmd_it != command_map.end())
        {
            if (cmd_it->second.second(shell_context, argv) != 0)
            {
                return;
            }
        }
        else
        {
            std::cout << "\tUnknown command. Type 'help' for command infomation.\n";
        }
        argv.clear();
    }
}