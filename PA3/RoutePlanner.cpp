#include "RoutePlanner.h"
#include <iostream>
#include <fstream>

// Array to help you out with name of the cities in order
const std::string cities[81] = { 
    "Adana", "Adiyaman", "Afyon", "Agri", "Amasya", "Ankara", "Antalya", "Artvin", "Aydin", "Balikesir", "Bilecik", 
    "Bingol", "Bitlis", "Bolu", "Burdur", "Bursa", "Canakkale", "Cankiri", "Corum", "Denizli", "Diyarbakir", "Edirne", 
    "Elazig", "Erzincan", "Erzurum", "Eskisehir", "Gaziantep", "Giresun", "Gumushane", "Hakkari", "Hatay", "Isparta", 
    "Mersin", "Istanbul", "Izmir", "Kars", "Kastamonu", "Kayseri", "Kirklareli", "Kirsehir", "Kocaeli", "Konya", "Kutahya", 
    "Malatya", "Manisa", "Kaharamanmaras", "Mardin", "Mugla", "Mus", "Nevsehir", "Nigde", "Ordu", "Rize", "Sakarya", 
    "Samsun", "Siirt", "Sinop", "Sivas", "Tekirdag", "Tokat", "Trabzon", "Tunceli", "Urfa", "Usak", "Van", "Yozgat", 
    "Zonguldak", "Aksaray", "Bayburt", "Karaman", "Kirikkale", "Batman", "Sirnak", "Bartin", "Ardahan", "Igdir", 
    "Yalova", "Karabuk", "Kilis", "Osmaniye", "Duzce" 
};

// Constructor to initialize and load constraints
RoutePlanner::RoutePlanner(const std::string& distance_data, const std::string& priority_data, const std::string& restricted_data, int maxDistance)
    : maxDistance(maxDistance), totalDistanceCovered(0), numPriorityProvinces(0), numWeatherRestrictedProvinces(0) {

    // TO DO:
    for (int i = 0; i < MAX_PRIORITY_PROVINCES; ++i) {
        priorityProvinces[i] = -1;
    }
    for (int i = 0; i < MAX_WEATHER_RESTRICTED_PROVINCES; ++i) {
        weatherRestrictedProvinces[i] = -1;
    }
    // Load map data from file
    map.loadDistanceData(distance_data);
    // Mark all provinces as unvisited initially
    map.resetVisited();
    // Load priority provinces
    loadPriorityProvinces(priority_data);
    // Load restricted provinces
    loadWeatherRestrictedProvinces(restricted_data);
}

// Load priority provinces from txt file to an array of indices
void RoutePlanner::loadPriorityProvinces(const std::string& filename) {
    // TODO: Your code here
    std::ifstream priority_file(filename);
    if (!priority_file.is_open()) {
        std::cout << "this file cannot opened!!!" << std::endl;
        return;
    }

    std::string each_row;
    int i = 0;
    
    while(std::getline(priority_file, each_row)) {
        std::string priority_number = "";
        for (char c : each_row) {
            if ('0' <= c && c <= '9') {
                priority_number += c;
                continue;
            } 
        }
        if (!priority_number.empty()) {
            priorityProvinces[i++] = std::stoi(priority_number);
        }
    }
    numPriorityProvinces = i;
    priority_file.close();
}

// Load weather-restricted provinces from txt file to an array of indices
void RoutePlanner::loadWeatherRestrictedProvinces(const std::string& filename) {
    // TODO: Your code here
    std::ifstream restricted_file(filename);
    if (!restricted_file.is_open()) {
        std::cout << "this file cannot opened!!!" << std::endl;
        return;
    }

    std::string each_row;
    int i = 0;
    
    while(std::getline(restricted_file, each_row)) {
        std::string restricted_number = "";
        for (char c : each_row) {
            if ('0' <= c && c <= '9') {
                restricted_number += c;
                continue;
            } 
        }
        if (!restricted_number.empty()) {
            weatherRestrictedProvinces[i++] = std::stoi(restricted_number);
        }
    }
    numWeatherRestrictedProvinces = i;
    restricted_file.close();
}

// Checks if a province is a priority province
bool RoutePlanner::isPriorityProvince(int province) const {
    // TODO: Your code here
    if (numPriorityProvinces <= 0) {
        return false;
    }
    for (int i = 0; i < numPriorityProvinces; i++) {
        if (priorityProvinces[i] == -1) {
            break;
        }
        if (priorityProvinces[i] == province) {
            return true;
        }
    }
    return false;
}

// Checks if a province is weather-restricted
bool RoutePlanner::isWeatherRestricted(int province) const {
    // TODO: Your code here
    if (numWeatherRestrictedProvinces <= 0) {
        return false;
    }
    //weatherRestrictedProvinces[numWeatherRestrictedProvinces];
    for (int i = 0; i < numWeatherRestrictedProvinces; i++) {
        if (weatherRestrictedProvinces[i] == -1) {
            break;
        }
        if (weatherRestrictedProvinces[i] == province) {
            return true;
        }
    }
    return false;
}

// Begins the route exploration from the starting point
void RoutePlanner::exploreRoute(int startingCity) {
    // TODO: Your code here
    if (!map.isVisited(startingCity)) {
        map.markAsVisited(startingCity);
        route.push_back(startingCity);
        stack.push(startingCity);
    }
    exploreFromProvince(startingCity);
}

// Helper function to explore from a specific province
void RoutePlanner::exploreFromProvince(int province) {
    // TODO: Your code here 
    enqueueNeighbors(province);
    bool flag = false;
    while (true) {
        if (queue.isEmpty()) {
            if (isExplorationComplete()) {
                displayResults();
                break;
            }
            backtrack();
        } else if (map.isWithinRange(stack.peek(), queue.peek(), maxDistance)) {
            totalDistanceCovered += map.getDistance(stack.peek(), queue.peek());
            flag = true;
            break;
        } else {
            queue.dequeue();
        }
    }
    if (flag) {
        exploreRoute(queue.dequeue());
    } else {
        return;
    }
}

void RoutePlanner::enqueueNeighbors(int province) {
    // TO DO: Enqueue priority & non-priority neighbors to the queue according to given constraints
    if (province < 0 || province >= 81) {
        return;
    }
    for (int i = 0; i < 81; i++) {
        if (map.isWithinRange(province, i, maxDistance) && !map.isVisited(i)) {
            if (isPriorityProvince(i)) {
                queue.enqueuePriority(i);
            } else if (isWeatherRestricted(i)) {
                std::cout << "Province " << cities[i] << " " << i << " is weather-restricted. Skipping." << std::endl;
                return;
            } else {
                queue.enqueue(i);
            }
        }
    }
}

void RoutePlanner::backtrack() {
    // If you reach a dead-end province 
    // TODO: Your code here
    stack.pop();
    for (int i = 0; i < 81; i++) {
        if (stack.isEmpty()) {
            return;
        }
        if (map.isWithinRange(stack.peek(), i, maxDistance) && !map.isVisited(i)) {
            enqueueNeighbors(i);
        }
    }
}

bool RoutePlanner::isExplorationComplete() const {
    // TODO: Your code here
    return stack.isEmpty() && queue.isEmpty();
}

void RoutePlanner::displayResults() const {
    // TODO: Your code here
    // Display "Journey Completed!" message
    // Display the total number of provinces visited
    // Display the total distance covered
    // Display the route in the order visited
    // Priority Province Summary
    std::cout << "----------------------------" << std::endl;
    std::cout << "Journey Completed!" << std::endl;
    std::cout << "----------------------------" << std::endl;
    std::cout << "Total Number of Provinces Visited: " << static_cast<int>(route.size()) << std::endl;
    std::cout << "Total Distance Covered: " << totalDistanceCovered << " km" << std::endl;
    std::cout << "Route Taken:" << std::endl;
    for (int i = 0; i < static_cast<int>(route.size()); i++) {
        std::cout << cities[route[i]] << " -> ";
    }
    std::cout << "End" << std::endl;
    std::cout << std::endl;
    std::cout << "Priority Provinces Status:" << std::endl;
    int visited_counter = 0;
    for (int i = 0; i < numPriorityProvinces; i++) {
        if (map.visited[priorityProvinces[i]]) {
            visited_counter++;
        }
        std::cout << "- " << cities[priorityProvinces[i]] << " (" << (map.visited[priorityProvinces[i]] ? "Visited" : "Unvisited") << ")" << std::endl; 
    }
    std::cout << std::endl;
    std::cout << "Total Priority Provinces Visited: " << visited_counter << " out of " << numPriorityProvinces << std::endl;
    if (visited_counter == numPriorityProvinces) {
        std::cout << "Success: All priority provinces were visited.";
    } else {
        std::cout << "Warning: Not all priority provinces were visited.";
    }
}