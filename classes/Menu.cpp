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
    urban_map.setDrivingMode(true);
    int src;
    int dest;
    std::cin>>src >> dest;
    auto Lsrc = urban_map.getLocationSet()[src-1];
    auto Ldest = urban_map.getLocationSet()[dest-1];
    dijkstra(&urban_map, src);
    auto Lset = getPath(&urban_map, Lsrc->getInfo(), Ldest->getInfo());
    Lsrc->setVisited(false);
    for (int i = 0; i < Lset.size(); i++) {
        std::cout<<Lset[i]<<" ";
    }
    std::cout << Ldest->getDist() << std::endl;

    dijkstra(&urban_map, src);
    auto Lalt = getPath(&urban_map, Lsrc->getInfo(), Ldest->getInfo());
    for (int i = 0; i < Lalt.size(); i++) {
        std::cout<<Lalt[i]<<" ";
    }
    std::cout << Ldest->getDist() << std::endl;
}
