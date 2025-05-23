//
// Created by joaop on 01/03/2025.
//

#ifndef MENU_H
#define MENU_H

#include <string>
#include "data_structures/UrbanMap.h"

class Menu {
    public:
        Menu();
        void init();
        static void end();
        void readOutputFile();
        void readEstimationFile();
        void help();

    private:
        void chooseOption();
        void handleOption1(UrbanMap<std::string>* urban_map);
        void handleOption2(UrbanMap<std::string>* urban_map);
        void handleOption3(UrbanMap<std::string>* urban_map);

};

#endif //MENU_H
