#ifndef FILESYSTEM_TREE_H
#define FILESYSTEM_TREE_H
#include <filesystem>
#include <iostream>
#include <map>
#include <set>
#include <stack>
#include <string>
#include <vector>
#include <cassert>

class FileType;

class FileTreeNode;
class DirectoryNode;
class RegularFileNode;
class OtherFileNode;

class FileSystemTree;

/**
 * @brief class FileType 
 * @details this class aims to classify different class types..
 */
class FileType
{
  public:
  /**
   * @brief enum class Type
   * @details represent specify file types
   */
    enum class Type
    {
        Directory,
        RegularFile,
        OtherFile
    };

    FileType() = delete;
    FileType(FileType::Type type) : _type(type)
    {
    }
    FileType &operator=(const FileType &) = default;

    /**
     * @brief get file type
     * @return FileType
     */ 
    FileType::Type get_type() const
    {
        return _type;
    }
    
    /**
     * @brief print FileType into std::string
     * @return stdstring
     */ 
    const std::string to_string() const
    {
        switch (_type)
        {
        case FileType::Type::Directory:
            return "Directory: ";
            break;
        case FileType::Type::RegularFile:
            return "Regular: ";
            break;
        case FileType::Type::OtherFile:
            return "Other: ";
            break;
        default:
            return "Error Type: ";
            break;
        }
    }

  private:
    FileType::Type _type; ///< specify file type
};

/**
 * @brief, FileTreeNode reprents a file in tree
 * @details, a abstract class
 */ 
class FileTreeNode
{
  public:
    FileTreeNode() = delete;
    FileTreeNode(const FileType file_type, const std::filesystem::path &file_path, FileTreeNode *&parent_node)
        : _file_type(file_type), _file_path(file_path), _parent_node(parent_node)
    {
    }
    FileTreeNode &operator=(const FileTreeNode &) = default;

    /**
     * @brief compare to another FileTree
     * @param another(FileTreeNode)
     */ 
    bool operator==(const FileTreeNode &another)
    {
        return this->_file_path == another._file_path;
    }

    /**
     * @brief compare to another FileTree
     * @param file_path(filesystem::path)
     */ 
    bool operator==(const std::filesystem::path file_path)
    {
        return this->_file_path == file_path;
    }

    /**
     * @brief judge this node whether the root is
     * @return true, yes
     *         false, no
     */ 
    bool is_root() const
    {
        return _parent_node == nullptr;
    }

    /**
     * @brief judge this node whether the leaf is
     * @return true, yes
     *         false, no
     */ 
    bool is_leaf() const
    {
        return is_regular_file() || is_other_file() || (is_directory() && !has_child());
    }

    /**
     * @brief get file type from FileType
     * @return FileType::Type
     */ 
    const FileType::Type get_file_type() const
    {
        return _file_type.get_type();
    }

    /**
     * @brief get file path of the node
     * @return filesystem::path
     */ 
    const std::filesystem::path &get_file_path() const
    {
        return _file_path;
    }

    /**
     * @brief get parent node of this node
     * @return FileTreeNode*(pointer)
     */ 
    const FileTreeNode *get_parent() const
    {
        return _parent_node;
    }

    /**
     * @brief get children nodes of this node
     * @return vector<FileTreeNode*>
     */
    const std::vector<FileTreeNode *> &get_children() const
    {
        assert(_file_type.get_type() == FileType::Type::Directory && "file type errors");
        
        return _children;
    }

    /**
     * @brief virtual function, print FileTreeNode into std::string
     * @return std::string
     */ 
    virtual const std::string to_string() const = 0;

    /**
     * @brief virtual function, print FileTreeNode into std::string
     * @return std::string
     */ 
    virtual void add_node(FileTreeNode *&node) = 0;

    /**
     * @brief virtual function, judge whether the childeren exist
     * @return true, children nodes exist
     *         fasle, children nodes don't exist
     */ 
    virtual bool has_child() const = 0;

    /**
     * @brief virtual function, judge whether directory is
     * @return true, directory
     *         false, maybe regular file or other file
     */ 
    virtual bool is_directory() const = 0;

    /**
     * @brief virtual function, judge whether regular file is
     * @return true, regular file
     *         false, maybe directory or other file
     */ 
    virtual bool is_regular_file() const = 0;

    /**
     * @brief virtual function, judge whether other file is
     * @return true, other file
     *         false, maybe regular file or other file
     */ 
    virtual bool is_other_file() const = 0;

  protected:
    FileType _file_type; ///< File Type  of FileTreeNode
    std::filesystem::path _file_path; ///< File Path of FileTreeNode

    FileTreeNode *_parent_node; ///< parent node of this FileTreeNode
    std::vector<FileTreeNode *> _children; ///< children nodes of this FileTreeNode

  private:
};

/**
 * @brief, a subclass of FileTreeNode reprents a directory in tree
 * @details, a specify kind of FileTreeNode
 */ 
class DirectoryNode : public FileTreeNode
{
  public:
    DirectoryNode(const std::filesystem::path &file_path, FileTreeNode *parent_node)
        : FileTreeNode(FileType(FileType::Type::Directory), file_path, parent_node)
    {
    }

    /**
     * @brief, print directory node in std::string
     * @return std::string
     */ 
    const std::string to_string() const override
    {
        // TODO: implement more
        return _file_type.to_string() + _file_path.string();
    }

    /**
     * @brief implementary function, add FileTreeNode to children nodes
     */ 
    void add_node(FileTreeNode *&node) override
    {
        _children.push_back(node);
    }

    /**
     * @brief implementary function
     */ 
    bool has_child() const override
    {
        return !_children.empty();
    }
    /**
     * @brief implementary function
     * @return true
     */ 
    bool is_directory() const override
    {
        return true;
    }

    /**
     * @brief implementary function
     * @return false
     */ 
    bool is_regular_file() const override
    {
        return false;
    }

    /**
     * @brief implementary function
     * @return false
     */ 
    bool is_other_file() const override
    {
        return false;
    }
};


/**
 * @brief, a subclass of FileTreeNode reprents a regular in tree
 * @details, a specify kind of FileTreeNode
 */ 
class RegularFileNode : public FileTreeNode
{
  public:
    RegularFileNode(const std::filesystem::path &file_path, FileTreeNode *parent_node)
        : FileTreeNode(FileType(FileType::Type::RegularFile), file_path, parent_node)
    {
    }

    /**
     * @brief, print regular file node in std::string
     * @return std::string
     */ 
    const std::string to_string() const override
    {
        return _file_type.to_string() + _file_path.string() + "\n";
    }

    /**
     * @brief implementary function
     * @details this function is banned in this kind of node
     */ 
    void add_node(FileTreeNode *&node) override
    {
        return;
    }

    /**
     * @brief implementary function
     * @return false
     */ 
    bool has_child() const override
    {
        return false;
    }

    /**
     * @brief implementary function
     * @return false
     */ 
    bool is_directory() const override
    {
        return false;
    }

    /**
     * @brief implementary function
     * @return true
     */ 
    bool is_regular_file() const override
    {
        return true;
    }

    /**
     * @brief implementary function
     * @return false
     */ 
    bool is_other_file() const override
    {
        return false;
    }
};

class OtherFileNode : public FileTreeNode
{
  public:
    OtherFileNode(const std::filesystem::path &file_path, FileTreeNode *parent_node)
        : FileTreeNode(FileType(FileType::Type::OtherFile), file_path, parent_node)
    {
    }

    /**
     * @brief, print other file node in std::string
     * @return std::string
     */ 
    const std::string to_string() const override
    {
        return _file_type.to_string() + _file_path.string() + "\n";
    }

    
    /**
     * @brief implementary function
     * @details this function is banned in this kind of node
     */ 
    void add_node(FileTreeNode *&node) override
    {
        return;
    }

    /**
     * @brief implementary function
     * @return false
     */
    bool has_child() const override
    {
        return false;
    }

    
    /**
     * @brief implementary function
     * @return false
     */ 
    bool is_directory() const override
    {
        return false;
    }

    
    /**
     * @brief implementary function
     * @return false
     */ 
    bool is_regular_file() const override
    {
        return false;
    }

    
    /**
     * @brief implementary function
     * @return true
     */ 
    bool is_other_file() const override
    {
        return true;
    }
};

/**
 * @brief FileSystemTree, consisting of FileTreeNodes
 * @details organize the list of tree in FileTree
 */ 
class FileSystemTree
{
  public:
    FileSystemTree()
    {
        _root = nullptr;
    }
    FileSystemTree(const std::filesystem::path &entry_path) : _entry_path(entry_path)
    {
        init_tree_root();
    }

    FileSystemTree &operator=(const FileSystemTree &) = default;

    ~FileSystemTree()
    {
        delete _root;
        _root = nullptr;
    }
    /**
     * @brief set initial settings in FileTree
     * @param entry_path(filesystem::path)
     */ 
    void set_tree(const std::filesystem::path &entry_path)
    {
        _entry_path = entry_path;
        if (_root != nullptr)
        {
            delete _root;
        }

        init_tree_root();
    }

    /**
     * @brief get root of FileSystemTree
     * @return FileTreeNode*
     */ 
    const FileTreeNode *get_root() const
    {
        return _root;
    }

    /**
     * @brief get tree nodes map
     * @return map of <filesystem::path, FileTreeNode*>
     */ 
    const std::map<std::filesystem::path, FileTreeNode *> &get_tree_nodes_map() const
    {
        return _tree_nodes_map;
    }

    /**
     * @brief build tree with file list
     * @param file_list(set<filesystem::path>)
     */ 
    void build_tree(const std::set<std::filesystem::path> &file_list);

  private:
    /**
     * @brief initialize tree root
     */ 
    void init_tree_root()
    {

        if (!std::filesystem::exists(_entry_path))
        {
            std::cerr << "Oops, entry does not exists..." << std::endl;
            exit(-1);
        }

        if (std::filesystem::is_directory(_entry_path))
        {
            _root = new DirectoryNode(_entry_path, nullptr);
        }
        else if (std::filesystem::is_regular_file(_entry_path))
        {
            _root = new RegularFileNode(_entry_path, nullptr);
        }
        else
        {
            _root = new OtherFileNode(_entry_path, nullptr);
        }

        _tree_nodes_map.insert(std::pair<std::filesystem::path, FileTreeNode *>(_entry_path, _root));
    }

    /**
     * @brief parse file path
     * @param file_path(filesystem::path)
     */ 
    std::stack<std::filesystem::path> parse_file_path(const std::filesystem::path &file_path);

  private:

    std::filesystem::path _entry_path; ///< entry path of project folder

    FileTreeNode *_root; ///< root of FileTreeNode
    std::map<std::filesystem::path, FileTreeNode *> _tree_nodes_map; ///< map of file path and its corresponding tree node
};

#endif
