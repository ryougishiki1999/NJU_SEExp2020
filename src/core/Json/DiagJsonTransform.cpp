#include "DiagJsonTransform.h"
#include "../AnalyzerOptions.h"
#include "../Statistics/StatisticsHelper.h"
namespace Json
{
static Value &parse(BugStats bug_stats, const OutputOptions& opts)
{
    auto &stat_obj = new_object();
    auto &bugs_array = new_array();
    stat_obj.append("num", new_value(bug_stats.get_num()));
    stat_obj.append("bugs", bugs_array);
    for (auto &bug : bug_stats.get_bug_desc_list())
    {
        auto &obj = new_object();
        bugs_array.append(obj);

        obj.append("file_name", new_value(bug._file_name));
        obj.append("line_no", new_value((int)bug._line_number));
        obj.append("col_no", new_value((int)bug._column_number));
       
        if (opts.output_checker)
        {
            obj.append("checker_name", new_value(bug._checker_name));
        }
        
        if (opts.output_description)
        {
            obj.append("description", new_value(bug._description));
        }
        
    }
    return stat_obj;
}

Value &parse(const OutputOptions &opts)
{
    auto &root = new_object();
    auto &diags = new_array();
    auto helper = StatsHelper::instance();

    root.append("diagnostics", diags);

    for (auto &bug_stats : helper->get_all_bug_stats_list())
    {
        diags.append(parse(bug_stats, opts));
    }

    if (opts.output_stats)
    {
        
        root.append("analysis_time", new_value(helper->get_all_analysis_time()));
        root.append("bug_num", new_value(helper->get_all_bug_num()));
        root.append("bug_type_num", new_value(helper->get_all_bug_type_num()));
        root.append("file_num", new_value(helper->get_all_file_num()));

        auto &bug_type_array = new_array();
        root.append("bug_type_list", bug_type_array);

        for (auto &type : helper->get_all_bug_type_list())
        {
            bug_type_array.append(new_value(type));
        }

        auto &file_name_array = new_array();
        root.append("file_name_list", file_name_array);

        for (auto &file_name : helper->get_filename_list())
        {
            file_name_array.append(new_value(file_name));
        }

        auto &file_stats_array = new_array();
        root.append("file_stats", file_stats_array);

        for (auto &file_stat : helper->get_file_stats_list())
        {
            auto &obj = new_object();
            file_stats_array.append(obj);
            obj.append("file_name", new_value(file_stat._filename));
            obj.append("analysis_time", new_value(file_stat.get_analysis_time()));
            obj.append("bug_num", new_value(file_stat.get_all_bug_num()));
            obj.append("bug_type_num", new_value(file_stat.get_bug_type_num()));
            auto& bug_type_array = new_array();
            obj.append("bug_type_list", bug_type_array);
            for (auto& bug_type : file_stat.get_bug_type_list()) {
                bug_type_array.append(new_value(bug_type));
            }
            auto &bug_stats_array = new_array();
            obj.append("bug_stats_list", bug_stats_array);

            for (auto &bug_stats : file_stat.get_bug_stats_list())
            {
            bug_stats_array.append(parse(bug_stats, opts));
            }
        }
    }
    return root;
}

std::string stringify(const OutputOptions &opts)
{
    auto &root = parse(opts);
    std::string s = stringify(root);
    root.release();
    return s;
}
} // namespace Json