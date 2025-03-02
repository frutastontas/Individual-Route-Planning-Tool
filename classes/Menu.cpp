//
// Created by joaop on 01/03/2025.
//

#include "Menu.h"
#include <iostream>

Menu::Menu() {
    std::cout << "\t\t\n\n" << "Bem-vindo!\n";
}


void Menu::init() {

    while (1) {
        std::string input;
        std::cout<<"Que tipo de plano de viagem deseja fazer hoje?"<<"\n"
                    <<"[1] Realizar uma das operações "<<"\n" <<"[2] Sair do programa "<<"\n";
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

}
