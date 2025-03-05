//
// Created by joaop on 04/03/2025.
//

#include "Planning.h"
#include <algorithm>

bool relax(Edge<std::string> *edge) {
    if (edge->getDest()->getDist() > edge->getDriving() + edge->getOrig()->getDist()) { //For now this only works for driving have to change
        edge->getDest()->setDist(edge->getDriving()+ edge->getOrig()->getDist());
        edge->getDest()->setPath(edge);
        return true;
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
        if (!v->isVisited())    //this makes it so if the intermediary node was visited in a previous djkstra it's not included
            pq.insert(v);
    }

    while (!pq.empty()) {
        auto u = pq.extractMin();
        for (auto e : u->getAdj()) {
            if (relax(e))
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
    g->findLocation(origin)->setVisited(false);
    g->findLocation(dest)->setVisited(false);
    std::reverse(res.begin(), res.end());
    return res;
}