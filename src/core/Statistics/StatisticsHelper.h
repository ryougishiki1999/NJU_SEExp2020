/**
 *  @file StatisticsHelper.h
 *  @brief Declarations of StatsHelper and its Utils
 *  @author Zhou RuoHeng
 *  @email  171860634@smail.nju.edu.cn
 */

#ifndef CORE_STATISTICS_STATISTICSHELPER_H
#define CORE_STATISTICS_STATISTICSHELPER_H
#include "../ResultCollector.h"
#include <map>
#include <memory>
#include <mutex>
#include <set>
#include <vector>

//---------------------------------------------------------------------------
// Platform Declaration
//---------------------------------------------------------------------------

class StatsHelper;
class FileStats;
class BugStats;
class BugStatsDesc;

/* @brief StatsHelper
 *  This class counts the entire results of project as a singleton.
 */
class StatsHelper
{
    typedef std::shared_ptr<StatsHelper> StatsHelperPtr;

  public:
    /**
     * @brief  instance
     * @details get the instance of the singleton class
     * @return  return the instance of StatsHelper as a pointer
     */
    static StatsHelperPtr instance();

    StatsHelper(const StatsHelper &) = delete;
    StatsHelper &operator=(const StatsHelper &) = delete;
    ~StatsHelper() = default;

    /**
     * @brief collect and generate all analysis statistics
     * @details interface of the StatsHelper to collect results of statistics
     */
    void collect_generate(const std::vector<Diagnostic> &diagnostics, const std::vector<Stats> &stats);

    /**
     * @brief clear Statistics
     * @details clear all statistics whether it is generated or not
     */
    void clear();

    /**
     * @brief get the list of filename
     * @details counts the files which are analyzed by analyzer
     * @return set of filename consisting of std::string
     */
    const std::set<std::string> &get_filename_list() const
    {
        return _filename_list;
    }

    /**
     * @brief get the list of statistics of every single file
     * @details FileStats is the util class which aims to collect statistics of separate file
     * @return vector of FileStats
     */
    const std::vector<FileStats> &get_file_stats_list() const
    {
        return _file_stats_list;
    }

    /**
     * @brief get the list of bug types
     * @details counts all occurrences of bug types
     * @return et of bug types consisting of std::string
     */
    const std::set<std::string> &get_all_bug_type_list() const
    {
        return _all_bug_type_list;
    }
    /**
     * @brief get the list of BugStats
     * @details BugStats is the util class which aims to collect statistics of the specify bug type
     * @return vector of BugStats
     */
    const std::vector<BugStats> &get_all_bug_stats_list() const
    {
        return _all_bug_stats_list;
    }

    /**
     * @brief get num of bugs
     * @details counts the number of bugs detected
     * @return int, num of bugs
     */
    int get_all_bug_num() const
    {
        return _all_bug_num;
    }
    /**
     * @brief get num of bug types
     * @return int, num of bug types
     */
    int get_all_bug_type_num() const
    {
        return _all_bug_type_num;
    }

    /**
     * @brief get num of files analyzed
     * @return int, num of files
     */
    int get_all_file_num() const
    {
        return _all_file_num;
    }

    /**
     * @brief get total time of analysis
     * @return double, analysis time
     */
    double get_all_analysis_time() const
    {
        return _all_analysis_time;
    }

  private:
    StatsHelper()
    {
        _all_analysis_time = 0.0;
        _all_bug_num = 0;
        _all_bug_type_num = 0;
        _all_file_num = 0;
    }
    /**
     * @brief collect diags and stats from Result collector
     * @param diagnostics, vector of diagnostics
     * @param stats, vector of stats
     */
    void collect_statistic(const std::vector<Diagnostic> &diagnostics, const std::vector<Stats> &stats);
    /**
     * @brief collect stats
     * @param stats, vector of stats
     */
    void collect_stats(const std::vector<Stats> &stats);
    /**
     * @brief collect diags
     * @param diagnostics, vector of digas
     */
    void collect_diagnostic(const std::vector<Diagnostic> &diagnostics);

    /**
     * @brief generate total statistics
     * @details after collecting, generate total statistics with collecting information
     */
    void generate();
    /**
     * @brief generate list of FileStats
     * @details A part of `generate`, generate a vector of FileStats
     */
    void generate_file_stats_list();
    /**
     * @brief get correct FileStats with filename(std::string)
     * @param filename, the name of file consisting of std::string
     * @return FileStats
     */
    const FileStats &get_file_stats(const std::string &filename) const;
    /**
     * @brief FileStats generates its statistics
     * @details after constructing list of FileStats, FileStats generate its statistics
     */
    void all_file_stats_generated();
    /**
     * @brief generate total num of bugs detected
     */
    void generate_all_bug_num();
    /**
     * @brief generate total num of files analyzed
     */
    void generate_all_file_num();
    /**
     * @brief generate total num of bug types
     */
    void generate_all_bug_type_num();
    /**
     * @brief generate list of BugStats
     */
    void generate_all_bug_stats_list();
    /**
     * @brief generate total analysis time
     */
    void generate_all_analysis_time();

  private:
    static StatsHelperPtr _sh; ///< the instance of pointer of StatsHelper
    static std::mutex _m_mutex;

    int _all_bug_num;          ///< the num of bugs
    int _all_bug_type_num;     ///< the num of bug types
    int _all_file_num;         ///< the num of analysis files
    double _all_analysis_time; ///< the time of ananlysis

    std::set<std::string> _filename_list;      ///< the list of name of files analyzed
    std::vector<FileStats> _file_stats_list;   ///< the list of FileStats totally
    std::set<std::string> _all_bug_type_list;  ///< the list of bug types totally
    std::vector<BugStats> _all_bug_stats_list; ///< the list of BugStats totally

    // This container is set to collect,not to be provided for
    // outsider caller
    std::map<std::string, FileStats> _file_stats_map;   ///< map of filename and its corresponding FileStats
    std::map<std::string, BugStats> _all_bug_stats_map; ///< map of bugname and its corresponding BugStats!!
};

//===----------------------------------------------------------------------------
//                      class FileStats decl
//===----------------------------------------------------------------------------
/**
 * @brief FileStats, statistics in this FileStats
 * @details collect all related statistics information for this FileStats, FileStats differs from its filename
 */
class FileStats final
{
    friend StatsHelper;

  public:
    FileStats()
    {
        _analysis_time = 0.0;
        _all_bug_num = 0;
        _bug_type_num = 0;
    }
    FileStats(const std::string &filename) : _filename(filename)
    {
        _analysis_time = 0.0 ;
        _all_bug_num = 0;
        _bug_type_num = 0;
    }
    FileStats &operator=(const FileStats &) = default;
    ~FileStats() = default;

    /**
     * @brief clear Statistics of this FileStats
     * @details clear all statistics of the FileStats
     */
    void clear();

    /**
     * @brief count analysis time of this FileStats
     */
    double get_analysis_time() const
    {
        return _analysis_time;
    }
    /**
     * @brief count num of bugs of this FileStats
     */
    int get_all_bug_num() const
    {
        return _all_bug_num;
    }

    /**
     * @brief count num of bug types of this FileStats
     */
    int get_bug_type_num() const
    {
        return _bug_type_num;
    }

    /**
     * @brief get list of BugStats of this FileStats
     * @details count this FileStats and generate its Bug Diagnositc statistics
     * @return vector of BugStats
     */
    const std::vector<BugStats> &get_bug_stats_list() const
    {
        return _bug_stats_list;
    }

    /**
     * @brief get list of bug types consisting of std::string
     * @return vector of bug types(std::string)
     */
    const std::set<std::string> &get_bug_type_list() const
    {
        return _bug_type_list;
    }

  public:
    const std::string _filename; ///< the filename of this FileStats

  private:
    /**
     * @brief generate statistics of this FileStats
     * @details normally called by `StatsHelper.generate`
     */
    void generate();
    /**
     * @brief generate list of BugStats of this FileStats
     * @details normally called by `FileStats.generate`
     */
    void generate_bug_stats_list();
    /**
     * @brief get BugStats by bug type
     * @param bug_type, type of bug
     * @return BugStats
     */
    const BugStats &get_bug_stats(const std::string &bug_type) const;
    /**
     * @brief generate num of bugs in this FileStats
     */
    void generate_all_bug_num();
    /**
     * @brief generate num of bug types in this FileStats
     */
    void generate_bug_type_num();

    /**
     * @brief set analysis time of this FileStats
     * @details get analysis time from `StatsHelper`
     * @param analysis_time
     */
    void set_analysis_time(const double analysis_time)
    {
        this->_analysis_time = analysis_time;
    }

  private:
    int _all_bug_num;      ///< num of bugs in this FileStats
    int _bug_type_num;     ///< num of bug types in this FileStats
    double _analysis_time; ///< analysis time of this FileStats

    std::set<std::string> _bug_type_list; ///< list of bug types(std::string)

    std::vector<BugStats> _bug_stats_list; ///< list of BugStats in this FileStats

    // This container is set to collect,not to be supplyed to
    // caller
    std::map<std::string, BugStats> _bug_stats_map; ///< map of bug type and corresponding BugStats
};

//===----------------------------------------------------------------------------
//                      class BugStats decl
//===----------------------------------------------------------------------------
/**
 * @brief BugStats, statistics in this BugStats
 * @details collect all related statistics information for this BugStats, BugStats differs from its bug type
 */
class BugStats final
{
    friend StatsHelper;
    friend FileStats;

  public:
    BugStats()
    {
        _num = 0;
    }
    BugStats(const std::string &bugtype, const int num) : _bug_type(bugtype), _num(num){};
    BugStats &operator=(const BugStats &)
    {
        return *this;
    }
    ~BugStats() = default;

    /**
     * @brief clear Statistics
     * @details clear statistics in this BugStats whether it is generated or not
     */
    void clear();

    /**
     * @brief get num of bug in this BugStats
     * @return int, num of bugs
     */
    int get_num() const
    {
        return _num;
    }
    /**
     * @brief get list of BugStatsDesc
     * @details BugStatsDesc collect every detailed information, like analysis checker, location filename...
     * @return vector of BugStatsDesc
     */
    const std::vector<BugStatsDesc> &get_bug_desc_list() const
    {
        return _bug_desc_list;
    }

  public:
    const std::string _bug_type; ///< bug type of this BugStats, primary key of this BugStats

  private:
    /**
     * @brief increase num of bugs by one
     */
    void num_inc_by_one()
    {
        _num = _num + 1;
    }

    /**
     * @brief increase num of bug by param(inc)
     * @param inc(int)
     */
    void num_inc_by(int inc)
    {
        _num = _num + inc;
    }

    /**
     * @brief set list of BugStatsDesc of this BugStats
     * @detail extend original _bug_desc_list of this BugStats gradually
     * @param bug_desc_list(vector<BugStatsDesc>)
     */
    void set_bug_desc_list(const std::vector<BugStatsDesc> &bug_desc_list)
    {
        _bug_desc_list.assign(bug_desc_list.begin(), bug_desc_list.end());
    }

  private:
    int _num; ///< num of bugs in this BugStats

    std::vector<BugStatsDesc>
        _bug_desc_list; ///< list of BugStatsDesc, collecting detailed description of this BugStats
};

/**
 * @brief BugStatsDesc, description of the BugStats which the BugStatsDesc belongs to
 * @details This class collect detailed description of BugStats, like location filename, location line and so on
 */
class BugStatsDesc final
{
    friend StatsHelper;
    friend FileStats;
    friend BugStats;

  public:
    BugStatsDesc() = default;
    BugStatsDesc(const std::string &file_name, const std::string &checker_name, const std::string &description,
                 const unsigned int line_number, const unsigned int column_number)
        : _file_name(file_name), _checker_name(checker_name), _description(description), _line_number(line_number),
          _column_number(column_number)
    {
    }
    BugStatsDesc &operator=(const BugStatsDesc &)
    {
        return *this;
    }
    ~BugStatsDesc() = default;

    /**
     * @brief clear statistics in this Description part
     */
    void clear();

  public:
    const std::string _file_name;      ///< the name of file where bug locates
    const std::string _checker_name;   ///< the name of checker which detects this bug
    const std::string _description;    ///< the description of this bug
    const unsigned int _line_number;   ///< the line number of file where bug locates
    const unsigned int _column_number; ///< the column number of file where bug locates

  private:
};

#endif