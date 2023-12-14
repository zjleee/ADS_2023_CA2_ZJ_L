#include "XMLParser.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Font.hpp>

using namespace std;

// Function prototypes for your functionalities
void countItems(TreeNode* root, string folderName, TreeNode* folderNode);  // Define this function
void determineMemoryUsage(TreeNode* root, string folderName, TreeNode* folderNode);  // Define this function
void pruneEmptyFolders(TreeNode* root);  // Define this function
void findFolder(TreeNode* root, string folderN);  // Define this function
void displayFolderContents(TreeNode* root, string folderName, TreeNode* folderNode);  // Define this function
void onButtonClick();

void printTree(TreeNode* node, int depth = 0, bool includeSize = false) {
    if (!node) return;

    // Indentation for visual hierarchy
    string indentation(depth * 4, ' ');

    // Print node details
    cout << indentation << "- " << node->name;
    if (node->type == "file" && includeSize) {
        cout << " (File, Size: " << node->length << ")";
    }
    cout << endl;

    // Recursive call for children
    for (auto child : node->children) {
        printTree(child, depth + 1, includeSize);
    }
}


int main() {

    string xmlFilePath = "vs_sample.xml";
    XMLParser parser(xmlFilePath);
    TreeNode* root = parser.parseXML();
   
    if (parser.validateXML()) {
        if (root) {
            printTree(root);
        }
        else {
            cerr << "Failed to parse XML file." << endl;
            return 1;
        }
    }
    else {
        cerr << "XML validation failed." << endl;
        return 1;
    }
    
    sf::RenderWindow window(sf::VideoMode(800, 600), "CA2");
    sf::Event event;

    sf::Font font;
    if (!font.loadFromFile("C:/Users/Lee/source/repos/CA2/Arial.ttf")) {
        std::cerr << "Error loading font" << std::endl;
        return -1;
    }

    const int numButtons = 6;
    sf::RectangleShape buttons[numButtons];
    sf::Text buttonTexts[numButtons];
    std::string buttonLabels[numButtons] = {
        "Count Items", "Memory Usage", "Prune Folders", "Find Folder", "Display Contents", "Exit"
    };

    for (int i = 0; i < numButtons; ++i) {
        buttons[i].setSize(sf::Vector2f(200, 50));
        buttons[i].setPosition(50, 50 + i * 60);
        buttons[i].setFillColor(sf::Color::White);

        buttonTexts[i].setFont(font);
        buttonTexts[i].setString(buttonLabels[i]);
        buttonTexts[i].setCharacterSize(20);
        buttonTexts[i].setPosition(60, 55 + i * 60);
        buttonTexts[i].setFillColor(sf::Color::Black);
    }

    while (window.isOpen())
    {
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }

            string folderName;
            TreeNode* folderNode = root;

            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    for (int i = 0; i < numButtons; ++i) {
                        if (buttons[i].getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                            switch (i) {
                            case 1:
                                countItems(root, folderName, folderNode);
                                break;
                            case 2:
                                determineMemoryUsage(root, folderName, folderNode);
                                break;
                            case 3:
                                pruneEmptyFolders(root);
                                break;
                            case 4:
                                findFolder(root, folderName);
                                break;
                            case 5:
                                displayFolderContents(root, folderName, folderNode);
                            case 6: 
                                onButtonClick();
                                break;
                            }
                        }
                    }
                }
            }
        }
        window.clear();
        for (int i = 0; i < numButtons; ++i) {
            window.draw(buttons[i]);
            window.draw(buttonTexts[i]);
        }
        window.display();
    }
   
    // Cleanup
    delete root;
    return 0;
}

/
void countItems(TreeNode* root, string folderName, TreeNode* folderNode) {
    
    cout << "Enter folder name: ";
    getline(cin, folderName);
    folderNode = root->findNode(folderName);
    if (folderNode) {
        cout << "Number of items in " << folderName << ": " << folderNode->countItems() << endl;
    }
    else {
        cout << "Folder not found." << endl;
    }
}

void determineMemoryUsage(TreeNode* root, string folderName, TreeNode* folderNode) {
    cout << "Enter folder name: ";
    getline(cin, folderName);
    folderNode = root->findNode(folderName);
    if (folderNode) {
        cout << "Memory used by " << folderName << ": " << folderNode->memoryUsed() << " bytes" << endl;
    }
    else {
        cout << "Folder not found." << endl;
    }
}

void pruneEmptyFolders(TreeNode* root) {
    cout << "Number of empty folders removed: " << root->pruneEmptyFolders() << endl;
}

void findFolder(TreeNode* root, string folderName) {
    cout << "Enter the partial or complete filename/folder name to search: ";
    getline(cin, folderName);
    string path = root->findPath(folderName);
    if (!path.empty()) {
        cout << "Path: " << path << endl;
    }
    else {
        cout << "File/Folder not found." << endl;
    }
}

void displayFolderContents(TreeNode* root, string folderName, TreeNode* folderNode) {
    cout << "Enter folder name to display its contents: ";
    getline(cin, folderName);
    folderNode = root->findNode(folderName);
    if (folderNode && folderNode->type == "dir") {
        cout << "Contents of folder \"" << folderName << "\":" << endl;
        printTree(folderNode, 0, true);  // Print with file size
    }
    else {
        cout << "Folder not found or it is a file." << endl;
    }
}

void onButtonClick() {
    std::cout << "Button clicked!" << std::endl;
    // Add your function logic here
}