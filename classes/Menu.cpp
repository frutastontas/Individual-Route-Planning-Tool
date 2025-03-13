//
// Created by joaop on 01/03/2025.
//

#include "Menu.h"
#include <iostream>
#include "DataParser.h"
#include "Planning.h"

Menu::Menu() {
    std::cout << "\t\t\n\n" << "Bem-vindo!\n";
}


void Menu::init() {

    while (1) {
        std::string input;
        std::cout<<"Que tipo de plano de viagem deseja fazer hoje?"<<"\n"
                    <<"[1] Realizar uma das operaçoes"<<"\n" <<"[2] Sair do programa "<<"\n";
        std::cin>>input;
        switch (stoi(input)) {
            case 1:
                chooseOption();
                break;
            case 2:
                return;
            default:
                std::cout<<"Input inválido"<<std::endl;
                std::cin.clear();
                std::cin.ignore(INT_MAX, '\n');
        }
    }
}

void Menu::chooseOption() {
    UrbanMap<std::string> urban_map = createUrbanMap<std::string>();
    UrbanMap<std::string> urban_map_copy = urban_map; //this is a copy to reset urban_map after each use of a case
    urban_map.setDrivingMode(true);
    case1(&urban_map);
}
