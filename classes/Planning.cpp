//
// Created by joaop on 04/03/2025.
//

#include "Planning.h"
#include <iostream>
#include <algorithm>

bool relax(Edge<std::string> *edge, bool Drivemode) {
    if (Drivemode) {
        if (edge->getDest()->getDist() > edge->getDriving() + edge->getOrig()->getDist()) {
            edge->getDest()->setDist(edge->getDriving()+ edge->getOrig()->getDist());
            edge->getDest()->setPath(edge);
            return true;
        }
    }else {
        if (edge->getDest()->getDist() > edge->getWalking() + edge->getOrig()->getDist()) {
            edge->getDest()->setDist(edge->getWalking()+ edge->getOrig()->getDist());
            edge->getDest()->setPath(edge);
            return true;
        }
    }
    return false;
}


void dijkstra(UrbanMap<std::string>* g, const int &origin) {
    for (auto v : g->getLocationSet()) {
        v->setDist(INT_MAX);
        v->setPath(nullptr);
    }
    auto *s = g->getLocationSet()[origin-1];
    s->setDist(0);
    MutablePriorityQueue<Vertex<std::string>> pq;
    for (auto v : g->getLocationSet()) {
        pq.insert(v);
    }

    while (!pq.empty()) {
        auto u = pq.extractMin();
        if (u->isVisited())continue;    //if this vertex was visited in a past djkstra will not be included again
        for (auto e : u->getAdj()) {
            if (relax(e,g->DrivingModeEnabled()))
                pq.decreaseKey(e->getDest());
        }
    }
}

std::vector<int> getPath(UrbanMap<std::string> * g, const std::string &origin, const std::string &dest) {
    std::vector<int> res;
    res.push_back(g->findLocation(dest)->getID());
    Edge<std::string>* cur = g->findLocation(dest)->getPath();
    while (cur) {
        auto tmp = cur->getOrig();
        tmp->setVisited(true);
        res.push_back(tmp->getID());
        cur = tmp->getPath();
    }
    std::reverse(res.begin(), res.end());
    return res;
}