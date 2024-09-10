#include "OutputManager.h"
#include "HtmlPrinter.h"
#include "ResultCollector.h"
#include "Statistics/StatisticsHelper.h"
#include "Json/DiagJsonTransform.h"
#include "Statistics/StatisticsHelper.h"
#include <fstream>

static void append_line(std::string &dst_str, std::vector<std::string> &strs, const std::vector<int> &widths);

class Table
{
private:
    std::vector<std::string> table_heads;
  std::vector<std::vector<std::string>> table_data;

  public:
  Table()
  {
  }

  Table(const std::vector<std::string> &heads) : table_heads(heads)
    {
    }

  Table(std::vector<std::string> &&heads) : table_heads(std::move(heads))
  {
  }

  void reset(const std::vector<std::string> &heads)
  {
      table_heads.clear();
      table_heads = heads;
      table_data.clear();
  }

  void reset(std::vector<std::string> &&heads)
  {
      table_heads = std::move(heads);
      table_data.clear();
  }

  void append_row(const std::vector<std::string> &row)
  {
      table_data.push_back(row);
  }

  void append_row(std::vector<std::string> &&row)
  {
      table_data.push_back(std::move(row));
  }

  std::string to_md()
  {
      std::string str;
      for (auto &head : table_heads)
      {
          str += (head + " | ");
      }
      str += "\n";

      for (int i = 0; i < table_heads.size(); i++)
      {
          str += "--- | ";
      }
      str += "\n";

      for (auto &row : table_data)
      {
          for (auto &dat : row)
          {
              str += (dat + " | ");
          }
          str += "\n";
      }
      str += "\n";
      return str;
  }

  std::string to_text()
  {
      const int offset = 5;
      std::vector<int> widths;
      widths.resize(table_heads.size(), 15);
      for (int i = 0; i < widths.size(); i++)
      {
          if (table_heads[i].length() + offset > widths[i])
          {
              widths[i] = table_heads[i].length() + offset;
          }

          for (auto &row : table_data)
          {
              if (row[i].length() + offset > widths[i])
              {
                  widths[i] = row[i].length() + offset;
              }
          }
      }

      return to_text(widths);
  }

  std::string to_text(const std::vector<int>& widths)
  {
      std::string str;
      append_line(str, table_heads, widths);
      str += "\n";
      for (auto &row : table_data)
      {
          append_line(str, row, widths);
      }
      str += '\n';
      return str;
  }
};

void OutputManager::output(const OutputOptions &opts)
{
    std::string filepath = opts.output_path;
    for (auto &mode : opts.output_formats)
    {
        if (mode == "json")
        {
            output_json(filepath, opts);
        }
        else if (mode == "html")
        {
            output_html(filepath, opts);
        }
        else if (mode == "md")
        {
            output_md(filepath, opts);
        }
        else if (mode == "text")
        {
            output_text(filepath, opts);
        }
        else if (mode == "console")
        {
            output_console(opts);
        }
    }
}
void OutputManager::output_json(const std::string &filepath, const OutputOptions &opts)
{
    JsonUseCase();
    std::fstream stream(filepath.back() == '/' ? filepath + "report.json" : filepath + "/report.json", std::ios::out);
    std::string json_text = Json::stringify(opts);

    stream << json_text;
    stream.close();
}

void OutputManager::output_html(const std::string &filepath, const OutputOptions &opts)
{
    HtmlPrinter printer(filepath, opts.output_time, true, true, opts.output_checker, opts.output_description,
                        opts.output_stats);
    printer.print();
}

void OutputManager::output_md(const std::string &filepath, const OutputOptions &opts)
{
    std::fstream stream(filepath.back() == '/' ? filepath + "report.md" : filepath + "/report.md", std::ios::out);
    auto helper = StatsHelper::instance();
    std::vector<std::string> head;
    std::vector<std::string> row;
    Table table;
    
    head.clear();
    head.push_back("location");
    if (opts.output_checker)
    {
        head.push_back("checker");
    }
    if (opts.output_description)
    {
        head.push_back("description");
    }
    table.reset(head);

    for (auto &list : helper->get_all_bug_stats_list())
    {
        for (auto &bug : list.get_bug_desc_list())
        {
            row.clear();
            row.push_back(bug._file_name + ":" + std::to_string(bug._line_number) + ":" +
                          std::to_string(bug._column_number));
            if (opts.output_checker)
            {
                row.push_back(bug._checker_name);
            }
            if (opts.output_description)
            {
                row.push_back(bug._description);
            }
            table.append_row(row);
        }
    }
    stream << table.to_md() << std::endl;

    if (opts.output_stats)
    {
        head.clear();
        head.emplace_back("analysis_time");
        head.emplace_back("bug_num");
        head.emplace_back("bug_type_num");
        head.emplace_back("file_num");
        table.reset(head);
       
        row.clear();
        row.emplace_back(std::to_string(helper->get_all_analysis_time()));
        row.emplace_back(std::to_string(helper->get_all_bug_num()));
        row.emplace_back(std::to_string(helper->get_all_bug_type_num()));
        row.emplace_back(std::to_string(helper->get_all_file_num()));
        table.append_row(row);

        stream << table.to_md() << std::endl;

        head.clear();
        head.emplace_back("bug_types");
        table.reset(head);
        for (auto &bug_type : helper->get_all_bug_type_list())
        {
            row.clear();
            row.emplace_back(bug_type);
            table.append_row(row);
        }
        stream << table.to_md() << std::endl;

        head.clear();
        head.emplace_back("file_name");
        table.reset(head);
        for (auto &file_name : helper->get_filename_list())
        {
            row.clear();
            row.emplace_back(file_name);
            table.append_row(row);
        }
        stream << table.to_md() << std::endl;

        stream << "file statistics:" << std::endl << std::endl;
        
        for (auto &file_stat : helper->get_file_stats_list())
        {
            stream << "file name: " << file_stat._filename << std::endl;

            head.clear();
            head.emplace_back("analysis_time");
            head.emplace_back("bug_num");
            head.emplace_back("bug_type_num");
            table.reset(head);

            row.clear();
            row.emplace_back(std::to_string(file_stat.get_analysis_time()));
            row.emplace_back(std::to_string(file_stat.get_all_bug_num()));
            row.emplace_back(std::to_string(file_stat.get_bug_type_num()));
            table.append_row(row);

            stream << table.to_md() << std::endl;

            head.clear();
            head.emplace_back("bug_type");
            table.reset(head);

            for (auto &bug_type : file_stat.get_bug_type_list())
            {
                row.clear();
                row.emplace_back(bug_type);
                table.append_row(row);
            }
            stream << table.to_md() << std::endl;

            for (auto &bug_stats : file_stat.get_bug_stats_list())
            {
                stream << "bug num: " << bug_stats.get_num() << std::endl;

                head.clear();
                head.emplace_back("file_name");
                head.emplace_back("line_no");
                head.emplace_back("col_no");
                head.emplace_back("checker_name");
                head.emplace_back("description");
                table.reset(head);

                for (auto &desc : bug_stats.get_bug_desc_list())
                {
                    row.clear();
                    row.emplace_back(desc._file_name);
                    row.emplace_back(std::to_string(desc._line_number));
                    row.emplace_back(std::to_string(desc._column_number));
                    row.emplace_back(desc._checker_name);
                    row.emplace_back(desc._description);
                    table.append_row(row);
                }

                stream << table.to_md() << std::endl;
            }
        }
    }

    stream.close();
}

static void append_space(std::string& str, int num)
{
    while (num-- > 0)
    {
        str += ' ';
    }
}

static void append_space(std::ostream &stream, int num)
{
    while (num-- > 0)
        stream << ' ';
}

struct StrBuffer
{
  private:
    const std::string *str = nullptr;

    bool mem_mng = false;

    size_t offset = 0;

  public:
    StrBuffer() = default;
    StrBuffer(StrBuffer &o) : str(o.str), offset(o.offset), mem_mng(o.mem_mng)
    {
        o.mem_mng = false;
    }
    StrBuffer(StrBuffer &&o) noexcept : str(o.str), offset(o.offset), mem_mng(o.mem_mng)
    {
        o.mem_mng = false;
    }
    StrBuffer(const std::string &_str)
    {
        reset(_str);
    }
    StrBuffer(std::string &&_str)
    {
        reset(std::move(_str));
    }

    const std::string &get_str() const
    {
        return *str;
    }

    size_t get_offset() const
    {
        return offset;
    }

    void set_offset(size_t _offset)
    {
        offset = _offset;
    }

    bool is_end() const
    {
        return offset == str->length();
    }

    void reset(const std::string &_str)
    {
        if (str && mem_mng)
            delete str;
        str = &_str;
        offset = 0;
        mem_mng = false;
    }

    void reset(std::string &&_str)
    {
        if (str && mem_mng)
            delete str;
        str = new std::string(std::move(_str));
        offset = 0;
        mem_mng = true;
    }

    size_t write(std::string &dst_str, size_t length)
    {
        size_t dst_offset = offset + length;
        if (dst_offset > str->length())
            dst_offset = str->length();
        length = dst_offset - offset;

        dst_str.append(str->begin() + offset, str->begin() + dst_offset);
        offset = dst_offset;
        return length;
    }

    size_t write(std::ostream &out, size_t length)
    {
        size_t dst_offset = offset + length;
        if (dst_offset > str->length())
            dst_offset = str->length();
        length = dst_offset - offset;

        out.write(str->c_str() + offset, length);
        /*for (int i = offset; i < dst_offset; i++) {
          out << str[i];
        }*/

        offset = dst_offset;
        return length;
    }

    ~StrBuffer()
    {
        if (str && mem_mng)
        {
            delete str;
            mem_mng = false;
        }
    }
};

static void append_line(std::string &dst_str, std::vector<std::string> &strs, const std::vector<int> &widths)
{
    std::vector<StrBuffer> bufs;
    for (auto &str : strs)
        bufs.emplace_back(str);
    while (true)
    {
        bool end = true;
        for (int i = 0; i < widths.size(); i++)
        {
            int width = widths[i];
            auto &buf = bufs[i];

            if (!buf.is_end())
            {
                end = false;
                size_t sz = buf.write(dst_str, (size_t)width - 3);
                append_space(dst_str, width - sz);
            }
        }
        if (end)
            break;
        dst_str += '\n';
    }
}

static void append_line(std::ostream &stream, std::vector<StrBuffer> &bufs, const std::vector<int> &widths)
{
    while (true)
    {
        bool end = true;
        for (int i = 0; i < widths.size(); i++)
        {
            int width = widths[i];
            auto &buf = bufs[i];

            if (!buf.is_end())
            {
                end = false;
                size_t sz = buf.write(stream, (size_t)width - 3);
                append_space(stream, width - sz);
            }
        }
        if (end)
            break;
        stream << std::endl;
    }
}

static void append_line(std::ostream &stream, const std::vector<std::string> &strs, const std::vector<int> &widths)
{
    std::vector<StrBuffer> bufs;
    for (auto &str : strs)
        bufs.emplace_back(str);
    append_line(stream, bufs, widths);
}



//#include <Windows.h>
void OutputManager::append_text(std::ostream &stream, const OutputOptions &opts)
{
    // SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
    // diags
    auto helper = StatsHelper::instance();
    std::vector<std::string> head;
    std::vector<std::string> row;
    Table table;

    head.clear();
    head.push_back("location");
    if (opts.output_checker)
    {
        head.push_back("checker");
    }
    if (opts.output_description)
    {
        head.push_back("description");
    }
    table.reset(head);

   for (auto &list : helper->get_all_bug_stats_list())
    {
        for (auto &bug : list.get_bug_desc_list())
        {
            row.clear();
            row.push_back(bug._file_name + ":" + std::to_string(bug._line_number) + ":" +
                          std::to_string(bug._column_number));
            if (opts.output_checker)
            {
                row.push_back(bug._checker_name);
            }
            if (opts.output_description)
            {
                row.push_back(bug._description);
            }
            table.append_row(row);
        }
    }
    stream << table.to_text() << std::endl;

    if (opts.output_stats)
    {
        head.clear();
        head.emplace_back("analysis_time");
        head.emplace_back("bug_num");
        head.emplace_back("bug_type_num");
        head.emplace_back("file_num");
        table.reset(head);

        row.clear();
        row.emplace_back(std::to_string(helper->get_all_analysis_time()));
        row.emplace_back(std::to_string(helper->get_all_bug_num()));
        row.emplace_back(std::to_string(helper->get_all_bug_type_num()));
        row.emplace_back(std::to_string(helper->get_all_file_num()));
        table.append_row(row);

        stream << table.to_text() << std::endl;

        head.clear();
        head.emplace_back("bug_types");
        table.reset(head);
        for (auto &bug_type : helper->get_all_bug_type_list())
        {
            row.clear();
            row.emplace_back(bug_type);
            table.append_row(row);
        }
        stream << table.to_text() << std::endl;

        head.clear();
        head.emplace_back("file_name");
        table.reset(head);
        for (auto &file_name : helper->get_filename_list())
        {
            row.clear();
            row.emplace_back(file_name);
            table.append_row(row);
        }
        stream << table.to_text() << std::endl;

        stream << "file statistics:" << std::endl;

        for (auto &file_stat : helper->get_file_stats_list())
        {
            stream << "file name: " << file_stat._filename << std::endl;

            head.clear();
            head.emplace_back("analysis_time");
            head.emplace_back("bug_num");
            head.emplace_back("bug_type_num");
            table.reset(head);

            row.clear();
            row.emplace_back(std::to_string(file_stat.get_analysis_time()));
            row.emplace_back(std::to_string(file_stat.get_all_bug_num()));
            row.emplace_back(std::to_string(file_stat.get_bug_type_num()));
            table.append_row(row);

            stream << table.to_text() << std::endl;

            head.clear();
            head.emplace_back("bug_type");
            table.reset(head);

            for (auto &bug_type : file_stat.get_bug_type_list())
            {
                row.clear();
                row.emplace_back(bug_type);
                table.append_row(row);
            }
            stream << table.to_text() << std::endl;

            for (auto &bug_stats : file_stat.get_bug_stats_list())
            {
                stream << "bug num: " << bug_stats.get_num() << std::endl;

                head.clear();
                head.emplace_back("file_name");
                head.emplace_back("line_no");
                head.emplace_back("col_no");
                head.emplace_back("checker_name");
                head.emplace_back("description");
                table.reset(head);

                for (auto &desc : bug_stats.get_bug_desc_list())
                {
                    row.clear();
                    row.emplace_back(desc._file_name);
                    row.emplace_back(std::to_string(desc._line_number));
                    row.emplace_back(std::to_string(desc._column_number));
                    row.emplace_back(desc._checker_name);
                    row.emplace_back(desc._description);
                    table.append_row(row);
                }

                stream << table.to_text() << std::endl;
            }
        }
    }

    stream.flush();

    // SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
    //                        FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}

void OutputManager::output_text(const std::string &filepath, const OutputOptions &opts)
{
    std::string path = filepath.back() == '/' ? filepath + "report.txt" : filepath + "/report.txt";
    std::fstream stream(path, std::ios::out);
    if (!stream.is_open())
    {
        assert(false && "file open failed: %s\n");
        return;
    }

    append_text(stream, opts);
    stream.close();
}

void OutputManager::output_console(const OutputOptions &opts)
{
    append_text(std::cout, opts);
}