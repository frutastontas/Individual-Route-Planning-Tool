//
// Created by joaop on 01/03/2025.
//

#ifndef URBANMAP_H
#define URBANMAP_H



#include <vector>
#include "MutablePriorityQueu.h"

template <class T>
class Edge;

/////////////////////////Vertex/////////////////////////////////////////

template <class T>
class Vertex {
public:
    Vertex(T in, int ID,int parking);
    bool operator<(Vertex<T> & vertex) const; // // required by MutablePriorityQueue

    T getInfo() const;
    std::vector<Edge<T> *> getAdj() const;
    bool isVisited() const;
    unsigned int getIndegree() const;
    double getDist() const;
    Edge<T> *getPath() const;
    std::vector<Edge<T> *> getIncoming() const;

    int hasParking();

    void setInfo(T info);
    void setVisited(bool visited);

    int getID();


    void setIndegree(unsigned int indegree);
    void setDist(double dist);
    void setPath(Edge<T> *path);

    void setParking(int p);

    Edge<T> * addEdge(Vertex<T> *dest, double driving, double walking);
    bool removeEdge(T in);
    void removeOutgoingEdges();

    friend class MutablePriorityQueue<Vertex>;
protected:
    T info;                // Code of the Node
    int ID;                // id of the node
    std::vector<Edge<T> *> adj;  // outgoing edges

    // auxiliary fields
    bool visited = false; // used by DFS, BFS, Prim ...
    unsigned int indegree; // used by topsort
    double dist = 0;
    Edge<T> *path = nullptr;
    std::vector<Edge<T> *> incoming; // incoming edges
    int parking;
    int queueIndex = 0; 		// required by MutablePriorityQueue and UFDS

    void deleteEdge(Edge<T> *edge);
};

template <class T>
Vertex<T>::Vertex(T in, int ID,int parking): info(in), parking(parking), ID(ID) {}
/*
 * Auxiliary function to add an outgoing edge to a vertex (this),
 * with a given destination vertex (d) and edge weight (w).
 */
template <class T>
Edge<T> * Vertex<T>::addEdge(Vertex<T> *d, double driving, double walking) {
    auto newEdge = new Edge<T>(this, d, driving, walking);
    adj.push_back(newEdge);
    d->incoming.push_back(newEdge);
    return newEdge;
}

/*
 * Auxiliary function to remove an outgoing edge (with a given destination (d))
 * from a vertex (this).
 * Returns true if successful, and false if such edge does not exist.
 */
template <class T>
bool Vertex<T>::removeEdge(T in) {
    bool removedEdge = false;
    auto it = adj.begin();
    while (it != adj.end()) {
        Edge<T> *edge = *it;
        Vertex<T> *dest = edge->getDest();
        if (dest->getInfo() == in) {
            it = adj.erase(it);
            deleteEdge(edge);
            removedEdge = true; // allows for multiple edges to connect the same pair of vertices (multigraph)
        }
        else {
            it++;
        }
    }
    return removedEdge;
}

/*
 * Auxiliary function to remove an outgoing edge of a vertex.
 */
template <class T>
void Vertex<T>::removeOutgoingEdges() {
    auto it = adj.begin();
    while (it != adj.end()) {
        Edge<T> *edge = *it;
        it = adj.erase(it);
        deleteEdge(edge);
    }
}

template <class T>
bool Vertex<T>::operator<(Vertex<T> & vertex) const {
    return this->dist < vertex.dist;
}

template <class T>
T Vertex<T>::getInfo() const {
    return this->info;
}

template <class T>
std::vector<Edge<T>*> Vertex<T>::getAdj() const {
    return this->adj;
}

template <class T>
bool Vertex<T>::isVisited() const {
    return this->visited;
}

template <class T>
unsigned int Vertex<T>::getIndegree() const {
    return this->indegree;
}

template <class T>
int Vertex<T>::getID() {
    return this->ID;
}

template <class T>
double Vertex<T>::getDist() const {
    return this->dist;
}

template <class T>
Edge<T> *Vertex<T>::getPath() const {
    return this->path;
}

template <class T>
std::vector<Edge<T> *> Vertex<T>::getIncoming() const {
    return this->incoming;
}

template <class T>
int Vertex<T>::hasParking() {
    return this->parking;
}

template <class T>
void Vertex<T>::setInfo(T in) {
    this->info = in;
}

template <class T>
void Vertex<T>::setVisited(bool visited) {
    this->visited = visited;
}



template <class T>
void Vertex<T>::setIndegree(unsigned int indegree) {
    this->indegree = indegree;
}

template <class T>
void Vertex<T>::setDist(double dist) {
    this->dist = dist;
}

template <class T>
void Vertex<T>::setPath(Edge<T> *path) {
    this->path = path;
}
template <class T>
void Vertex<T>::deleteEdge(Edge<T> *edge) {
    Vertex<T> *dest = edge->getDest();
    // Remove the corresponding edge from the incoming list
    auto it = dest->incoming.begin();
    while (it != dest->incoming.end()) {
        if ((*it)->getOrig()->getInfo() == info) {
            it = dest->incoming.erase(it);
        }
        else {
            it++;
        }
    }
    delete edge;
}


///////////////////////////Edge////////////////////////////////////////////////////

template <class T>
class Edge {
public:
    Edge(Vertex<T> *orig, Vertex<T> *dest, double driving, double walking);

    Vertex<T> * getDest() const;
    double getDriving() const;
    double getWalking() const;
    bool isSelected() const;
    Vertex<T> * getOrig() const;
    Edge<T> *getReverse() const;


    void setSelected(bool selected);
    void setReverse(Edge<T> *reverse);

protected:
    Vertex<T> * dest; // destination vertex
    double driving; // edge weight when driving
    double walking; // edge weight for walking

    // auxiliary fields (will be used as a boolean to ckeck if the edge is deactivated)
    bool selected = false;

    // used for bidirectional edges
    Vertex<T> *orig;
    Edge<T> *reverse = nullptr;

};

template <class T>
Edge<T>::Edge(Vertex<T> *orig, Vertex<T> *dest, double driving, double walking): orig(orig), dest(dest), driving(driving), walking(walking){}

template <class T>
Vertex<T> * Edge<T>::getDest() const {
    return this->dest;
}

template <class T>
double Edge<T>::getDriving() const {
    return this->driving;
}

template<class T>
double Edge<T>::getWalking() const {
    return this->walking;
}


template <class T>
Vertex<T> * Edge<T>::getOrig() const {
    return this->orig;
}

template <class T>
Edge<T> *Edge<T>::getReverse() const {
    return this->reverse;
}

template <class T>
bool Edge<T>::isSelected() const {
    return this->selected;
}

template <class T>
void Edge<T>::setSelected(bool selected) {
    this->selected = selected;
}

template <class T>
void Edge<T>::setReverse(Edge<T> *reverse) {
    this->reverse = reverse;
}




///////////////////Graph//////////////////////////////////////////////




/**
 * @class UrbanMap
 * @brief Represents an urban map as a graph structure.
 *
 * This class models locations as vertices and roads as edges, supporting various operations
 * such as adding and removing locations and roads, and managing connectivity.
 */

template <class T>
class UrbanMap {
public:

    /**
     * @brief Finds a location (vertex) with the given content.
     * @param in The content to search for.
     * @return A pointer to the vertex if found, otherwise nullptr.
     */
    Vertex<T> *findLocation(const T &in) const;

    /**
     * @brief Adds a location (vertex) to the urban map.
     * @param in The content of the location.
     * @return True if successful, false if a location with that content already exists.
     */
    bool addLocation(const T &in, const int ID,const int parking);

    /**
     * @brief Removes a location (vertex) from the urban map.
     * @param in The content of the location to remove.
     * @return True if successful, false otherwise.
     */
    bool removeLocation(const T &in);

    /**
     * @brief Adds a road (edge) between two locations.
     * @param source The content of the source location.
     * @param dest The content of the destination location.
     * @param w The weight (distance) of the road.
     * @return True if successful, false if either location does not exist.
     */
    bool addRoad(const T &source, const T &dest, double driving, double walking);

    /**
     * @brief Removes a road (edge) between two locations.
     * @param source The content of the source location.
     * @param dest The content of the destination location.
     * @return True if successful, false otherwise.
     */
    bool removeRoad(const T &source, const T &dest);

    /**
     * @brief Adds a bidirectional road (edge) between two locations.
     * @param source The content of the first location.
     * @param dest The content of the second location.
     * @param w The weight (distance) of the road.
     * @return True if successful, false if either location does not exist.
     */
    bool addBidirectionalRoad(const T &source, const T &dest, double driving, double walking);

    /**
     * @brief Gets the number of locations (vertices) in the urban map.
     * @return The number of locations.
     */
    int getNumLocations() const;

    /**
     * @brief Gets the set of locations (vertices) in the urban map.
     * @return A vector of pointers to the locations.
     */
    std::vector<Vertex<T> *> getLocationSet() const;


    bool DrivingModeEnabled() const;    /** if true then we use the driving weight, false we use the walking*/

    void setDrivingMode(bool mode);

    void setParkingNode(int id);
    std::vector<int> getParkingNodes() const;

    void resetGraph();

protected:
    std::vector<Vertex<T> *> locationSet;    ///< Set of locations (vertices)

    //hash set for efficient find of Vertex (to implement usage)

    //vector with the ids of all the nodes that have available parking
    std::vector<int> parkingNodes;

    bool drivingMode = false;
    /**
     * @brief Finds the index of a location with the given content.
     * @param in The content to search for.
     * @return The index of the location if found, otherwise -1.
     */
    int findLocationIdx(const T &in) const;

    /**
     * @brief Auxiliary function to set the "path" field to create a spanning tree.
     */
};

template <class T>
int UrbanMap<T>::getNumLocations() const {
    return locationSet.size();
}

template <class T>
std::vector<Vertex<T> *> UrbanMap<T>::getLocationSet() const {
    return locationSet;
}

/*
 * Auxiliary function to find a vertex with a given content.
 */
template <class T>
Vertex<T> * UrbanMap<T>::findLocation(const T &in) const {
    for (auto v : locationSet)
        if (v->getInfo() == in)
            return v;
    return nullptr;
}

/*
 * Finds the index of the vertex with a given content.
 */
template <class T>
int UrbanMap<T>::findLocationIdx(const T &in) const {
    for (unsigned i = 0; i < locationSet.size(); i++)
        if (locationSet[i]->getInfo() == in)
            return i;
    return -1;
}
/*
 *  Adds a vertex with a given content or info (in) to a graph (this).
 *  Returns true if successful, and false if a vertex with that content already exists.
 */
template <class T>
bool UrbanMap<T>::addLocation(const T &in, const int ID,const int parking) {
    if (findLocation(in) != nullptr)
        return false;
    locationSet.push_back(new Vertex<T>(in,ID,parking));
    return true;
}

/*
 *  Removes a vertex with a given content (in) from a graph (this), and
 *  all outgoing and incoming edges.
 *  Returns true if successful, and false if such vertex does not exist.
 */
template <class T>
bool UrbanMap<T>::removeLocation(const T &in) {
    for (auto it = locationSet.begin(); it != locationSet.end(); it++) {
        if ((*it)->getInfo() == in) {
            auto v = *it;
            v->removeOutgoingEdges();
            for (auto u : locationSet) {
                u->removeEdge(v->getInfo());
            }
            locationSet.erase(it);
            delete v;
            return true;
        }
    }
    return false;
}

/*
 * Adds an edge to a graph (this), given the contents of the source and
 * destination vertices and the edge weight (w).
 * Returns true if successful, and false if the source or destination vertex does not exist.
 */
template <class T>
bool UrbanMap<T>::addRoad(const T &sourc, const T &dest, double driving, double walking) {
    auto v1 = findVertex(sourc);
    auto v2 = findVertex(dest);
    if (v1 == nullptr || v2 == nullptr)
        return false;
    v1->addEdge(v2, driving, walking);
    return true;
}

/*
 * Removes an edge from a graph (this).
 * The edge is identified by the source (sourc) and destination (dest) contents.
 * Returns true if successful, and false if such edge does not exist.
 */
template <class T>
bool UrbanMap<T>::removeRoad(const T &sourc, const T &dest) {
    Vertex<T> * srcVertex = findLocation(sourc);
    if (srcVertex == nullptr) {
        return false;
    }
    return srcVertex->removeEdge(dest);
}

template <class T>
bool UrbanMap<T>::addBidirectionalRoad(const T &sourc, const T &dest, double driving, double walking) {
    auto v1 = findLocation(sourc);
    auto v2 = findLocation(dest);
    if (v1 == nullptr || v2 == nullptr)
        return false;
    auto e1 = v1->addEdge(v2, driving, walking);
    auto e2 = v2->addEdge(v1, driving, walking);
    e1->setReverse(e2);
    e2->setReverse(e1);
    return true;
}

template <class T>
bool UrbanMap<T>::DrivingModeEnabled() const {
    return drivingMode;
}

template <class T>
void UrbanMap<T>::setDrivingMode(bool mode) {
    drivingMode = mode;
}

template <class T>
void UrbanMap<T>::setParkingNode(int id) {
    this->parkingNodes.push_back(id);
}

template <class T>
std::vector<int> UrbanMap<T>::getParkingNodes() const {
    return this->parkingNodes;
}

template<class T>
void UrbanMap<T>::resetGraph() {
    for (auto v : locationSet) {
        v->setVisited(false);
        for (auto e : v->getAdj()) {
            e->setSelected(false);
        }
    }
}


#endif //URBANMAP_H


