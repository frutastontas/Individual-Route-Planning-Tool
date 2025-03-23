//
// Created by joaop on 01/03/2025.
//

#ifndef MENU_H
#define MENU_H

#include <string>

class Menu {
    public:
        Menu();
        void init();
        static void end();

    private:
        void chooseOption();
        void handleOption1();
        void handleOption2();
        void handleOption3();
        void writeToFile(const std::string& filename, const std::string& content);
        void resetGraph();
};

#endif //MENU_H
