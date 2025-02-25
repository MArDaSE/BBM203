#include "RealmShaper.h"
#include <fstream>
#include <sstream>
#include <iostream>

#define NECESSARY_ENERGY 2000 // Necessary energy to craft an Isle

RealmShaper::RealmShaper(std::string name, int honour) : name(name)
{
    this->honour = honour;
}

const std::string &RealmShaper::getName() const
{
    return this->name;
}

int RealmShaper::getEnergyLevel()
{
    return this->collectedEnergyPoints;
}

int RealmShaper::getHonour()
{
    return this->honour;
}

void RealmShaper::gainHonour()
{
    this->honour += 150;
}

void RealmShaper::loseHonour()
{
    this->honour -= 150;
}

void RealmShaper::collectItem(Item item)
{
    this->collectedEnergyPoints += item;
}

void RealmShaper::loseEnergy()
{
    this->collectedEnergyPoints -= NECESSARY_ENERGY;
}

bool RealmShaper::hasEnoughEnergy()
{
    return this->collectedEnergyPoints >= NECESSARY_ENERGY;
}

std::vector<RealmShaper *> RealmShaper::readFromFile(const std::string &filename)
{
    std::vector<RealmShaper *> players;

    // TODO: Read RealmShapers from the file,
    // add them to vector
    // return the vector
    // Input format: playerName[tab]honourPoints
    std::ifstream realmShaperFile(filename);
    if (!realmShaperFile.is_open()) {
        std::cout << "this file cannot opened!!!" << std::endl;
        return players;
    }

    std::string each_row;
    while(std::getline(realmShaperFile, each_row)) {
        std::stringstream row(each_row);
        std::string name;
        std::string honour;

        row >> name >> honour;
        players.push_back(new RealmShaper(name, std::stoi(honour)));
    }

    realmShaperFile.close();

    for (RealmShaper* rs : players) {
        std::cout << "name: " << rs->getName() << "   rank: " << rs->getHonour() << std::endl; 
    }
    return players;
}

bool RealmShaper::operator==(const RealmShaper &other) const
{
    // TODO: Compare by name, return true if same
    return (this->getName() == other.getName());
}

std::ostream &operator<<(std::ostream &os, const RealmShaper &p)
{
    // Outs the player name
    return (os << p.name); // << std::endl);
}
