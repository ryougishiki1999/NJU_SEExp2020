#include "OutputOptions.h"

OutputOptions::OutputOptions()
{
    output_time = output_checker = output_description = output_stats = 0;
}
void OutputOptions::dump(std::ostream &os)
{
    os << "\tOutput Time: " << (bool)output_time << '\n';
    os << "\tOutput Checker: " << (bool)output_checker << '\n';
    os << "\tOutput Description: " << (bool)output_description << '\n';
    os << "\tOutput Stats: " << (bool)output_stats << '\n';
    os << "\tOutput Path: " << output_path << '\n';
    os << "\tOutput Formats: ";
    for (const auto &i : output_formats)
    {
        os << i << ' ';
    }
    os << '\n';
}
void OutputOptions::reset()
{
    output_time = output_checker = output_description = output_stats = 0;
    output_path = "";
    clear_output_format();
}

void OutputOptions::add_output_format(const std::string &f)
{
    output_formats.push_back(f);
}
void OutputOptions::clear_output_format()
{
    output_formats.clear();
}