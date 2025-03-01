//
// Created by joaop on 01/03/2025.
//

#ifndef URBANMAP_H
#define URBANMAP_H

#include "Edge.h"
#include "Vertex.h"


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

#endif //URBANMAP_H


