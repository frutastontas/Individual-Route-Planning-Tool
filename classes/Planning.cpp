//
// Created by joaop on 04/03/2025.
//

#include "Planning.h"
#include <iostream>
#include <algorithm>
#include <ostream>
#include <cerrno>
#include <cstring>
#include <fstream>

const std::string outputDir = "C:/Users/joaop/Desktop/DA_proj1/Individual-Route-Planning-Tool/classes/output/";

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



void case1(UrbanMap<std::string> urban_map) {
    std::ofstream out(outputDir + "output1.txt");
    if (!out) {
        std::cerr << "Error: Could not open output file!"<<" "<< strerror(errno) << ")" << std::endl;
        return ;
    }
    int src;
    int dest;
    std::cin>>src >> dest;
    auto Lsrc = urban_map.getLocationSet()[src-1];
    auto Ldest = urban_map.getLocationSet()[dest-1];
    dijkstra(&urban_map, src);
    auto Lset = getPath(&urban_map, Lsrc->getInfo(), Ldest->getInfo());
    Lsrc->setVisited(false);
    if (Ldest->getPath() == nullptr) {
        out<<"none"<<std::endl;
    }else {
        for (int i = 0; i < Lset.size(); i++) {
            out<<Lset[i]<<" ";
        }
        out << Ldest->getDist() << std::endl;
    }


    dijkstra(&urban_map, src);
    auto Lalt = getPath(&urban_map, Lsrc->getInfo(), Ldest->getInfo());

    if (Ldest->getPath() == nullptr) {
        out<<"none"<<std::endl;
    }else {
        for (int i = 0; i < Lalt.size(); i++) {
            out<<Lalt[i]<<" ";
        }
        out << Ldest->getDist() << std::endl;
    }
    out.close();
}

void case2(UrbanMap<std::string> urban_map) { //this is a hardcode solution for now
    int src;
    int dest;
    std::cin>>src >> dest;
    auto Lsrc = urban_map.getLocationSet()[src-1];
    auto Ldest = urban_map.getLocationSet()[dest-1];
    int avoid_nodes;
    int number_of_nodes;
    std::cin>>number_of_nodes;
    while (number_of_nodes >0) {    //this loop will avoid certain nodes
        std::cin>>avoid_nodes;
        urban_map.getLocationSet()[avoid_nodes-1]->setVisited(true);
        number_of_nodes--;
    }

    dijkstra(&urban_map, src);
    auto Lset = getPath(&urban_map, Lsrc->getInfo(), Ldest->getInfo());
    Lsrc->setVisited(false);
    if (Ldest->getPath() == nullptr) {
        std::cout<<"none"<<std::endl;
    }else {
        for (int i = 0; i < Lset.size(); i++) {
            std::cout<<Lset[i]<<" ";
        }
        std::cout << Ldest->getDist() << std::endl;
    }
}


