#include "XMLParser.h"
#include "tinyxml2.h"
#include <stack>

XMLParser::XMLParser(const std::string& filename) : filename(filename) {}

TreeNode* XMLParser::parseXML() {
    tinyxml2::XMLDocument doc;
    doc.LoadFile(filename.c_str());

    tinyxml2::XMLElement* rootElement = doc.FirstChildElement("dir");
    if (!rootElement) return nullptr;

    return processDirectory(rootElement);
}

TreeNode* XMLParser::processDirectory(tinyxml2::XMLElement* element) {
    if (!element) return nullptr;

    tinyxml2::XMLElement* nameElement = element->FirstChildElement("name");
    std::string dirName = nameElement ? nameElement->GetText() : "";

    TreeNode* node = new TreeNode(dirName, "dir");

    for (tinyxml2::XMLElement* child = element->FirstChildElement(); child != nullptr; child = child->NextSiblingElement()) {
        std::string childType = child->Value();
        if (childType == "dir") {
            node->addChild(processDirectory(child));
        }
        else if (childType == "file") {
            node->addChild(processFile(child));
        }
    }
    return node;
}

TreeNode* XMLParser::processFile(tinyxml2::XMLElement* element) {
    if (!element) return nullptr;

    tinyxml2::XMLElement* nameElement = element->FirstChildElement("name");
    std::string fileName = nameElement ? nameElement->GetText() : "";

    tinyxml2::XMLElement* lengthElement = element->FirstChildElement("length");
    std::string fileLength = lengthElement ? lengthElement->GetText() : "";

    tinyxml2::XMLElement* typeElement = element->FirstChildElement("type");
    std::string fileType = typeElement ? typeElement->GetText() : "";

    return new TreeNode(fileName, "file", fileLength);
}

bool XMLParser::validateXML() {
    tinyxml2::XMLDocument doc;
    if (doc.LoadFile(filename.c_str()) != tinyxml2::XML_SUCCESS) {
        return false; // File could not be loaded
    }

    std::stack<std::string> tags;
    for (tinyxml2::XMLElement* elem = doc.FirstChildElement(); elem != nullptr; elem = elem->NextSiblingElement()) {
        if (elem->NoChildren()) {
            // Self-closing tag, no need to push to stack
        }
        else {
            // Push opening tag onto the stack
            tags.push(elem->Value());
        }

        // For each closing tag, check if it matches the top of the stack
        if (!tags.empty() && tags.top() == elem->Value()) {
            tags.pop();
        }
        else {
            // Mismatched or missing closing tag
            return false;
        }
    }

    return tags.empty(); // Return true if all tags were properly closed and nested
}