#pragma once
#include <vector>
#include <string>

class TreeNode {
public:
    std::string name;
    std::string type;  
    std::string length; 
    std::vector<TreeNode*> children;
    
  
    TreeNode* findNode(const std::string& searchName);
    TreeNode(std::string name, std::string type, std::string length = "");
    ~TreeNode();
    void addChild(TreeNode* child);
    int pruneEmptyFolders();
    int countItems();  
    int memoryUsed();
    std::string findPath(const std::string& searchName);
};
