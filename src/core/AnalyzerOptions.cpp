#include "AnalyzerOptions.h"

AnalyzerOptions::AnalyzerOptions()
{
    enable_god_mode = enable_stats = enable_debug_mode = show_analysis_progress = enable_exploded_graph =
        trim_exploded_graph = 0;
}
void AnalyzerOptions::dump(std::ostream &os)
{
    os << "\tGod Mode: " << (bool)enable_god_mode << '\n';
    os << "\tStats: " << (bool)enable_stats << '\n';
    os << "\tDebug Mode: " << (bool)enable_debug_mode << '\n';
    if (enable_debug_mode)
    {
        os << "\tShow Analysis Progress: " << (bool)show_analysis_progress << '\n';
        os << "\tExploded Graph: " << (bool)enable_exploded_graph << '\n';
        os << "\tTrim Exploded Graph: " << (bool)trim_exploded_graph << '\n';
        os << "\tDebug Output Path: " << debug_output_path << '\n';
    }
    os << "\tCompilation Database: " << compilation_database_path << '\n';
}
void AnalyzerOptions::reset()
{
    enable_god_mode = enable_stats = enable_debug_mode = show_analysis_progress = enable_exploded_graph =
        trim_exploded_graph  = 0;
    debug_output_path = "";
    compilation_database_path = "";
}