//
// Created by joaop on 12/03/2025.
//

#ifndef CASES_H
#define CASES_H

#include <string>
#include <vector>

// Struct for storing case 1 information
struct Case1Data {
    int src;
    int dest;
};

// Struct for storing case 2 information
struct Case2Data {
    int src;
    int dest;
    std::vector<int> avoid_nodes;
    std::vector<std::pair<int,int>> avoid_edges;
    int include_node;
};

#endif //CASES_H
