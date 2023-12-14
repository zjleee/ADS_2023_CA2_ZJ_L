#pragma once
#include <string>
#include "TreeNode.h"
#include "tinyxml2.h"

class XMLParser {
public:
    XMLParser(const std::string& filename);
    TreeNode* parseXML();
    bool validateXML();

private:
    std::string filename;
    TreeNode* processDirectory(tinyxml2::XMLElement* element);
    TreeNode* processFile(tinyxml2::XMLElement* element);
};
