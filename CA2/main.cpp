#include <iostream>
#include <stack>
#include <string>

// Function to extract the tag name from a string
std::string getTagName(const std::string& tag) {
    if (tag[1] == '/') {
        return tag.substr(2, tag.size() - 3);  // Closing tag
    }
    else {
        return tag.substr(1, tag.size() - 2);  // Opening tag
    }
}

// Function to check if the tag is an opening tag
bool isOpeningTag(const std::string& tag) {
    return tag[1] != '/';
}

// Function to validate XML using a stack
bool validateXML(const std::string& xml) {
    std::stack<std::string> tags;
    std::string::size_type pos = 0;

    while (pos < xml.length()) {
        std::string::size_type start = xml.find('<', pos);
        if (start == std::string::npos) break;

        std::string::size_type end = xml.find('>', start);
        if (end == std::string::npos) return false;  // Invalid XML: Tag not closed

        std::string tag = xml.substr(start, end - start + 1);
        if (isOpeningTag(tag)) {
            tags.push(getTagName(tag));
        }
        else {
            if (tags.empty() || tags.top() != getTagName(tag)) {
                return false;  // Invalid XML: Tag mismatch
            }
            tags.pop();
        }

        pos = end + 1;
    }

    return tags.empty();
}

int main() {
    std::string xml = "<root><child></child></root>";
    if (validateXML(xml)) {
        std::cout << "XML is valid.\n";
    }
    else {
        std::cout << "XML is invalid.\n";
    }

    return 0;
}
