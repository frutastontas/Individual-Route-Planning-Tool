//
// Created by joaop on 01/03/2025.
//


#include <fstream>
#include <sstream>

#include "data_structures/UrbanMap.h"

template <typename T>
UrbanMap<T> createUrbanMap() {
    UrbanMap<T> urbanMap;

}

template <typename T>
void createLocations(UrbanMap<T> urban_map) {
    std::ifstream is;
    std::string location, Code, line,tmp;
    int ID, parking;
    is.open("../data/Locations.csv");
    getline(is, line);
    while (getline(is, line)) {
        std::stringstream iss(line);
        std::getline(iss,location,',');
        std::getline(iss,Code,',');
        std::getline(iss,tmp,',');
        ID = std::stoi(tmp);
        std::getline(iss,tmp,',');
        parking = std::stoi(tmp);
        Vertex<T> *vertex = Vertex<T>();
        vertex->setID(ID);
        vertex->setParking(parking);
        vertex->setCode(Code);
        urban_map.addLocation(vertex);
    }
}


