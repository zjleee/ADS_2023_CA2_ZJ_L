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
        count += 1 + child->countItems(); // Count child and its descendants
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

        // If it's a file, add its size to the total
        if (current->type == "file") {
            totalMemory += std::stoi(current->length);
        }

        // Add all children to the queue
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

    return nullptr; // If not found
}

int TreeNode::pruneEmptyFolders() {
    int prunedCount = 0;
    auto it = children.begin();
    while (it != children.end()) {
        TreeNode* child = *it;

        // Prune child and count how many were removed
        prunedCount += child->pruneEmptyFolders();

        // Check if child is an empty folder
        if (child->type == "dir" && child->children.empty()) {
            delete child;  // Free memory
            it = children.erase(it);  // Remove from children list
            prunedCount++;  // Increment count for this node
        }
        else {
            ++it;
        }
    }
    return prunedCount;
}

std::string TreeNode::findPath(const std::string& searchName) {
    // Check if the current node matches the search name
    if (this->name.find(searchName) != std::string::npos) {
        return "/" + this->name;  // Found the node
    }

    // Search in children
    for (auto child : children) {
        std::string path = child->findPath(searchName);
        if (!path.empty()) {
            return "/" + this->name + path;  // Append current node's name to the path
        }
    }

    return "";  // Not found in this branch
}