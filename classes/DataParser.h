//
// Created by joaop on 04/03/2025.
//
#ifndef DATA_PARSER_H
#define DATA_PARSER_H

#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_set>
#include "data_structures/Location.h"
#include "data_structures/UrbanMap.h"

template <typename T>
void createLocations(UrbanMap<T>* urban_map);

template <typename T>
void createRoads(UrbanMap<T>* urban_map);

template <typename T>
UrbanMap<T> createUrbanMap();


template <typename T>
void createLocations(UrbanMap<T> *urban_map) {
    std::ifstream is;
    std::string name, Code, line,tmp;
    int ID, parking;
    is.open("../data/LocationsTest_updated.csv");
    getline(is, line);
    while (getline(is, line)) {
        std::stringstream iss(line);

        std::getline(iss,name,',');
        std::getline(iss,tmp,',');
        ID = std::stoi(tmp);
        std::getline(iss,Code,',');
        std::getline(iss,tmp,',');
        parking = std::stoi(tmp);
        //create Location using unordered map
        if (parking == 1) {
            urban_map->setParkingNode(ID);
        }
        urban_map->addLocation(Code,ID,parking);
    }
}

template <typename T>
void createRoads(UrbanMap<T>* urban_map) {
    std::ifstream is;
    is.open("../data/DistancesTest_updated.csv");
    std::string code1, code2, line, tmp;
    int driving, walking;
    getline(is, line);
    while (getline(is, line)) {
        std::stringstream iss(line);
        std::getline(iss,code1,',');
        std::getline(iss,code2,',');
        std::getline(iss,tmp,',');
        if (tmp == "X") {
            driving = INT_MAX; //meaning that you cannot drive in this road
        }else {
            driving = std::stoi(tmp);
        }
        std::getline(iss,tmp,',');
        walking = std::stoi(tmp);
        urban_map->addBidirectionalRoad(code1,code2,driving,walking);
    }
}


template <typename T>
UrbanMap<T> createUrbanMap() {

    UrbanMap<T> urbanMap;
    createLocations(&urbanMap);
    createRoads(&urbanMap);

    return urbanMap;
}







#endif // DATA_PARSER_H
