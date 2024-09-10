#ifndef CORE_ANALYZER_OPTIONS_H
#define CORE_ANALYZER_OPTIONS_H

#include <cstdint>
#include <iostream>
#include <string>
/*! \brief Analyzer Options

    This class stores all the options user can customize to control the analyzer.
*/
class AnalyzerOptions
{
  public:
    AnalyzerOptions();
    /*! \brief dump the options to the output stream
        \param[in] os output stream
    */
    void dump(std::ostream &os);
    /*! \brief reset the options to default values.
     */
    void reset();

  public:
    std::uint32_t enable_god_mode : 1; //! use the CSA checkers
    std::uint32_t enable_stats : 1;    //! enable the timer in analyzer
    std::uint32_t enable_debug_mode : 1;//! enable the debug mode
    std::uint32_t show_analysis_progress : 1;//! print the analysis progress, only works under debug mode
    std::uint32_t enable_exploded_graph : 1;//! dump the exploded graph, only works under debug mode
    std::uint32_t trim_exploded_graph : 1;//! trim the exploded graph, only works under debug mode
    std::string debug_output_path = "";//! the path of exploded graph, only works under debug mode
    std::string compilation_database_path = "";//! the path of compilation database
};
#endif