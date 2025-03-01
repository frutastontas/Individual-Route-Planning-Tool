//
// Created by joaop on 01/03/2025.
//


#include <fstream>
#include <sstream>
#include <unordered_set>
#include "Location.h"
#include "data_structures/UrbanMap.h"


template <typename T>
void createLocations(UrbanMap<T> *urban_map) {
    std::ifstream is;
    std::string name, Code, line,tmp;
    int ID, parking;
    is.open("../data/Locations.csv");
    getline(is, line);
    while (getline(is, line)) {
        std::stringstream iss(line);
        std::getline(iss,name,',');
        std::getline(iss,Code,',');
        std::getline(iss,tmp,',');
        ID = std::stoi(tmp);
        std::getline(iss,tmp,',');
        parking = std::stoi(tmp);
        //create Location using unordered map
        Location location = Location(name,Code,ID,parking);

        urban_map->addLocation(Code);
    }
}

template <typename T>
void createRoads(UrbanMap<T>* urban_map) {
    std::ifstream is;
    is.open("../data/Locations.csv");
    std::string code1, code2, line, tmp;
    int driving, walking;
    getline(is, line);
    while (getline(is, line)) {
        std::stringstream iss(line);
        std::getline(iss,code1,',');
        std::getline(iss,code2,',');
        std::getline(iss,tmp,',');
        driving = std::stoi(tmp);
        std::getline(iss,tmp,',');
        walking = std::stoi(tmp);
        urban_map->addRoad(code1,code2,driving);
    }
}


template <typename T>
UrbanMap<T>* createUrbanMap() {

    UrbanMap<T> urbanMap;
    createLocations(&urbanMap);
}

