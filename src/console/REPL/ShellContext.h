#ifndef REPL_SHELL_CONTEXT_H
#define REPL_SHELL_CONTEXT_H

#include "../../core/tool/Instance.h"
/*! \brief the context of shell
*/
class ShellContext
{
  public:
    AnalyzerOptions analyzer_options;//! analyzer options
    OutputOptions output_options;//! output options
    std::vector<std::string> sources;//! source files we want to analyze
    HiSAInstance instance;//! HiSA instance
};
#endif