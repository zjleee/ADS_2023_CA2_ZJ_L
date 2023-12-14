#include "XMLParser.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Font.hpp>
#include <sstream>

using namespace std;

// Function prototypes for your functionalities
void countItems(TreeNode* root, sf::RenderWindow& window, sf::Font& font);  // Define this function
void determineMemoryUsage(TreeNode* root, sf::RenderWindow& window, sf::Font& font);  // Define this function
void pruneEmptyFolders(TreeNode* root, sf::RenderWindow& window, sf::Font& font);  // Define this function
void findFolder(TreeNode* root, sf::RenderWindow& window, sf::Font& font);  // Define this function
void displayFolderContents(TreeNode* root, sf::RenderWindow& window, sf::Font& font);  // Define this function
void printTreeToStream(TreeNode* node, std::stringstream& stream, int depth, bool includeSize);

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
                            case 0:
                                countItems(root, window, font);
                                break;
                            case 1:
                                determineMemoryUsage(root, window, font);
                                break;
                            case 2:
                                pruneEmptyFolders(root, window, font);
                                break;
                            case 3:
                                findFolder(root, window, font);
                                break;
                            case 4:
                                displayFolderContents(root, window, font);
                                break;
                            case 5: 
                                window.close();
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

void countItems(TreeNode* root, sf::RenderWindow& window, sf::Font& font) {
    std::string input;
    sf::Text inputText("", font, 20);
    inputText.setPosition(50, 50);
    inputText.setFillColor(sf::Color::White);

    sf::Text prompt("Enter folder name: ", font, 20);
    prompt.setPosition(50, 20);
    prompt.setFillColor(sf::Color::White);

    sf::Text outputText("", font, 20);
    outputText.setPosition(50, 100);
    outputText.setFillColor(sf::Color::Yellow);

    bool done = false;
    while (window.isOpen() && !done) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            else if (event.type == sf::Event::TextEntered) {
                if (event.text.unicode == '\b' && !input.empty()) {  // Handle backspace
                    input.pop_back();
                }
                else if (event.text.unicode < 128 && event.text.unicode != '\r' && event.text.unicode != '\n') {
                    input += static_cast<char>(event.text.unicode);
                }
            }
            else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Enter) {
                    done = true;  // User pressed Enter, finish input
                }
            }
        }

        inputText.setString(input);

        // Redraw the window
        window.clear();
        window.draw(prompt);
        window.draw(inputText);
        window.display();
    }

    TreeNode* folderNode = root->findNode(input);
    if (folderNode) {
        outputText.setString("Number of items in " + input + ": " + std::to_string(folderNode->countItems()));
    }
    else {
        outputText.setString("Folder not found.");
    }

    window.clear();
    window.draw(prompt);
    window.draw(inputText);
    window.draw(outputText);
    window.display();

    // Wait for a few seconds or until user closes the window
    sf::Clock clock;
    while (window.isOpen() && clock.getElapsedTime().asSeconds() < 4 ) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            else if (event.type == sf::Event::MouseButtonPressed) {
                // If any mouse button is pressed, exit the loop
                return;
            }
        }
    }
}

void determineMemoryUsage(TreeNode* root, sf::RenderWindow& window, sf::Font& font) {
    std::string input;
    sf::Text inputText("", font, 20);
    inputText.setPosition(50, 50);
    inputText.setFillColor(sf::Color::White);

    sf::Text prompt("Enter folder name: ", font, 20);
    prompt.setPosition(50, 20);
    prompt.setFillColor(sf::Color::White);

    sf::Text outputText("", font, 20);
    outputText.setPosition(50, 100);
    outputText.setFillColor(sf::Color::Yellow);

    bool done = false;
    while (window.isOpen() && !done) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            else if (event.type == sf::Event::TextEntered) {
                if (event.text.unicode == '\b' && !input.empty()) {
                    input.pop_back();
                }
                else if (event.text.unicode < 128 && event.text.unicode != '\r' && event.text.unicode != '\n') {
                    input += static_cast<char>(event.text.unicode);
                }
            }
            else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Enter) {
                    done = true;
                }
            }
        }

        inputText.setString(input);

        window.clear();
        window.draw(prompt);
        window.draw(inputText);
        window.display();
    }

    TreeNode* folderNode = root->findNode(input);
    outputText.setPosition(50, 100);
    outputText.setFillColor(sf::Color::Yellow);
    if (folderNode) {
        outputText.setString("Memory used by " + input + ": " + std::to_string(folderNode->memoryUsed()) + " bytes");
    }
    else {
        outputText.setString("Folder not found.");
    }

    window.clear();
    window.draw(prompt);
    window.draw(inputText);
    window.draw(outputText);
    window.display();

    sf::Clock clock;
    while (window.isOpen() && clock.getElapsedTime().asSeconds() < 4) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            else if (event.type == sf::Event::MouseButtonPressed) {
                return;
            }
        }
    }

}

void pruneEmptyFolders(TreeNode* root, sf::RenderWindow& window, sf::Font& font) {
    int prunedCount = root->pruneEmptyFolders();

    sf::Text outputText("Number of empty folders removed: " + std::to_string(prunedCount), font, 20);
    outputText.setPosition(50, 50);
    outputText.setFillColor(sf::Color::Yellow);

    window.clear();
    window.draw(outputText);
    window.display();

    sf::Clock clock;
    while (window.isOpen() && clock.getElapsedTime().asSeconds() < 4) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            else if (event.type == sf::Event::MouseButtonPressed) {
                return;
            }
        }
    }
}

void findFolder(TreeNode* root, sf::RenderWindow& window, sf::Font& font) {
    std::string input;
    sf::Text inputText("", font, 20);
    inputText.setPosition(50, 50);
    inputText.setFillColor(sf::Color::White);

    sf::Text prompt("Enter folder name: ", font, 20);
    prompt.setPosition(50, 20);
    prompt.setFillColor(sf::Color::White);

    sf::Text outputText("", font, 20);
    outputText.setPosition(50, 100);
    outputText.setFillColor(sf::Color::Yellow);

    bool done = false;
    while (window.isOpen() && !done) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            else if (event.type == sf::Event::TextEntered) {
                if (event.text.unicode == '\b' && !input.empty()) {
                    input.pop_back();
                }
                else if (event.text.unicode < 128 && event.text.unicode != '\r' && event.text.unicode != '\n') {
                    input += static_cast<char>(event.text.unicode);
                }
            }
            else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Enter) {
                    done = true;
                }
            }
        }

        inputText.setString(input);

        window.clear();
        window.draw(prompt);
        window.draw(inputText);
        window.display();
    }

    outputText.setPosition(50, 100);
    outputText.setFillColor(sf::Color::Yellow);
    
    string path = root->findPath(input);
    if (!path.empty()) {
        outputText.setString("Path: " + path);
    }
    else {
        outputText.setString("File/Folder not found.");
    }
    window.clear();
    window.draw(prompt);
    window.draw(inputText);
    window.draw(outputText);
    window.display();

    sf::Clock clock;
    while (window.isOpen() && clock.getElapsedTime().asSeconds() < 4) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            else if (event.type == sf::Event::MouseButtonPressed) {
                return;
            }
        }
    }
}

void displayFolderContents(TreeNode* root, sf::RenderWindow& window, sf::Font& font) {
    std::string input;
    sf::Text inputText("", font, 20);
    inputText.setPosition(50, 50);
    inputText.setFillColor(sf::Color::White);

    sf::Text prompt("Enter folder name to display contents: ", font, 20);
    prompt.setPosition(50, 20);
    prompt.setFillColor(sf::Color::White);

    sf::Text outputText("", font, 15); // Adjust font size for better readability
    outputText.setPosition(50, 150);    // Adjust position as needed
    outputText.setFillColor(sf::Color::Yellow);

    bool done = false;
    while (window.isOpen() && !done) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            else if (event.type == sf::Event::TextEntered) {
                if (event.text.unicode == '\b' && !input.empty()) {  // Handle backspace
                    input.pop_back();
                }
                else if (event.text.unicode < 128 && event.text.unicode != '\r' && event.text.unicode != '\n') {
                    input += static_cast<char>(event.text.unicode);
                }
            }
            else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Enter) {
                    done = true;  // User pressed Enter, finish input
                }
            }
        }

        inputText.setString(input);

        // Redraw the window
        window.clear();
        window.draw(prompt);
        window.draw(inputText);
        window.display();
    }

    TreeNode* folderNode = root->findNode(input);
    if (folderNode && folderNode->type == "dir") {
        std::stringstream contentStream;
        printTreeToStream(folderNode, contentStream, 0, true);  // Function to print tree contents to stream
        outputText.setString(contentStream.str());
    }
    else {
        outputText.setString("Folder not found or it is a file.");
    }

    window.clear();
    window.draw(prompt);
    window.draw(inputText);
    window.draw(outputText);
    window.display();

    // Wait for a few seconds or until user closes the window
    sf::Clock clock;
    while (window.isOpen() && clock.getElapsedTime().asSeconds() < 4) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            else if (event.type == sf::Event::MouseButtonPressed) {
                return;
            }
        }
    }
}

void printTreeToStream(TreeNode* node, std::stringstream& stream, int depth = 0, bool includeSize = false) {
    if (!node) return;

    // Indentation for visual hierarchy
    std::string indentation(depth * 4, ' ');

    // Append node details to stream
    stream << indentation << "- " << node->name;
    if (node->type == "file" && includeSize) {
        stream << " (File, Size: " << node->length << ")";
    }
    stream << "\n";

    // Recursive call for children
    for (auto child : node->children) {
        printTreeToStream(child, stream, depth + 1, includeSize);
    }
}