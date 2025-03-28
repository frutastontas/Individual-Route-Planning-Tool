//
// Created by joaop on 04/03/2025.
//
#ifndef DATA_PARSER_H
#define DATA_PARSER_H

#include <fstream> // Read files
#include <iostream>
#include <sstream>
#include <unordered_set>
#include <climits>
#include "data_structures/UrbanMap.h"
#include "data_structures/cases.h"

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
    is.open("../data/LocationsTest.csv");
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
    is.open("../data/DistancesTest.csv");
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

inline Case1Data getCase1() {
    // Input: mode, src, dest >> Data structure: data1
    Case1Data data1; // Create structure to store data

    std::ifstream input1("../input/input.txt"); // Open file
    std::string line; // Line to be read

    // Error opening file
    if (!input1) {
        std::cerr << "Error opening file!" << std::endl;
    }

    // Reading the file (Case 1: mode, src, dest)
    int n = 0;
    while (std::getline(input1, line)) { // n < 3
        size_t pos = line.find(':');
        std::string value = line.substr(pos + 1); // Read ':' onwards

        if (n == 0) {
            if (value == "driving") data1.driving = true;
            else
                data1.driving = false;
        }
        else if (n == 1) data1.src = stoi(value);
        else if (n == 2) data1.dest = stoi(value);
        else break;
        n++;
    }

    input1.close(); // Close file

    return data1;
}

inline Case2Data getCase2() {
    Case2Data data2;  // Criar estrutura para armazenar os dados

    std::ifstream input2("../input/input.txt");
    if (!input2) {
        std::cerr << "Error opening file!" << std::endl;
        return data2;  // Retorna estrutura vazia se não conseguir abrir o ficheiro
    }

    std::string line;
    int n = 0;

    while (std::getline(input2, line)) {
        size_t pos = line.find(':');
        if (pos == std::string::npos) continue;  // Ignorar linhas inválidas

        std::string value = line.substr(pos + 1);
        while (!value.empty() && (value.front() == ' ' || value.front() == '\t')) {
            value.erase(value.begin());  // Remover espaços no início
        }

        if (n == 0) {
            data2.driving = (value == "driving");
        }
        else if (n == 1) {
            data2.src = std::stoi(value);
        }
        else if (n == 2) {
            data2.dest = std::stoi(value);
        }
        else if (n == 3) {
            std::stringstream ss(value);
            std::string token;
            while (std::getline(ss, token, ',')) {
                data2.avoid_nodes.push_back(std::stoi(token));
            }
        }
        else if (n == 4) {
            std::stringstream ss(value);
            std::string token;
            while (std::getline(ss, token, ')')) {
                size_t start = token.find('(');
                if (start != std::string::npos) {
                    std::string pair = token.substr(start + 1);
                    std::stringstream pairStream(pair);
                    std::string n1, n2;

                    if (std::getline(pairStream, n1, ',') && std::getline(pairStream, n2, ',')) {
                        data2.avoid_edges.emplace_back(std::stoi(n1), std::stoi(n2));
                    }
                }
            }
        }
        else if (n == 5) {
            if (!value.empty()) {
                data2.include_node = std::stoi(value);
            } else {
                data2.include_node = -1;  // Definir um valor padrão caso esteja vazio
            }
        }
        else {
            break;
        }

        n++;
    }

    input2.close();
    return data2;
}

inline Case3Data getCase3() {
    Case3Data data3;  // Criar estrutura para armazenar os dados

    std::ifstream input3("../input/input.txt");  // Abrir ficheiro
    if (!input3) {
        std::cerr << "Error opening file!" << std::endl;
        return data3;  // Retorna estrutura vazia
    }

    std::string line;
    int n = 0;

    while (std::getline(input3, line)) {
        size_t pos = line.find(':');
        if (pos == std::string::npos) continue;  // Ignorar linhas inválidas

        std::string value = line.substr(pos + 1);
        while (!value.empty() && (value.front() == ' ' || value.front() == '\t')) {
            value.erase(value.begin());  // Remover espaços extras
        }

        if (n == 0) {  // Mode
            if (value == "driving-walking") {
                data3.drivingwalking = true;
                data3.driving = false;
                data3.walking = false;
            }
            else if (value == "driving") {
                data3.drivingwalking = false;
                data3.driving = true;
                data3.walking = false;
            }
            else {  // walking
                data3.drivingwalking = false;
                data3.driving = false;
                data3.walking = true;
            }
        }
        else if (n == 1) {
            data3.src = std::stoi(value);
        }
        else if (n == 2) {
            data3.dest = std::stoi(value);
        }
        else if (n == 3) {
            data3.maxWalkTime = std::stoi(value);
        }
        else if (n == 4) {  // AvoidNodes
            if (!value.empty()) {
                std::stringstream ss(value);
                std::string token;
                while (std::getline(ss, token, ',')) {
                    data3.avoid_nodes.push_back(std::stoi(token));
                }
            }
        }
        else if (n == 5) {  // AvoidSegments
            if (!value.empty()) {
                std::stringstream ss(value);
                std::string token;
                while (std::getline(ss, token, ')')) {
                    size_t start = token.find('(');
                    if (start != std::string::npos) {
                        std::string pair = token.substr(start + 1);
                        std::stringstream pairStream(pair);
                        std::string n1, n2;

                        if (std::getline(pairStream, n1, ',') && std::getline(pairStream, n2, ',')) {
                            data3.avoid_edges.emplace_back(std::stoi(n1), std::stoi(n2));
                        }
                    }
                }
            }
        }
        else {
            break;
        }

        n++;
    }

    input3.close();
    return data3;
}

#endif // DATA_PARSER_H
