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

#include <iostream>
#include <thread>
#include <chrono>

/**
 * @brief Initializes the menu with a dynamic welcome effect.
 *
 * Displays a welcome message with a loading animation before
 * proceeding to the menu options.
 */
void Menu::init() {
    std::cout << "=====================================\n";
    std::cout << "🚀 Welcome to the Navigation System! 🚀\n";
    std::cout << "=====================================\n";

    std::cout << "Loading menu";

    // Simulate loading animation
    for (int i = 0; i < 4; i++) {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        std::cout << "." << std::flush;
    }

    std::cout << "\n\n🎯 Wich type of route would you like to do today?\n";

    chooseOption();
}


void Menu::end() {
    // Static end function implementation
}


/**
 * @brief Displays a menu and prompts the user to choose an option.
 *
 * This function displays a list of options for the user to choose from, handles the user input,
 * and invokes the appropriate functions based on the selected option. It validates the user's input,
 * ensuring that only valid numbers (1, 2, 3, or 0) are accepted. The menu continues to loop until
 * the user selects the exit option (0).
 *
 */
void Menu::chooseOption() {
    UrbanMap<std::string> urban_map = createUrbanMap<std::string>();
    int option = -1;  // Initialize with an invalid value to enter the loop

    while (option != 0) {  // Loop until the user chooses to exit (option 0)
        std::cout << "\nChoose an option:\n";
        std::cout << "1. Option 1\n";
        std::cout << "2. Option 2\n";
        std::cout << "3. Option 3\n";
        std::cout << "0. Exit\n";
        std::cout << "Enter your choice: ";
        std::cin >> option;

        switch (option) {
            case 1:
                handleOption1(&urban_map);
            break;
            case 2:
                handleOption2(&urban_map);
            break;
            case 3:
                handleOption3(&urban_map);
            break;
            case 0:
                std::cout << "Exiting the program. Goodbye!\n";
            break;
            default:
                std::cout << "Invalid option. Please try again.\n";
            break;
        }
    }
}


void Menu::handleOption1(UrbanMap<std::string>* urban_map) {
    std::string source, destination;
    std::cout << "Enter source ID: ";
    std::cin >> source;

    std::cout << "Enter destination ID: ";
    std::cin >> destination;

    std::cout <<"Enter the mode that you want the route to be\n"<<"1.Driving\n"<<"0.Walking" <<std::endl;
    bool mode;
    std::cin >> mode;


    std::ofstream outFile("../input/input1.txt");
    if (outFile.is_open()) {
        outFile << "Mode:";
        if (mode == 1) {
            outFile << "driving";
        }else{
            outFile << "walking";
        }
        outFile<<std::endl;

        outFile << "Source:" << source << "\n";
        outFile << "Destination:" << destination << "\n";
        outFile.close();
        std::cout << "Data written to input1.txt\n";
    } else {
        std::cerr << "Error opening file.\n";
    }

    case1(urban_map);
    urban_map->resetGraph();
    int option;
    std::cout<< "Would you like to read the output file?:\n"<<"1.Yes\n"<<"0.No\n";
    std::cin >> option;
    if (option == 1) {
        readOutputFile();
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
}


void Menu::handleOption2(UrbanMap<std::string>* urban_map) {
    std::string source;
    std::cout << "Enter source ID: ";
    std::cin >> source;
    writeToFile("input2.txt", source);
    resetGraph();
}

void Menu::handleOption3(UrbanMap<std::string>* urban_map) {
    std::string source;
    std::cout << "Enter source ID: ";
    std::cin >> source;
    writeToFile("input3.txt", source);
    resetGraph();
}

/**
 * @brief Reads and displays the contents of "output1.txt".
 *
 * This function opens "output.txt", reads its contents line by line,
 * and prints them to the console. If the file cannot be opened,
 * an error message is displayed.
 */
void Menu::readOutputFile() {
    std::ifstream inFile("../output/output1.txt");  // Open the file for reading

    if (!inFile) {
        std::cerr << "Error: Could not open output.txt\n";
        return;
    }

    std::string line;
    while (std::getline(inFile, line)) {  // Read file line by line
        std::cout << line << std::endl;
    }
    inFile.close();  // Close the file
}

void Menu::writeToFile(const std::string& filename, const std::string& content) {

}

void Menu::resetGraph() {

}
