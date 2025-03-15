//
// Created by joaop on 04/03/2025.
//

#ifndef ROUTEPLANNING_H
#define ROUTEPLANNING_H

#include "data_structures\UrbanMap.h"
#include "data_structures/cases.h"

void dijkstra(UrbanMap<std::string> * g, const int &origin);

std::vector<int> getPath(UrbanMap<std::string> * g, const std::string &origin, const std::string &dest);

void case1(UrbanMap<std::string>* urban_map);

void case2(UrbanMap<std::string>* urban_map);

void case3(UrbanMap<std::string>* urban_map);

#endif //ROUTEPLANNING_H
