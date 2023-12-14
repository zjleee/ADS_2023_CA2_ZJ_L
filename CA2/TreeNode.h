#pragma once
#include <vector>
#include <string>

class TreeNode {
public:
    std::string name;
    std::string type;  // "file" or "dir"
    std::string length; // For files
    std::vector<TreeNode*> children;
    
  

    TreeNode* findNode(const std::string& searchName);
    TreeNode(std::string name, std::string type, std::string length = "");
    ~TreeNode();
    void addChild(TreeNode* child);
    int pruneEmptyFolders();
    int countItems();  // Count the number of items in a folder
    int memoryUsed();
    std::string findPath(const std::string& searchName);
};
