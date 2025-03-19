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
#include <climits>

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


/**
 * @brief Implements Dijkstra's shortest path algorithm on an UrbanMap.
 *
 * This function initializes all locations with a maximum distance and then applies
 * Dijkstra's algorithm using a priority queue to find the shortest paths from the given origin.
 *
 * @param g Pointer to the UrbanMap graph containing locations and connections.
 * @param origin The index (1-based) of the starting location for Dijkstra's algorithm.
 *
 * @note The algorithm uses a mutable priority queue to efficiently retrieve the vertex with
 * the minimum distance, ensuring an optimal performance.
 *
 * @complexity The time complexity is **O((V + E) log V)**, where:
 *   - V is the number of locations (vertices) in the UrbanMap.
 *   - E is the number of connections (edges) between locations.
 *   - The `extractMin()` operation runs in **O(log V)**.
 *   - The relaxation step is performed **O(E)** times, each taking **O(log V)** in the worst case.
 */
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
            if (e->isSelected()) continue;  //avoid this segment if it was selected

            if (relax(e,g->DrivingModeEnabled()))
                pq.decreaseKey(e->getDest());
        }
    }
}


/**
 * @brief Retrieves the shortest path from an origin to a destination in an UrbanMap.
 *
 * This function reconstructs the shortest path by backtracking from the destination
 * to the origin using the `getPath()` pointers stored during the execution of Dijkstra’s algorithm.
 *
 * @param g Pointer to the UrbanMap graph containing locations and connections.
 * @param origin The name of the starting location.
 * @param dest The name of the destination location.
 * @return A vector of integers representing the sequence of location IDs along the shortest path.
 *
 * @note The function assumes that Dijkstra's algorithm has already been executed, and the `getPath()`
 * pointers are correctly set for each location.
 *
 * @complexity The time complexity is **O(V)** in the worst case, where:
 *   - Finding the destination vertex: **O(1)** (assuming efficient hashing or indexing).
 *   - Backtracking through the path: **O(V)** (in the worst case of a long path).
 *   - Reversing the result vector: **O(V)**.
 *   - Overall complexity: **O(V)**.
 */
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

/**
 * @brief computes the top-2 best routes to take from the source node to the destination on a specific mode
 *
 * This function uses two dijkstras, the first one to compute the best route, and the second one executes
 * after the intermediary nodes of the first were labelled, so that it does not include them in the dijkstra.
 *
 * @param urban_map is a pointer to the UrbanMap graph containing locations and connections.
 *
 * @note This function outputs to an output1.txt where the results can be seen.
 *
 * @complexity Does two dijkstras each one with complexity **O((V+E)logV)**
 * and also does two getPath() with complexity **O(V)**
 * This results in a final complexity of **O((V+E)logV)**
*/
void case1(UrbanMap<std::string>* urban_map) {
    std::ofstream out(outputDir + "output1.txt");
    if (!out) {
        std::cerr << "Error: Could not open output file!"<<" "<< strerror(errno) << ")" << std::endl;
        return ;
    }
    Case1Data case1_data;
    int src;
    int dest;
    std::cin>>src >> dest;
    auto Lsrc = urban_map->getLocationSet()[src-1];
    auto Ldest = urban_map->getLocationSet()[dest-1];
    dijkstra(urban_map, src);

    auto Lset = getPath(urban_map, Lsrc->getInfo(), Ldest->getInfo());

    Lsrc->setVisited(false);    //simply to reset for next iteration of dijkstra

    out<<"Source:"<<src<<std::endl;
    out<<"Destination:"<<dest<<std::endl;
    out<<"BestDrivingRoute:";
    if (Ldest->getPath() == nullptr) {
        out<<"none"<<std::endl;
    }else {
        for (int i = 0; i < Lset.size(); i++) {
            out<<Lset[i]<<",";
        }
        out <<" "<< Ldest->getDist() << std::endl;
    }


    dijkstra(urban_map, src);
    auto Lalt = getPath(urban_map, Lsrc->getInfo(), Ldest->getInfo());
    out<<"AlternativeDrivingRoute:";
    if (Ldest->getPath() == nullptr) {
        out<<"none"<<std::endl;
    }else {
        for (int i = 0; i < Lalt.size(); i++) {
            out<<Lalt[i]<<",";
        }
        out << Ldest->getDist() << std::endl;
    }
    out.close();
}

/**
 * @brief computes the best route that avoids certain segments, locations(vertex) and may
 * include a certain node in the path. Each node is deactivated using a boolean value *isVisited*
 * and if that value is true the dijkstra won't include that node in the path. Same thing goes for the
 * segments that also use a boolean value *isSelected*.
 *
 * This function uses one dijkstra if there is no Node to include, or uses two dijkstras, one from the source
 * to compute the distance to the node to include, and another starting from the included node to the destination.
 *
 * @param urban_map is a pointer to the UrbanMap graph containing locations and connections.
 *
 * @note This function outputs to an output2.txt where the results can be seen.
 *
 * @complexity Does two dijkstras each one with complexity **O((V+E)logV)**
 * or does only one dijkstra with the same complexity,
 * and also uses function getPath() with complexity **O(V)**
 * To deactivate the nodes it takes a traversal on a vector with ids **O(n)**
 * To deactivate the segments it takes a traversal on a vector with pairs of ids
 * and for the origin and destination it finds in the adjacency list the other node
 * of the segment, so the complexity is **O(n*|E|)**
 * This results in a final complexity of **O((V+E)logV), because the dijkstra dominates the complexity**
*/
void case2(UrbanMap<std::string>* urban_map) { //this is a hardcode solution for now
    Case2Data case2_data;
    int src;
    int dest;
    std::cin>>src >> dest;
    auto Lsrc = urban_map->getLocationSet()[src-1];
    auto Ldest = urban_map->getLocationSet()[dest-1];

    /////////////////////////////////////////////////////////////////////////////////////////////////////
    int avoid_nodes;
    int number_of_nodes;
    std::cin>>number_of_nodes;
    while (number_of_nodes >0) {    //this loop will avoid certain nodes
        std::cout<<"Please input the number of nodes to avoid:"<<std::endl;
        std::cin>>avoid_nodes;
        urban_map->getLocationSet()[avoid_nodes-1]->setVisited(true);
        number_of_nodes--;
    }

    std::vector<std::pair<int,int>> test_vector;
    //test_vector.push_back(std::make_pair(3,2));
    //test_vector.push_back(std::make_pair(7,8)); //test cases

    for (auto p : test_vector) {
        auto orig = urban_map->getLocationSet()[p.first-1];
        auto dest = urban_map->getLocationSet()[p.second-1];
        for (auto e :orig->getAdj()) {
            if (e->getDest()->getInfo() == dest->getInfo()) {
                e->setSelected(true);
                std::cout << "Edge eleminated"<<std::endl;
                break;
            }
        }
        for (auto e :dest->getAdj()) {
            if (e->getDest()->getInfo() == orig->getInfo()) {
                e->setSelected(true);
                std::cout << "Edge eleminated"<<std::endl;
                break;
            }
        }
    }
    ////////////////////////////////////////////////////////////////////////////////////////////
    int includenode;
    std::cout<<"IncludeNode: ";
    std::cin>>includenode;

    if (includenode == -1) {
        dijkstra(urban_map, src);
        auto Lset = getPath(urban_map, Lsrc->getInfo(), Ldest->getInfo());
        Lsrc->setVisited(false);
        if (Ldest->getPath() == nullptr) {
            std::cout<<"none"<<std::endl;
        }else {
            for (int i = 0; i < Lset.size(); i++) {
                std::cout<<Lset[i]<<",";
            }
            std::cout << Ldest->getDist() << std::endl;
        }
    }else {
        auto VertexInclude = urban_map->getLocationSet()[includenode-1];
        dijkstra(urban_map, src);   //primeiro dijkstra
        auto Lset = getPath(urban_map, Lsrc->getInfo(), VertexInclude->getInfo());  //encontrar caminho do src ate o include
        Lsrc->setVisited(false);
        int totaldistance = 0;
        if (VertexInclude->getPath() == nullptr) {
            std::cout<<"none"<<std::endl;
        }else {
            for (int i = 0; i < Lset.size(); i++) {
                std::cout<<Lset[i]<<",";
            }
            totaldistance += VertexInclude->getDist();
        }

        dijkstra(urban_map, includenode);       //segundo dijkstra do nó incluido para o destino
        Lset = getPath(urban_map, VertexInclude->getInfo(), Ldest->getInfo());
        if (Ldest->getPath() == nullptr) {
            std::cout<<"none"<<std::endl;
        }else {
            for (int i = 1; i < Lset.size(); i++) { //needs to start from zero to not repeat the included
                std::cout<<Lset[i]<<",";
            }
            totaldistance += Ldest->getDist();
        }
        std::cout<<totaldistance<<std::endl;
    }

}





std::vector<int> getPathEconomic(UrbanMap<std::string> * g, const std::string &origin, const std::string &dest) {
    std::vector<int> res;
    res.push_back(g->findLocation(dest)->getID());
    Edge<std::string>* cur = g->findLocation(dest)->getPath();
    while (cur) {
        auto tmp = cur->getOrig();
        res.push_back(tmp->getID());
        cur = tmp->getPath();
    }
    std::reverse(res.begin(), res.end());
    return res;
}



/**
 * @brief This function combines both walking and driving. First we do a dijkstra from the source to a node in the
 * parkingNodes vector of the graph, wich has all the ids of the nodes that have parking. Then we do a dijkstra from
 * that parking node to the destination using walking distances, making sure that the resulting walking time
 * does not exceed the maximum. We do this for every parking node and we get the best overall time.
 *
 *
 * @param urban_map is a pointer to the UrbanMap graph containing locations and connections.
 *
 * @note This function outputs to an output3.txt where the results can be seen.
 *
 * @complexity Does two dijkstras each one with complexity **O((V+E)logV)**
 * for each node in the parkingNodes vector of the graph, this results in **O(V(V+E)logV)**,
 * and also uses function getPath() with complexity **O(V)**
 * To deactivate the nodes it takes a traversal on a vector with ids **O(n)**
 * To deactivate the segments it takes a traversal on a vector with pairs of ids
 * and for the origin and destination it finds in the adjacency list the other node
 * of the segment, so the complexity is **O(n*|E|)**
 * This results in a final complexity of **O(V(V+E)logV)**
*/
void case3(UrbanMap<std::string>* urban_map) {

    int src;
    int dest;
    std::cin>>src >> dest;
    auto Lsrc = urban_map->getLocationSet()[src-1];
    auto Ldest = urban_map->getLocationSet()[dest-1];

    //////////////////////////////////////////////////////////////////////////////////////////////////
    ///
    int avoid_nodes;
    int number_of_nodes;
    std::cin>>number_of_nodes;
    while (number_of_nodes >0) {    //this loop will avoid certain nodes
        std::cin>>avoid_nodes;
        urban_map->getLocationSet()[avoid_nodes-1]->setVisited(true);
        number_of_nodes--;
    }
    std::vector<std::pair<int,int>> test_vector;
    //test_vector.push_back(std::make_pair(3,6));
    //test_vector.push_back(std::make_pair(6,7)); //test cases

    for (auto p : test_vector) {
        auto orig = urban_map->getLocationSet()[p.first-1];
        auto dest = urban_map->getLocationSet()[p.second-1];
        for (auto e :orig->getAdj()) {
            if (e->getDest()->getInfo() == dest->getInfo()) {
                e->setSelected(true);
                std::cout << "Edge eleminated"<<std::endl;
                break;
            }
        }
        for (auto e :dest->getAdj()) {
            if (e->getDest()->getInfo() == orig->getInfo()) {
                e->setSelected(true);
                std::cout << "Edge eleminated"<<std::endl;
                break;
            }
        }
    }
    ////////////////////////////////////////////////////////////////////////////////////////////////
    ///
    int parkingNodeID;    //will display the ID of the node where the car will be parked
    int bestDrivingTime = INT_MAX;
    int bestWalkingTime = 0;    //this is to prevent overflow
    int maxWalkingtime = 18;
    std::vector<int> bestDrivingRoute;
    std::vector<int> bestWalkingRoute;

    for (auto i : urban_map->getParkingNodes()) {
        std::cout<<i<<std::endl;
        urban_map->setDrivingMode(true);

        dijkstra(urban_map, src);

        Vertex<std::string>* parkingNode = urban_map->getLocationSet()[i-1];

        auto driverouteTMP = getPathEconomic(urban_map, Lsrc->getInfo(), parkingNode->getInfo());
        int distDriving = parkingNode->getDist();

        urban_map->setDrivingMode(false);


        // now do a dijkstra from the parking node to the dest and make sure it does not pass the maxWalk time
        dijkstra(urban_map, i);
        int walktime =Ldest->getDist();
        auto walkrouteTMP = getPathEconomic(urban_map, parkingNode->getInfo(), Ldest->getInfo());

        if (walktime <= maxWalkingtime) {
            int finaldistance = distDriving+walktime;
            if (finaldistance < bestDrivingTime+bestWalkingTime) {
                bestDrivingTime = distDriving;  //update the best driving time
                bestWalkingTime = walktime;     //update the best walking time
                parkingNodeID =  i;
                bestDrivingRoute = driverouteTMP;
                bestWalkingRoute = walkrouteTMP;
            }else if (finaldistance == bestDrivingTime+bestWalkingTime) { //if equal than select the one with the best walking time(most)
                if (walktime>bestWalkingTime) {
                    bestDrivingTime = distDriving;  //update the best driving time
                    bestWalkingTime = walktime;     //update the best walking time
                    parkingNodeID =  i;
                    bestDrivingRoute = driverouteTMP;
                    bestWalkingRoute = walkrouteTMP;
                }
            }
        }
    }
    std::cout<<parkingNodeID<<std::endl;
    for (auto i : bestDrivingRoute) {
        std::cout<<i<<",";
    }
    std::cout<<std::endl;
    for (auto i : bestWalkingRoute) {
        std::cout<<i<<",";
    }
    std::cout<<std::endl;
}


