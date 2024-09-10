#include "FileSystemTree.h"
#include <cassert>

////////////////////////////////////////////////////////////////////////////////
//                      class FileSystemTree method def
////////////////////////////////////////////////////////////////////////////////
void FileSystemTree::build_tree(const std::set<std::filesystem::path> &file_list)
{
    std::set<std::filesystem::path>::iterator file_list_iter = file_list.begin();
    FileTreeNode *tree_iter = _root;

    while (file_list_iter != file_list.end())
    {
        const std::filesystem::path file_path = file_list_iter.operator*();
        assert(std::filesystem::exists(file_path) && "does not exist corresponding file path");
        std::stack<std::filesystem::path> node_list = parse_file_path(file_path);

        while (!node_list.empty())
        {
            const std::filesystem::path node_path = node_list.top();
            const std::filesystem::path node_parent_path = node_path.parent_path();
            node_list.pop();

            if (_tree_nodes_map.find(node_path) != _tree_nodes_map.end())
            {
                continue;
            }
            else
            {
                if (_tree_nodes_map.find(node_parent_path) != _tree_nodes_map.end())
                {
                    tree_iter = _tree_nodes_map.at(node_parent_path);

                    FileTreeNode *new_node;

                    if (std::filesystem::exists(node_path))
                    {
                        if (std::filesystem::is_directory(node_path))
                        {
                            new_node = new DirectoryNode(node_path, tree_iter);
                        }
                        else if (std::filesystem::is_regular_file(node_path))
                        {
                            new_node = new RegularFileNode(node_path, tree_iter);
                        }
                        else
                        {
                            new_node = new OtherFileNode(node_path, tree_iter);
                        }
                    }

                    if (tree_iter->is_directory())
                    {
                        tree_iter->add_node(new_node);
                    }

                    _tree_nodes_map.insert(std::pair<std::filesystem::path, FileTreeNode *>(node_path, new_node));
                }
            }
        }

        file_list_iter.operator++();
    }
}

std::stack<std::filesystem::path> FileSystemTree::parse_file_path(const std::filesystem::path &file_path)
{
    std::filesystem::path path = file_path;
    std::stack<std::filesystem::path> path_list;

    while (path != _entry_path)
    {
        path_list.push(path);
        path = path.parent_path();
    }

    return path_list;
}

