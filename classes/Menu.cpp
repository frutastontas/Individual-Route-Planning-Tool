//
// Created by joaop on 01/03/2025.
//

#include "Menu.h"
#include <iostream>
#include <fstream>
#include "DataParser.h"
#include "Planning.h"

Menu::Menu() {
    // Constructor implementation
}

void Menu::init() {
    chooseOption();
}

void Menu::end() {
    // Static end function implementation
}

void Menu::chooseOption() {
    int option;
    std::cout << "Choose an option:\n";
    std::cout << "1. Option 1\n";
    std::cout << "2. Option 2\n";
    std::cout << "3. Option 3\n";
    std::cin >> option;

    switch (option) {
        case 1:
            handleOption1();
            break;
        case 2:
            handleOption2();
            break;
        case 3:
            handleOption3();
            break;
        default:
            std::cout << "Invalid option. Please try again.\n";
            chooseOption();
            break;
    }
}

void Menu::handleOption1() {
    std::string source;
    std::cout << "Enter source ID: ";
    std::cin >> source;
    writeToFile("input1.txt", source);
    resetGraph();
}

void Menu::handleOption2() {
    std::string source;
    std::cout << "Enter source ID: ";
    std::cin >> source;
    writeToFile("input2.txt", source);
    resetGraph();
}

void Menu::handleOption3() {
    std::string source;
    std::cout << "Enter source ID: ";
    std::cin >> source;
    writeToFile("input3.txt", source);
    resetGraph();
}

void Menu::writeToFile(const std::string& filename, const std::string& content) {
    std::ofstream outFile(filename);
    if (outFile.is_open()) {
        outFile << content;
        outFile.close();
    } else {
        std::cerr << "Unable to open file " << filename << "\n";
    }
}

void Menu::resetGraph() {
}
