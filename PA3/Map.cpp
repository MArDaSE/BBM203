#include "Map.h"
#include <fstream>
#include <iostream>
#include <sstream>

Map::Map() {
    // TODO: Your code here
    // Initialize all distances to a value representing no direct connection
    // Initialize all provinces as unvisited
    for (int i = 0; i < MAX_SIZE; i++) {
        for (int j = 0; j < MAX_SIZE; j++) {
            distanceMatrix[i][j] = -1;
        }
    }

    for (int i = 0; i < MAX_SIZE; i++) {
        visited[i] = false;
    }
}

// Loads distance data from a file and fills the distanceMatrix
void Map::loadDistanceData(const std::string& filename) {
    // TODO: Your code here
    // Read each line in the CSV file
    // Read each cell separated by a comma
    // Convert cell to an integer and store in distanceMatrix
    std::ifstream map_file(filename);
    if (!map_file.is_open()) {
        std::cout << "this file cannot opened!!!" << std::endl;
    }

    std::string each_row;
    int i = 0;
    int j = 0;
    
    while(std::getline(map_file, each_row)) {
        std::string distance = "";
        for (char c : each_row) {
            if (c == ',') {
                distanceMatrix[i][j++] = std::stoi(distance);
                distance = "";
                continue;
            }
            distance += c; 
        }
        distanceMatrix[i++][j] = std::stoi(distance);
        j = 0;
    }
    map_file.close();
}

// Checks if the distance between two provinces is within the allowed maxDistance
bool Map::isWithinRange(int provinceA, int provinceB, int maxDistance) const {
    // TODO: Your code here
    return distanceMatrix[provinceA][provinceB] <= maxDistance; 
}

// Marks a province as visited
void Map::markAsVisited(int province) {
    // TODO: Your code here
    visited[province] = true;
}

// Checks if a province has already been visited
bool Map::isVisited(int province) const {
    // TODO: Your code here
    return visited[province];
}

// Resets all provinces to unvisited
void Map::resetVisited() {
    // TODO: Your code here
    for (int i = 0; i < MAX_SIZE; i++) {
        visited[i] = false;
    }
}

// Function to count the number of visited provinces
int Map::countVisitedProvinces() const {
    // TODO: Your code here
    int counter = 0;
    for (int i = 0; i < MAX_SIZE; i++) {
        if (visited[i] == true) {
            counter++;
        }
    }
    return counter;
}

// Function to get the distance between two provinces
int Map::getDistance(int provinceA, int provinceB) const {
    // TODO: Your code here
    if ((0 <= provinceA && provinceA < MAX_SIZE) && (0 <= provinceB && provinceB < MAX_SIZE)) {
        return distanceMatrix[provinceA][provinceB];
    } else {
        return -1;
    }
    
}