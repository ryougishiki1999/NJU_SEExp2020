#ifndef REPL_SHELL_H
#define REPL_SHELL_H

#include "ShellContext.h"
#include <iostream>
#include <map>
/*! \brief a REPL environment of HiSA
 */
class Shell
{
  public:
    using CommandCallbackType = int (*)(ShellContext &, const std::vector<std::string> &);

  public:
    /*! \brief run shell
        \param[in] is shell will get commands from this input stream
    */
    void exec(std::istream &is);

  public:
    static const std::map<std::string, std::pair<std::string, CommandCallbackType>>
        command_map; //! shell's command look-up table
};

#endif