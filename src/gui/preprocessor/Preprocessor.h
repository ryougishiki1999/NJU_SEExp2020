#ifndef GUI_PREPROCESSOR_H
#define GUI_PREPROCESSOR_H
#include "FileSystemTree.h"
#include <filesystem>
#include <map>
#include <memory>
#include <mutex>
#include <queue>
#include <set>
#include <string>
#include <vector>

#define ROOT_DIRECTORY "."
#define PARENT_DIRECTORY ".."

#define COMPILE_COMMANDS_FILENAME "compile_commands.json"
#define COMPILE_FILE_ATTRIBUTE "file"

class Preprocessor;

//===----------------------------------------------------------
//				class Preprocessor Decl
//===----------------------------------------------------------
/**
 * @brief class Preprocessor to preprocess the input of gui
 * @details this class is a singleton class, aiming to resolve project folder to find compile_commands.json
 */
class Preprocessor
{
    typedef std::shared_ptr<Preprocessor> PreprocessorPtr;

  public:
    /**
     * @brief  instance
     * @details get the instance of the singleton class
     * @return  return the instance of StatsHelper as a pointer
     */
    static PreprocessorPtr instance();

    Preprocessor(const Preprocessor &) = delete;
    Preprocessor &operator=(const Preprocessor &) = delete;
    ~Preprocessor() = default;

    /**
     * @brief clear all information stored in Preprocess
     * @details, clear FileTree, list of source files and so on
     */
    void clear()
    {
        delete _tree;
        _tree = nullptr;
        _entry_path.clear();
        _target_source_file_list.clear();
        _compile_commands_json_list.clear();
        _corresponding_source_file_map.clear();
    }
    /**
     * @brief get list of compile_commands.json found in preprocessing
     * @return vector of filesystem::path
     */
    const std::vector<std::filesystem::path> &get_compile_commands_list()
    {
        return _compile_commands_json_list;
    }

    /**
     * @brief get map of compile_command.json and its corresponding source list
     * @return map of <filesystem::path, set<filesystem::path>
     */
    const std::map<std::filesystem::path, std::set<std::filesystem::path>> &get_corres_file_map() const
    {
        return _corresponding_source_file_map;
    }

    /**
     * @brief set the list of target source files
     * @details called by gui's preprocessing, input is from gui interaction
     * @param target_file_list(set<filesystem::path>)
     */
    void set_target_source_file_list(const std::set<std::filesystem::path> &target_file_list)
    {
        _target_source_file_list = target_file_list;
    }

    /** @brief get FileTree consisting of target source files
     *  @details after building File Tree, the list of source files consists of a tree
     *  @return FileSystemTree*(pointer)
     */
    const FileSystemTree *get_file_tree() const
    {
        return _tree;
    }
    /**
     * @brief resolve original gui input
     * @param source(std::string)
     * @return true, legal input
     *         false, illegal input
     */
    bool resolve_source(const std::string &source);

    /**
     * @brief search compile_commands.json in project folder
     * @details the entry inferface of the process
     */
    void search_compile_commands_json_entry();
    /**
     * @brief parse the target compile_commands.json
     * @details parse to get target list of source files
     */
    void parse_compile_commands_json_entry();

    /**
     * @brief build related file_tree
     * @details build tree with target list or source files
     */
    void build_file_tree();

  private:
    Preprocessor()
    {
        _tree = nullptr;
    }

    /**
     * @brief search compile_commands.json in selective folder
     * @param directory(filesystem::path)
     */
    void search_compile_commands_json(const std::filesystem::path &directory);

    /**
     * @brief parse the selective compile_commands.jsons found in project folder
     * @param json_path(std::string), compile_commands.json path
     * @details parse to get target list of source files
     */
    const std::set<std::filesystem::path> parse_compile_commands_json(const std::string &json_path);

  private:
    static PreprocessorPtr _preprocessor;
    static std::mutex _m_mutex;

    FileSystemTree *_tree; ///< FileSystemTree entry

    std::filesystem::path _entry_path; ///< entry_path of project folder after resolving source

    std::set<std::filesystem::path> _target_source_file_list; ///< target list of source files to be analyzed

    std::vector<std::filesystem::path> _compile_commands_json_list; ///< list of compile_command.json

    std::map<std::filesystem::path, std::set<std::filesystem::path>> _corresponding_source_file_map; ///< map of path of compile_commands.json and corresponding list of source files
};

#endif
