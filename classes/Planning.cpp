//
// Created by joaop on 04/03/2025.
//

#include "Planning.h"


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