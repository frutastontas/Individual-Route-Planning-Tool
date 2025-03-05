//
// Created by joaop on 04/03/2025.
//

#ifndef ROUTEPLANNING_H
#define ROUTEPLANNING_H

#include "data_structures\UrbanMap.h"

void dijkstra(UrbanMap<std::string> * g, const int &origin);

std::vector<int> getPath(UrbanMap<std::string> * g, const std::string &origin, const std::string &dest);

#endif //ROUTEPLANNING_H
