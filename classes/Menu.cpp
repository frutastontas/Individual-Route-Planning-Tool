//
// Created by joaop on 01/03/2025.
//

#include "Menu.h"
#include <iostream>
#include "DataParser.h"

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


    auto v =urban_map.getLocationSet();
    for (int i=0; i<10; i++) {
        auto u = v[i];
        std::cout<<u->getInfo()<<" "<< i+1<<std::endl;
    }

}
