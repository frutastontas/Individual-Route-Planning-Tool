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

#include "DataParser.h"
#include "data_structures/cases.h"


/**
 * @brief Performs edge relaxation for a graph algorithm.
 *
 * This function updates the distance of the destination node of the given edge
 * if a shorter path is found using either driving or walking mode.
 *
 * @tparam T The type of the data stored in the graph nodes (e.g., std::string).
 * @param edge A pointer to the edge that is being relaxed.
 * @param Drivemode A boolean flag indicating the mode of transportation.
 *                  - If true, driving time is used.
 *                  - If false, walking time is used.
 *
 * @return True if the relaxation was successful (i.e., the distance was updated), false otherwise.
 */
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
 * @brief computes the top-2 best routes to take from the source node to the destination on a specific mode.
 *
 * This function uses two dijkstras, the first one to compute the best route, and the second one executes
 * after the intermediary nodes of the first were labelled, so that it does not include them in the dijkstra.
 *
 * @param urban_map is a pointer to the UrbanMap graph containing locations and connections.
 *
 * @note This function outputs to an output.txt where the results can be seen.
 *
 * @complexity Does two dijkstras each one with complexity **O((V+E)logV)**
 * and also does two getPath() with complexity **O(V)**
 * This results in a final complexity of **O((V+E)logV)**
*/
void case1(UrbanMap<std::string>* urban_map) {
    std::ofstream out( "../output/output.txt");
    if (!out) {
        std::cerr << "Error: Could not open output file!"<<" "<< strerror(errno) << ")" << std::endl;
        return;
    }
    Case1Data case1_data = getCase1();
    urban_map->setDrivingMode(case1_data.driving);
    int src = case1_data.src;
    int dest = case1_data.dest;
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
            out<<Lset[i];
            if (i< Lset.size()-1) out<<",";
        }
        out <<"("<< Ldest->getDist()<<")"<< std::endl;
    }


    dijkstra(urban_map, src);
    auto Lalt = getPath(urban_map, Lsrc->getInfo(), Ldest->getInfo());
    out<<"AlternativeDrivingRoute:";
    if (Ldest->getPath() == nullptr) {
        out<<"none"<<std::endl;
    }else {
        for (int i = 0; i < Lalt.size(); i++) {
            out<<Lalt[i];
            if (i<Lalt.size()-1) out<<",";
        }
        out << "("<<Ldest->getDist()<<")" << std::endl;
    }
    out.close();
}

/**
 * @brief Computes the best route that avoids certain segments, locations(vertex) and may
 * include a certain node in the path. Each node is deactivated using a boolean value *isVisited*
 * and if that value is true the dijkstra won't include that node in the path. Same thing goes for the
 * segments that also use a boolean value *isSelected*.
 *
 * This function uses one dijkstra if there is no Node to include, or uses two dijkstras, one from the source
 * to compute the distance to the node to include, and another starting from the included node to the destination.
 *
 * @param urban_map is a pointer to the UrbanMap graph containing locations and connections.
 *
 * @note This function outputs to an output.txt where the results can be seen.
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
void case2(UrbanMap<std::string>* urban_map) {
    std::ofstream out( "../output/output.txt");
    if (!out) {
        std::cerr << "Error: Could not open output file!"<<" "<< strerror(errno) << ")" << std::endl;
        return;
    }
    Case2Data case2_data;
    try {
        case2_data = getCase2();
    }catch (...) {
        std::cerr << "Error: Check the input file format" << std::endl;
    }

    int src = case2_data.src;
    int dest = case2_data.dest;
    out<<"Source:"<<src<<std::endl;
    out<<"Destination:"<<dest<<std::endl;
    auto Lsrc = urban_map->getLocationSet()[src-1];
    auto Ldest = urban_map->getLocationSet()[dest-1];

    urban_map->setDrivingMode(case2_data.driving);

    /////////////////////////////////////////////////////////////////////////////////////////////////////
    std::vector<int> avoid_nodes = case2_data.avoid_nodes;
    for (auto a : avoid_nodes) {    //this loop will avoid certain nodes
        urban_map->getLocationSet()[a-1]->setVisited(true);
    }

    std::vector<std::pair<int,int>> avoidEdges = case2_data.avoid_edges;

    for (auto p : avoidEdges) {
        auto orig = urban_map->getLocationSet()[p.first-1];
        auto dest = urban_map->getLocationSet()[p.second-1];
        for (auto e :orig->getAdj()) {
            if (e->getDest()->getInfo() == dest->getInfo()) {
                e->setSelected(true);
                break;
            }
        }
        for (auto e :dest->getAdj()) {
            if (e->getDest()->getInfo() == orig->getInfo()) {
                e->setSelected(true);
                break;
            }
        }
    }
    ////////////////////////////////////////////////////////////////////////////////////////////
    ///
    out<<"RestrictedDrivingRoute:";
    int includenode = case2_data.include_node;

    if (includenode == -1) {
        dijkstra(urban_map, src);
        auto Lset = getPath(urban_map, Lsrc->getInfo(), Ldest->getInfo());
        Lsrc->setVisited(false);
        if (Ldest->getPath() == nullptr) {
            out<<"none"<<std::endl;
        }else {
            for (int i = 0; i < Lset.size(); i++) {
                out<<Lset[i];
                if (i<Lset.size()-1) out<<",";
            }
            out <<"("<<Ldest->getDist()<< ")"<< std::endl;
        }
    }else {
        auto VertexInclude = urban_map->getLocationSet()[includenode-1];
        dijkstra(urban_map, src);   //primeiro dijkstra
        auto Lset = getPath(urban_map, Lsrc->getInfo(), VertexInclude->getInfo());  //encontrar caminho do src ate o include
        Lsrc->setVisited(false);
        int totaldistance = 0;
        if (VertexInclude->getPath() == nullptr) {
            out<<"none"<<std::endl;
            out.close();
            return;
        }else {
            for (int i = 0; i < Lset.size(); i++) {
                out<<Lset[i];
                if (i<Lset.size()-1) out<<",";
            }
            totaldistance += VertexInclude->getDist();
        }

        dijkstra(urban_map, includenode);       //segundo dijkstra do nó incluido para o destino
        Lset = getPath(urban_map, VertexInclude->getInfo(), Ldest->getInfo());
        if (Ldest->getPath() == nullptr) {
            out<<"none"<<std::endl;
        }else {
            out<<",";
            for (int i = 1; i < Lset.size(); i++) { //needs to start from zero to not repeat the included
                out<<Lset[i];
                if (i<Lset.size()-1) out<<",";
            }
            totaldistance += Ldest->getDist();
        }
        out<<"("<<totaldistance<<")"<<std::endl;
    }
    out.close();
}




//Variation that does not deactivate any nodes
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
 * @brief this sruct will be used for case4, where we estimate other paths, by finding the two other best options
 * and displaying them. This will store the specific parking node that the route includes, and also the routes taken.
 */
struct RouteOption {
    int parkingNodeID;
    int drivingTime;
    int walkingTime;
    std::vector<int> drivingRoute;
    std::vector<int> walkingRoute;

    // Define a comparison operator for sorting
    bool operator<(const RouteOption& other) const {
        int totalTime = drivingTime + walkingTime;
        int otherTotalTime = other.drivingTime + other.walkingTime;

        // Primary sorting criterion: lowest total time
        if (totalTime != otherTotalTime) {
            return totalTime < otherTotalTime;
        }

        // Secondary criterion: favoring more walking time
        return walkingTime > other.walkingTime;
    }
};


void estimation(std::vector<RouteOption> &routeOptions,int src, int dest);

/**
 * @brief This function combines both walking and driving. First we do a dijkstra from the source to a node in the
 * parkingNodes vector of the graph, wich has all the ids of the nodes that have parking. Then we do a dijkstra from
 * that parking node to the destination using walking distances, making sure that the resulting walking time
 * does not exceed the maximum. We do this for every parking node and we get the best overall time.
 * In case two paths have the same total time, we select the one with more walking time to be more economic.
 *
 *
 * @param urban_map is a pointer to the UrbanMap graph containing locations and connections.
 *
 * @note This function outputs to an output.txt where the results can be seen.
 *
 * @return returns an integer that acts as a flag to check if estimation function was called
 * (0 means yes, 1 means no)
 *
 * @complexity Does two dijkstras each one with complexity **O((V+E)logV)**
 * for each node in the parkingNodes vector of the graph, this results in **O(V(V+E)logV)**,
 * and also uses function getPath() with complexity **O(V)**
 * To deactivate the nodes it takes a traversal on a vector with ids **O(n)**
 * To deactivate the segments it takes a traversal on a vector with pairs of ids
 * and for the origin and destination it finds in the adjacency list the other node
 * of the segment, so the complexity is **O(n*|E|)**
 * This results in a final complexity of **O(V(V+E)logV)**
 *
 * When it comes to spacial complexity, since we compute each route to the parking node and then
 * to the destination and we compute the path and save it on a vector, this results in @complexity
 * **O(n)**.
*/
int case3(UrbanMap<std::string>* urban_map) {
    std::ofstream out( "../output/output.txt");
    if (!out) {
        std::cerr << "Error: Could not open output file!"<<" "<< strerror(errno) << ")" << std::endl;
        return 1;
    }
    Case3Data case3_data = getCase3();
    int src = case3_data.src;
    int dest = case3_data.dest;
    out<<"Source: "<<src<<std::endl;
    out<<"Destination: "<<dest<<std::endl;
    auto Lsrc = urban_map->getLocationSet()[src-1];
    auto Ldest = urban_map->getLocationSet()[dest-1];

    //////////////////////////////////////////////////////////////////////////////////////////////////
    ///

    std::vector<int> avoid_nodes = case3_data.avoid_nodes;
    for (auto a : avoid_nodes) {    //this loop will avoid certain nodes
        urban_map->getLocationSet()[a-1]->setVisited(true);
    }

    std::vector<std::pair<int,int>> avoidEdges = case3_data.avoid_edges;

    for (auto p : avoidEdges) {
        auto orig = urban_map->getLocationSet()[p.first-1];
        auto dest = urban_map->getLocationSet()[p.second-1];
        for (auto e :orig->getAdj()) {
            if (e->getDest()->getInfo() == dest->getInfo()) {
                e->setSelected(true);
                break;
            }
        }
        for (auto e :dest->getAdj()) {
            if (e->getDest()->getInfo() == orig->getInfo()) {
                e->setSelected(true);
                break;
            }
        }
    }
    ////////////////////////////////////////////////////////////////////////////////////////////////
    ///
    int parkingNodeID =  NULL;    //will display the ID of the node where the car will be parked
    int bestDrivingTime = INT_MAX;
    int bestWalkingTime = 0;    //this is to prevent overflow
    int maxWalkingtime = case3_data.maxWalkTime;
    std::vector<int> bestDrivingRoute;
    std::vector<int> bestWalkingRoute;
    std::vector<RouteOption> routeOptions;  //list all the available options

    for (auto i : urban_map->getParkingNodes()) {
        urban_map->setDrivingMode(true);

        dijkstra(urban_map, src);

        Vertex<std::string>* parkingNode = urban_map->getLocationSet()[i-1];

        auto driverouteTMP = getPathEconomic(urban_map, Lsrc->getInfo(), parkingNode->getInfo());
        int distDriving = parkingNode->getDist();

        if (distDriving == INT_MAX) {
            continue;
        }
        urban_map->setDrivingMode(false);


        // now do a dijkstra from the parking node to the dest and make sure it does not pass the maxWalk time
        dijkstra(urban_map, i);
        int walktime =Ldest->getDist();
        auto walkrouteTMP = getPathEconomic(urban_map, parkingNode->getInfo(), Ldest->getInfo());

        routeOptions.push_back
            ({i,distDriving,walktime, driverouteTMP,walkrouteTMP});

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
    if (parkingNodeID == NULL) {
        out << "DrivingRoute:" << std::endl;
        out << "ParkingNode:" << std::endl;
        out << "WalkingRoute:" << std::endl;
        out << "TotalTime:" << std::endl;
        out << "Message:No possible route with max. walking time of " << maxWalkingtime << " minutes." << std::endl;
        out.close();
        estimation(routeOptions, src,dest);
        return 0;
    }
    // Driving Route
    out << "DrivingRoute:";
    for (size_t i = 0; i < bestDrivingRoute.size(); ++i) {
        out << bestDrivingRoute[i];
        if (i < bestDrivingRoute.size() - 1) out << ",";
    }
    out << "(" << bestDrivingTime << ")" << std::endl;

    // Parking Node
    out << "ParkingNode:" << parkingNodeID << std::endl;

    // Walking Route
    out << "WalkingRoute:";
    for (size_t i = 0; i < bestWalkingRoute.size(); ++i) {
        out << bestWalkingRoute[i];
        if (i < bestWalkingRoute.size() - 1) out << ",";
    }
    out << "(" << bestWalkingTime << ")" << std::endl;

    // Total Time
    out << "TotalTime:" << (bestDrivingTime + bestWalkingTime) << std::endl;
    out.close();
    return 1;
}

/**
 * @brief This function will select the best two options that combine driving and walking, passing through
 * a parking node. It will sort a vector that houses structs **RouteOption** that store previously computed
 * routes of case 3. This estimation will only appear if there is no path from case 3. It selects the best
 * two and puts them in the file.
 * 
 * @param routeOptions vector of struct that houses information of a specific route to the destination.
 *
 * @complexity The complexity is dominated by the sorting step since everything else is a constant
 * time operation. This results in complexity **O(nlog(n))**.
 */
void estimation(std::vector<RouteOption> &routeOptions, int src, int dest) {
    if (routeOptions.empty()) {
        std::ofstream out("../output/estimation.txt");
        if (!out) {
            std::cerr << "Error: Could not open output file! (" << strerror(errno) << ")" << std::endl;
            return;
        }

        out << "Message: No valid alternative routes found." << std::endl;
        out.close();
        return;
    }

    std::ofstream out("../output/estimation.txt");
    if (!out) {
        std::cerr << "Error: Could not open output file! (" << strerror(errno) << ")" << std::endl;
        return;
    }
    out<<"Source:"<<src<<std::endl;
    out<<"Destination:"<<dest<<std::endl;
    // Sort based on the custom comparator in RouteOption
    std::sort(routeOptions.begin(), routeOptions.end());

    // Get the top two options
    int numOptions = std::min(2, (int)routeOptions.size());

    for (int i = 0; i < numOptions; ++i) {
        const auto& option = routeOptions[i];

        // Driving Route
        out << "DrivingRoute" << i + 1 << ":";
        for (size_t j = 0; j < option.drivingRoute.size(); ++j) {
            out << option.drivingRoute[j];
            if (j < option.drivingRoute.size() - 1) out << ",";
        }
        out << "(" << option.drivingTime << ")\n";

        // Parking Node
        out << "ParkingNode" << i + 1 << ":" << option.parkingNodeID << "\n";

        // Walking Route
        out << "WalkingRoute" << i + 1 << ":";
        for (size_t j = 0; j < option.walkingRoute.size(); ++j) {
            out << option.walkingRoute[j];
            if (j < option.walkingRoute.size() - 1) out << ",";
        }
        out << "(" << option.walkingTime << ")\n";

        // Total Time
        out << "TotalTime" << i + 1 << ":" << (option.drivingTime + option.walkingTime) << "\n";
    }
    out.close();
}




