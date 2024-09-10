#include "StatisticsHelper.h"

StatsHelper::StatsHelperPtr StatsHelper::_sh = nullptr;

std::mutex StatsHelper::_m_mutex;

////////////////////////////////////////////////////////////////////////////////
//                      class StatsHelper method def
////////////////////////////////////////////////////////////////////////////////

StatsHelper::StatsHelperPtr StatsHelper::instance()
{
    if (_sh == nullptr)
    {
        std::lock_guard<std::mutex> lock(_m_mutex);
        if (_sh == nullptr)
        {
            _sh = StatsHelperPtr(new StatsHelper);
        }
    }
    return _sh;
}

void StatsHelper::collect_generate(const std::vector<Diagnostic> &diagnostics, const std::vector<Stats> &stats)
{
    collect_statistic(diagnostics, stats);
    generate();
}

void StatsHelper::clear()
{
    
    _filename_list.clear();
    _file_stats_list.clear();
    _all_bug_type_list.clear();
    _all_bug_stats_list.clear();
    _file_stats_map.clear();
    _all_bug_stats_map.clear();
    
    _all_bug_num = NULL;
    _all_bug_type_num = NULL;
    _all_file_num = NULL;
    _all_analysis_time = NULL;

}

void StatsHelper::collect_statistic(const std::vector<Diagnostic> &diagnostics, const std::vector<Stats> &stats)
{
    collect_stats(stats);
    collect_diagnostic(diagnostics);
}

void StatsHelper::collect_stats(const std::vector<Stats> &stats)
{
    for (const auto &s : stats)
    {
        const std::string filename = s.filename;
        const double analysis_time = s.analysis_time;

        if (_file_stats_map.find(filename) == _file_stats_map.end())
        {

            _filename_list.insert(filename);

            FileStats file_stats(filename);
            file_stats.set_analysis_time(analysis_time);

            _file_stats_map.insert(std::pair<std::string, FileStats>(filename, file_stats));
        }
        else
        {
            ;
        }
    }
}

void StatsHelper::collect_diagnostic(const std::vector<Diagnostic> &diagnostics)
{
    for (const auto &d : diagnostics)
    {
        const std::string filename = d.filename;
        const std::string bug_type = d.bug_type;
        const std::string checker_name = d.checker_name;
        const std::string description = d.description;
        const unsigned int line_number = d.line_number;
        const unsigned int column_number = d.column_number;

        if (_file_stats_map.find(filename) == _file_stats_map.end())
        {
            // _file_stats_map does not contains current file...

            _filename_list.insert(filename);
            // create new file_stats to insert in _file_stats_map...
            FileStats file_stats(filename);

            if (file_stats._bug_stats_map.find(bug_type) == file_stats._bug_stats_map.end())
            {
                // for file_stats is newly born, its _bug_stats_map should be empty...

                _all_bug_type_list.insert(bug_type);

                file_stats._bug_type_list.insert(bug_type);
                BugStats bug_stats(bug_type, 1);

                // add bug detailed description including checkername, description and location....
                BugStatsDesc bug_desc(filename, checker_name, description, line_number, column_number);
                bug_stats._bug_desc_list.push_back(bug_desc);

                file_stats._bug_stats_map.insert(std::pair<std::string, BugStats>(bug_type, bug_stats));
            }
            else
            {
                // This should not be reachable...

            }

            _file_stats_map.insert(std::pair<std::string, FileStats>(filename, file_stats));
        }
        else
        {
            // _file_stats_map contains current file

            if (_file_stats_map[filename]._bug_stats_map.find(bug_type) ==
                _file_stats_map[filename]._bug_stats_map.end())
            {
                //_bug_stats_map of the existing file_stats does not contain current
                // bug_type, create new bug for its _bug_stats_map...

                _all_bug_type_list.insert(bug_type);

                _file_stats_map[filename]._bug_type_list.insert(bug_type);
                BugStats bug_stats(bug_type, 1);

                // add bug detailed description including checkername, description and location....
                BugStatsDesc bug_desc(filename, checker_name, description, line_number, column_number);
                bug_stats._bug_desc_list.push_back(bug_desc);

                _file_stats_map[filename]._bug_stats_map.insert(std::pair<std::string, BugStats>(bug_type, bug_stats));
            }
            else
            {
                // corresponding _bug_stats_map has current bug_type, Since increase the
                // num of bug by one...

                _file_stats_map[filename]._bug_stats_map[bug_type].num_inc_by_one();

                // add bug detailed description to exsiting bug_stats....
                BugStatsDesc bug_desc(filename, checker_name, description, line_number, column_number);
                _file_stats_map[filename]._bug_stats_map[bug_type]._bug_desc_list.push_back(bug_desc);
            }
        }
    }
}

void StatsHelper::generate()
{
    generate_file_stats_list();
    all_file_stats_generated();
    generate_all_bug_num();
    generate_all_file_num();
    generate_all_bug_type_num();
    generate_all_bug_stats_list();
    generate_all_analysis_time();
}

void StatsHelper::generate_file_stats_list()
{
    for (auto &filename : _filename_list)
    {
        _file_stats_list.push_back(get_file_stats(filename));
    }
}

const FileStats &StatsHelper::get_file_stats(const std::string &filename) const
{
    if (_file_stats_map.find(filename) == _file_stats_map.end())
    {
        ;
    }
    else
    {
        return _file_stats_map.at(filename);
    }
}

void StatsHelper::all_file_stats_generated()
{
    for (auto &file_stats : _file_stats_list)
    {
        file_stats.generate();
    }
}

void StatsHelper::generate_all_bug_num()
{
    _all_bug_num = 0;
    for (auto &file_stats : _file_stats_list)
    {
        _all_bug_num += file_stats.get_all_bug_num();
    }
}

void StatsHelper::generate_all_file_num()
{
    _all_file_num = _filename_list.size();
}

void StatsHelper::generate_all_bug_type_num()
{
    _all_bug_type_num = _all_bug_type_list.size();
}

void StatsHelper::generate_all_bug_stats_list()
{
    for (const auto &file_stats : _file_stats_list)
    {
        for (const auto &bug_stats : file_stats._bug_stats_list)
        {
            const std::string bug_type = bug_stats._bug_type;
            const int bug_num = bug_stats._num;
            const std::vector<BugStatsDesc> bug_desc_list = bug_stats._bug_desc_list;

            if (_all_bug_stats_map.find(bug_type) == _all_bug_stats_map.end())
            {
                BugStats new_bug_stats(bug_type, bug_num);
                new_bug_stats.set_bug_desc_list(bug_desc_list);

                _all_bug_stats_map.insert(std::pair<std::string, BugStats>(bug_type, new_bug_stats));
            }
            else
            {
                _all_bug_stats_map[bug_type].num_inc_by(bug_num);
                _all_bug_stats_map[bug_type]._bug_desc_list.insert(_all_bug_stats_map[bug_type]._bug_desc_list.end(),
                                                                   bug_desc_list.begin(), bug_desc_list.end());
            }
        }
    }

    std::map<std::string, BugStats>::iterator iter = _all_bug_stats_map.begin();

    while (iter != _all_bug_stats_map.end())
    {
        _all_bug_stats_list.push_back(iter->second);
        iter++;
    }
}

void StatsHelper::generate_all_analysis_time()
{
    _all_analysis_time = 0;
    for (const auto &file_stats : _file_stats_list)
    {
        _all_analysis_time += file_stats.get_analysis_time();
    }
}

////////////////////////////////////////////////////////////////////////////////
//                      class FileStats method def
////////////////////////////////////////////////////////////////////////////////

void FileStats::clear()
{
    _bug_type_list.clear();
    _bug_stats_list.clear();
    _bug_stats_map.clear();

}

void FileStats::generate()
{
    generate_bug_stats_list();
    generate_all_bug_num();
    generate_bug_type_num();
}

void FileStats::generate_bug_stats_list()
{
    for (auto &bug_type : _bug_type_list)
    {
        _bug_stats_list.push_back(get_bug_stats(bug_type));
    }
}

const BugStats &FileStats::get_bug_stats(const std::string &bug_type) const
{
    if (_bug_stats_map.find(bug_type) == _bug_stats_map.end())
    {
       ;
    }
    else
    {
        return _bug_stats_map.at(bug_type);
    }
}

void FileStats::generate_all_bug_num()
{
    _all_bug_num = 0;
    for (const auto &bug_stats : _bug_stats_map)
    {
        _all_bug_num += bug_stats.second._num;
    }
}

void FileStats::generate_bug_type_num()
{
    _bug_type_num = _bug_type_list.size();
}

////////////////////////////////////////////////////////////////////////////////
//                      class BugStats method def
////////////////////////////////////////////////////////////////////////////////

void BugStats::clear()
{

    _bug_desc_list.clear();

}

////////////////////////////////////////////////////////////////////////////////
//                      class BugStatsDesc method def
////////////////////////////////////////////////////////////////////////////////

void BugStatsDesc::clear()
{
}