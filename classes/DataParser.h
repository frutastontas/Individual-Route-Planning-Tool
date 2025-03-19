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
#include "data_structures/Location.h"
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
        Location location = Location(name,Code,ID,parking);
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

Case1Data getCase1() {
    // Input: mode, src, dest >> Data structure: data1
    Case1Data data1; // Create structure to store data

    std::ifstream input1("input.txt"); // Open file
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

Case2Data getCase2() {
    // Input: mode, src, dest >> Data structure: data1
    Case2Data data2; // Create structure to store data

    std::ifstream input2("input.txt"); // Open file
    std::string line; // Line to be read

    // Error opening file
    if (!input2) {
        std::cerr << "Error opening file!" << std::endl;
    }

    // Reading the file (Case 2: mode, src, dest, avoid_nodes, avoid_edges, include)
    int n = 0;
    while (std::getline(input2, line)) { // n < 6

        size_t pos = line.find(':');
        std::string value = line.substr(pos + 1); // Read ':' onwards

        if (n == 0) {
            if (value == "driving") data2.driving = true;
            data2.driving = false;
        }
        else if (n == 1) data2.src = stoi(value);
        else if (n == 2) data2.dest = stoi(value);
        else if (n == 3) {
            // ' ', or list of int: 1,2,3,4 >> vector<int>
            std::vector<int> nodes;
            std::stringstream ss(value);
            std::string token;
            while (std::getline(ss, token, ',')) {
                nodes.push_back(std::stoi(token));
            }
            for (int i = 0; i < nodes.size(); i++) {
                data2.avoid_nodes[i] = nodes[i];
            }
        }
        else if (n == 4) {
            // ' ', or list of pairs: (1,2),(3,4) >> vector< pair<int, int> >
            std::vector< std::pair<int, int>> edges;
            std::stringstream ss(value);
            std::string token;

            while (std::getline(ss, token, ')')) { // (int,int
                size_t start = token.find('(');
                std::string pair = token.substr(start + 1); // int,int
                std::stringstream pairStream(pair);
                std::string n1, n2;

                if (std::getline(pairStream, n1, ',') and std::getline(pairStream, n2, ',')) {
                    edges.emplace_back(std::stoi(n1), std::stoi(n2)); // store as int
                }
            }
            for (int i = 0; i < edges.size(); i++) {
                data2.avoid_edges[i] = edges[i];
            }
        }//
        else if (n == 5) data2.include_node = stoi(value);
        else break;
        n++;
    }

    input2.close(); // Close file

    return data2;
}

Case3Data getCase3() {
    // Input: mode, src, dest >> Data structure: data1
    Case3Data data3; // Create structure to store data

    std::ifstream input3("input.txt"); // Open file
    std::string line; // Line to be read

    // Error opening file
    if (!input3) {
        std::cerr << "Error opening file!" << std::endl;
    }

    // Reading the file (Case 3: mode, src, dest, max, avoid_nodes, avoid_edges)
    int n = 0;
    while (std::getline(input3, line)) { // n < 6

        size_t pos = line.find(':');
        std::string value = line.substr(pos + 1); // Read ':' onwards

        if (n == 0) { // Mode
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
            else { // walking
                data3.drivingwalking = false;
                data3.driving = false;
                data3.walking = true;
            }
        }
        else if (n == 1) data3.src = stoi(value); // Source
        else if (n == 2) data3.dest = stoi(value); // Destination
        else if (n == 3) data3.maxWalkTime = stoi(value); // MaxWalkTime
        else if (n == 4) { // AvoidNodes
            // ' ', or list of int: 1,2,3,4 >> vector<int>
            std::vector<int> nodes;
            std::stringstream ss(value);
            std::string token;
            while (std::getline(ss, token, ',')) {
                nodes.push_back(std::stoi(token));
            }
            for (int i = 0; i < nodes.size(); i++) {
                data3.avoid_nodes[i] = nodes[i];
            }
        }
        else if (n == 5) { // AvoidSegments
            // ' ', or list of pairs: (1,2),(3,4) >> vector< pair<int, int> >
            std::vector< std::pair<int, int>> edges;
            std::stringstream ss(value);
            std::string token;

            while (std::getline(ss, token, ')')) { // (int,int
                size_t start = token.find('(');
                std::string pair = token.substr(start + 1); // int,int
                std::stringstream pairStream(pair);
                std::string n1, n2;

                if (std::getline(pairStream, n1, ',') and std::getline(pairStream, n2, ',')) {
                    edges.emplace_back(std::stoi(n1), std::stoi(n2)); // store as int
                }
            }
            for (int i = 0; i < edges.size(); i++) {
                data3.avoid_edges[i] = edges[i];
            }
        }
        else break;
        n++;
    }

    input3.close(); // Close file

    return data3;
}

#endif // DATA_PARSER_H
