#include "ResultCollector.h"

/////////////////////////////////////
// Diagnostic
/////////////////////////////////////
Diagnostic::Diagnostic(const std::string &chker_name, const std::string &bt, const std::string &fn,
                       const std::string &desc, unsigned int lno, unsigned cno)
    : checker_name(chker_name), bug_type(bt), filename(fn), description(desc), line_number(lno), column_number(cno)
{
}

/////////////////////////////////////
// Stats
/////////////////////////////////////
Stats::Stats(const std::string &fn, double at) : filename(fn), analysis_time(at)
{
}
/////////////////////////////////////
// ResultCollector
/////////////////////////////////////
ResultCollector &ResultCollector::instance()
{
    static ResultCollector rc;
    return rc;
}

void ResultCollector::add_diagnostic(Diagnostic &&diag)
{
    diagnostics.push_back(std::move(diag));
}
void ResultCollector::add_stats(Stats &&s)
{
    stats.push_back(std::move(s));
}
void ResultCollector::clear_diagnostics()
{
    diagnostics.clear();
}
void ResultCollector::clear_stats()
{
    stats.clear();
}