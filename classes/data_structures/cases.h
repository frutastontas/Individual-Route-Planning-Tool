//
// Created by joaop on 12/03/2025.
// lala

#ifndef CASES_H
#define CASES_H

#include <string>
#include <vector>

// Struct for storing case 1 information
struct Case1Data {
    bool driving;
    int src;
    int dest;
};

// Struct for storing case 2 information
struct Case2Data {
    bool driving;
    int src;
    int dest;
    std::vector<int> avoid_nodes;
    std::vector<std::pair<int,int>> avoid_edges;
    int include_node;
};

 // Struct for storing case 3 information (7.)
struct Case3Data {
    bool driving;
    bool drivingwalking;
    bool walking;
    // string mode;
    int src;
    int dest;
    int maxWalkTime;
    std::vector<int> avoid_nodes;
    std::vector<std::pair<int,int>> avoid_edges;
 };
#endif //CASES_H
