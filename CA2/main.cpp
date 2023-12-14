#include "XMLParser.h"
#include <iostream>

using namespace std;
void printTree(TreeNode* node, int depth = 0, bool includeSize = false) {
    if (!node) return;

    // Indentation for visual hierarchy
    std::string indentation(depth * 4, ' ');

    // Print node details
    std::cout << indentation << "- " << node->name;
    if (node->type == "file" && includeSize) {
        std::cout << " (File, Size: " << node->length << ")";
    }
    std::cout << std::endl;

    // Recursive call for children
    for (auto child : node->children) {
        printTree(child, depth + 1, includeSize);
    }
}


int main() {
    std::string xmlFilePath = "vs_sample.xml";
    XMLParser parser(xmlFilePath);
    TreeNode* root = parser.parseXML();

    cout << parser.parseXML();
    if (parser.validateXML()) {
        if (root) {
            printTree(root);
        }
        else {
            std::cerr << "Failed to parse XML file." << std::endl;
            return 1;
        }
    }
    else {
        std::cerr << "XML validation failed." << std::endl;
        return 1;
    }

    int choice;
    do {
        std::cout << "\n--- MENU ---\n";
        std::cout << "1. Determine the number of items in a folder\n";
        std::cout << "2. Determine the amount of memory used by a folder\n";
        std::cout << "3. Prune the tree to remove empty folders\n";
        std::cout << "4. Find a file/folder\n";
        std::cout << "5. Display contents of a folder\n";
        std::cout << "6. Exit\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear the input buffer

        std::string folderName;
        TreeNode* folderNode;
        std::string path = "C:/";
        int prunedFoldersCount;

        switch (choice) {
        case 1:
            std::cout << "Enter folder name: ";
            std::getline(std::cin, folderName);
            folderNode = root->findNode(folderName);
            if (folderNode) {
                std::cout << "Number of items in " << folderName << ": " << folderNode->countItems() << std::endl;
            }
            else {
                std::cout << "Folder not found." << std::endl;
            }
            break;
        case 2:
            std::cout << "Enter folder name: ";
            std::getline(std::cin, folderName);
            folderNode = root->findNode(folderName);
            if (folderNode) {
                std::cout << "Memory used by " << folderName << ": " << folderNode->memoryUsed() << " bytes" << std::endl;
            }
            else {
                std::cout << "Folder not found." << std::endl;
            }
            break;
        case 3:
            prunedFoldersCount = root->pruneEmptyFolders();
            std::cout << "Number of empty folders removed: " << prunedFoldersCount << std::endl;
            break;
        case 4:
            std::cout << "Enter the partial or complete filename/folder name to search: ";
            std::getline(std::cin, folderName);
            path = root->findPath(folderName);
            if (!path.empty()) {
                std::cout << "Path: " << path << std::endl;
            }
            else {
                std::cout << "File/Folder not found." << std::endl;
            }
            break;
        case 5:
            std::cout << "Enter folder name to display its contents: ";
            std::getline(std::cin, folderName);
            folderNode = root->findNode(folderName);
            if (folderNode && folderNode->type == "dir") {
                std::cout << "Contents of folder \"" << folderName << "\":" << std::endl;
                printTree(folderNode, 0, true);  // Print with file size
            }
            else {
                std::cout << "Folder not found or it is a file." << std::endl;
            }
            break;
        case 6:
            std::cout << "Exiting program." << std::endl;
            break;
        default:
            std::cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 6);

    // Cleanup
    delete root;
    return 0;
}

