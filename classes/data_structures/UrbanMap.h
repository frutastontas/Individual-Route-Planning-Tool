//
// Created by joaop on 01/03/2025.
//

#ifndef URBANMAP_H
#define URBANMAP_H


#include "Vertex.h"
#include <vector>

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
     * @brief Destructor for UrbanMap.
     */
    ~UrbanMap();

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
    bool addLocation(const T &in);

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
    bool addRoad(const T &source, const T &dest, double w);

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
    bool addBidirectionalRoad(const T &source, const T &dest, double w);

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

protected:
    std::vector<Vertex<T> *> locationSet;    ///< Set of locations (vertices)

    double ** distMatrix = nullptr;   ///< Distance matrix for Floyd-Warshall
    int **pathMatrix = nullptr;   ///< Path matrix for Floyd-Warshall

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
bool UrbanMap<T>::addLocation(const T &in) {
    if (findLocation(in) != nullptr)
        return false;
    locationSet.push_back(new Vertex<T>(in));
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
bool UrbanMap<T>::addRoad(const T &sourc, const T &dest, double w) {
    auto v1 = findVertex(sourc);
    auto v2 = findVertex(dest);
    if (v1 == nullptr || v2 == nullptr)
        return false;
    v1->addEdge(v2, w);
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
bool UrbanMap<T>::addBidirectionalRoad(const T &sourc, const T &dest, double w) {
    auto v1 = findLocation(sourc);
    auto v2 = findLocation(dest);
    if (v1 == nullptr || v2 == nullptr)
        return false;
    auto e1 = v1->addEdge(v2, w);
    auto e2 = v2->addEdge(v1, w);
    e1->setReverse(e2);
    e2->setReverse(e1);
    return true;
}

#endif //URBANMAP_H


