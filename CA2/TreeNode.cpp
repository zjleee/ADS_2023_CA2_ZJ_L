#include "TreeNode.h"
#include <queue>

TreeNode::TreeNode(std::string name, std::string type, std::string length)
    : name(name), type(type), length(length) {}

TreeNode::~TreeNode() {
    for (auto child : children) {
        delete child;
    }
}

void TreeNode::addChild(TreeNode* child) {
    children.push_back(child);
}

int TreeNode::countItems() {
    int count = 0;
    for (auto child : children) {
        count += 1 + child->countItems(); 
    }
    return count;
}

int TreeNode::memoryUsed() {
    int totalMemory = 0;
    std::queue<TreeNode*> q;
    q.push(this);

    while (!q.empty()) {
        TreeNode* current = q.front();
        q.pop();

        if (current->type == "file") {
            totalMemory += std::stoi(current->length);
        }

        for (auto child : current->children) {
            q.push(child);
        }
    }

    return totalMemory;
}


TreeNode* TreeNode::findNode(const std::string& searchName) {
    if (this->name == searchName) {
        return this;
    }

    for (auto child : children) {
        TreeNode* result = child->findNode(searchName);
        if (result != nullptr) {
            return result;
        }
    }

    return nullptr; 
}

int TreeNode::pruneEmptyFolders() {
    int prunedCount = 0;
    auto it = children.begin();
    while (it != children.end()) {
        TreeNode* child = *it;

        prunedCount += child->pruneEmptyFolders();

        if (child->type == "dir" && child->children.empty()) {
            delete child;  
            it = children.erase(it);  
            prunedCount++;  
        }
        else {
            ++it;
        }
    }
    return prunedCount;
}

std::string TreeNode::findPath(const std::string& searchName) {
    if (this->name.find(searchName) != std::string::npos) {
        return "/" + this->name;  
    }

    for (auto child : children) {
        std::string path = child->findPath(searchName);
        if (!path.empty()) {
            return "/" + this->name + path; 
        }
    }

    return ""; 
}