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
 * @brief Displays the help menu with available commands and their descriptions.
 *
 * This function prints a list of available options in the application, providing
 * users with information on how to use the menu system. It includes options
 * for route selection, avoidance preferences, and eco-friendly travel options.
 *
 * The function pauses for 4 seconds after displaying the menu to allow users
 * to read the information.
 *
 * @note This function is part of the Menu class.
 */
void Menu::help() {
    std::cout << "================ Help Menu =================\n";
    std::cout << "Available Commands:\n";
    std::cout << "  1. option1 - Get the best two routes from your source to the destination\n";
    std::cout << "  2. option2 - Select specific locations and roads that you would like to avoid, and also include a location you would like to pass through\n";
    std::cout << "  3. option3 - See the best option to reach your destination in a economic and environment-friendly way by parking your car and then walking to the destination.\n";
    std::cout << "  4. exit    - Exit the application\n";
    std::cout << "===========================================\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(4000));
}



/**
 * @brief Initializes the menu with a dynamic welcome effect.
 *
 * Displays a welcome message with a loading animation before
 * proceeding to the menu options.
 */
void Menu::init() {
    std::cout << "=====================================\n";
    std::cout << "  Welcome to the Navigation System! \n";
    std::cout << "=====================================\n";

    std::cout << "Loading menu";

    // Simulate loading animation
    for (int i = 0; i < 4; i++) {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        std::cout << "." << std::flush;
    }

    std::cout << "\n\n Wich type of route would you like to do today?\n";

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

    while (option != 0) {
        try {
            std::cout << "\nChoose an option:\n";
            std::cout << "1. Option 1\n";
            std::cout << "2. Option 2\n";
            std::cout << "3. Option 3\n";
            std::cout << "4. Help\n";
            std::cout << "0. Exit\n";
            std::cout << "Enter your choice: ";

            if (!(std::cin >> option)) {  // Handle invalid input
                std::cin.clear();  // Clear error state
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard bad input
                std::cout << "Invalid input. Please enter a number.\n";
                continue;
            }

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
                case 4:
                    help();
                    break;
                case 0:
                    std::cout << "Exiting the program. Goodbye!\n";
                    break;
                default:
                    std::cout << "Invalid option. Please try again.\n";
                    break;
            }
        } catch (const std::exception &e) {  // Catch standard exceptions
            std::cerr << "An error occurred: " << e.what() << "\n";
        } catch (...) {  // Catch unknown exceptions
            std::cerr << "Please make sure the input.txt has the right configuration\n";
        }
    }
}


void Menu::handleOption1(UrbanMap<std::string>* urban_map) {

    std::cout << "Would you like to to set the input yourself\n";
    std::cout << "1. Input yourself\n";
    std::cout << "0. Ignore\n";
    int option ;
    std::cin >> option;
    if (option == 0) {
        case1(urban_map);
        urban_map->resetGraph();
        int option2;
        std::cout<< "Would you like to read the output file?:\n"<<"1.Yes\n"<<"0.No\n";
        std::cin >> option2;
        if (option2 == 1) {
            readOutputFile();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }else {
        std::string source, destination;
        std::cout << "Enter source ID: ";
        std::cin >> source;

        std::cout << "Enter destination ID: ";
        std::cin >> destination;

        std::cout <<"Enter the mode that you want the route to be\n"<<"1.Driving\n"<<"0.Walking" <<std::endl;
        bool mode;
        std::cin >> mode;


        std::ofstream outFile("../input/input.txt");
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
            std::cout << "Data written to input.txt\n";
        } else {
            std::cerr << "Error opening file.\n";
        }

        case1(urban_map);
        urban_map->resetGraph();
        int option2;
        std::cout<< "Would you like to read the output file?:\n"<<"1.Yes\n"<<"0.No\n";
        std::cin >> option2;
        if (option2 == 1) {
            readOutputFile();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }

}


void Menu::handleOption2(UrbanMap<std::string>* urban_map) {
    std::cout << "Please make sure the input file has the correct format\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    case2(urban_map);
    urban_map->resetGraph();
    int option2;
    std::cout<< "Would you like to read the output file?:\n"<<"1.Yes\n"<<"0.No\n";
    std::cin >> option2;
    if (option2 == 1) {
        readOutputFile();
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
}

void Menu::handleOption3(UrbanMap<std::string>* urban_map) {
    std::cout << "Please make sure the input file has the correct format\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    int flag = case3(urban_map);
    urban_map->resetGraph();
    int option2;
    std::cout<< "Would you like to read the output file?:\n"<<"1.Yes\n"<<"0.No\n";
    std::cin >> option2;
    if (option2 == 1) {
        readOutputFile();
        if (flag == 0) {
            std::cout<< "/////////////// Estimation File ////////////////////\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
            readEstimationFile();
        }
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
}

/**
 * @brief Reads and displays the contents of "output.txt".
 *
 * This function opens "output.txt", reads its contents line by line,
 * and prints them to the console. If the file cannot be opened,
 * an error message is displayed.
 */
void Menu::readOutputFile() {
    std::ifstream inFile("../output/output.txt");  // Open the file for reading

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

void Menu::readEstimationFile() {
    std::ifstream inFile("../output/estimation.txt");  // Open the file for reading

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


