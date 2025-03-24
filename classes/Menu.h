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

    private:
        void chooseOption();
        void handleOption1(UrbanMap<std::string>* urban_map);
        void handleOption2(UrbanMap<std::string>* urban_map);
        void handleOption3(UrbanMap<std::string>* urban_map);
        void writeToFile(const std::string& filename, const std::string& content);
        void resetGraph();
};

#endif //MENU_H
