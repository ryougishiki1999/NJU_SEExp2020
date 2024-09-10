#include "Preprocessor.h"
#include "../../core/Json/JsonProc.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

Preprocessor::PreprocessorPtr Preprocessor::_preprocessor = nullptr;

std::mutex Preprocessor::_m_mutex;

////////////////////////////////////////////////////////////////////////////////
//                      class Preprocessor method def
////////////////////////////////////////////////////////////////////////////////

Preprocessor::PreprocessorPtr Preprocessor::instance()
{
    if (_preprocessor == nullptr)
    {
        std::lock_guard<std::mutex> lock(_m_mutex);
        if (_preprocessor == nullptr)
        {
            _preprocessor = PreprocessorPtr(new Preprocessor);
        }
    }

    return _preprocessor;
}

bool Preprocessor::resolve_source(const std::string &source)
{
    _entry_path.assign(source);
    if (std::filesystem::exists(_entry_path))
    {
        if (_tree == nullptr)
        {
            _tree = new FileSystemTree(_entry_path);
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}

void Preprocessor::search_compile_commands_json_entry()
{
    search_compile_commands_json(_entry_path);
}

void Preprocessor::search_compile_commands_json(const std::filesystem::path &directory)
{
    for (const auto &dir_entry : std::filesystem::directory_iterator(directory))
    {
        auto path = dir_entry.path();
        auto filename = path.filename();

        if (std::filesystem::is_directory(dir_entry))
        {
            if (filename != std::string(ROOT_DIRECTORY) && filename != std::string(PARENT_DIRECTORY))
            {
                search_compile_commands_json(path);
            }
        }
        else if (filename == std::string(COMPILE_COMMANDS_FILENAME))
        {
            _compile_commands_json_list.push_back(path);
        }
    }
}

void Preprocessor::parse_compile_commands_json_entry()
{
    for (size_t index = 0; index < _compile_commands_json_list.size(); index++)
    {
        std::filesystem::path compile_commmands_path = _compile_commands_json_list.at(index);
        std::set<std::filesystem::path> file_list = parse_compile_commands_json(compile_commmands_path.string());

        //_all_source_file_list.push_back(file_list);
        _corresponding_source_file_map.insert(
            std::pair<std::filesystem::path, std::set<std::filesystem::path>>(compile_commmands_path, file_list));
    }
}

const std::set<std::filesystem::path> Preprocessor::parse_compile_commands_json(
    const std::string &compile_commands_json_path)
{

    std::set<std::filesystem::path> source_file_list;

    std::ifstream json_file_in(compile_commands_json_path, std::ios::in);

    if (json_file_in.is_open())
    {
        std::stringstream ss;
        ss << json_file_in.rdbuf();
        std::string json_content(ss.str());

        Json::Value &original = Json::parse(json_content);

        assert(original.get_type() == Json::ValueType::ARRAY && "compile_commands.json exists errors");
        Json::ArrayValue arr_value = dynamic_cast<Json::ArrayValue &>(original);

        const size_t arr_size = arr_value.size();

        for (size_t index = 0; index < arr_size; index++)
        {
            Json::Value &element = arr_value.get_value(index);

            assert(element.get_type() == Json::ValueType::OBJECT && "compile_commands.json exists errors...");
            Json::ObjectValue object = dynamic_cast<Json::ObjectValue &>(element);

            std::string path_str = Json::stringify(object.get_value(COMPILE_FILE_ATTRIBUTE));
            path_str = path_str.substr(1, path_str.size() - 2);

            const std::filesystem::path source_file_path = path_str;

            if (std::filesystem::exists(source_file_path))
            {
                if (source_file_list.find(source_file_path) == source_file_list.end())
                {
                    source_file_list.insert(source_file_path);
                }
            }

        }
    }
    json_file_in.close();

    return source_file_list;
}

void Preprocessor::build_file_tree()
{
    if (!_target_source_file_list.empty())
    {
        _tree->build_tree(_target_source_file_list);
    }
}
