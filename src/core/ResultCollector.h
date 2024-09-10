#ifndef CORE_RESULT_COLLECTOR_H
#define CORE_RESULT_COLLECTOR_H

#include <map>
#include <string>
#include <vector>
/*! \brief Diagnostic
 */
class Diagnostic
{
  public:
    Diagnostic(const std::string &chker_name, const std::string &bt, const std::string &fn, const std::string &desc,
               unsigned int lno, unsigned cno);

  public:
    const std::string checker_name;   //! the name of checker which detects the diagnostic
    const std::string bug_type;       //! the bug type of diagnostic
    const std::string filename;       //! the name of file diagnostic is located in
    const std::string description;    //! diagnostic's description
    const unsigned int line_number;   //! the line number of file diagnostic is located in
    const unsigned int column_number; //! the column number of file diagnostic is located in
};
/*! \brief Stats
 */
class Stats
{
  public:
    Stats(const std::string &fn, double at);

  public:
    const std::string filename; //! the name of file stats belongs to
    const double analysis_time; //! total analysis time on this file
};
/*! \brief Singleton, used to collect all results from analyzer
 */
class ResultCollector
{
  public:
    ResultCollector(const ResultCollector &) = delete;
    ResultCollector &operator=(const ResultCollector &) = delete;
    static ResultCollector &instance();

    /*! \brief add new diagnostic
        \param[in] diag diagnostic
    */
    void add_diagnostic(Diagnostic &&diag);
    /*! \brief add new stats
        \param[in] stats stats
    */
    void add_stats(Stats &&stats);
    /*! \brief get all diagnostics
        \return vector of diagnostics
    */
    const std::vector<Diagnostic> &get_diagnostics()
    {
        return diagnostics;
    }
    /*! \brief get all stats
        \return vector of stats
    */
    const std::vector<Stats> &get_stats()
    {
        return stats;
    }
    /*! \brief clear all diagnostics
     */
    void clear_diagnostics();
    /*! \brief clear all stats
     */
    void clear_stats();

  private:
    ResultCollector() = default;
    ~ResultCollector() = default;

  private:
    std::vector<Diagnostic> diagnostics;
    std::vector<Stats> stats;
};
#endif