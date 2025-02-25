#include "RealmShapers.h"
#include <cmath>
#include <algorithm>

ShaperTree::ShaperTree()
{
}

ShaperTree::~ShaperTree()
{
    // TODO: Free any dynamically allocated memory if necessary
    for (RealmShaper* pointer : realmShapers) {
        delete pointer;
    }
    realmShapers.clear();
}

void ShaperTree::initializeTree(std::vector<RealmShaper *> shapers)
{
    // TODO: Insert innitial shapers to the tree
    for (RealmShaper* rs : shapers) {
        realmShapers.push_back(rs);
    }
}

int ShaperTree::getSize()
{
    // TODO: Return number of shapers in the tree
    //!!!return 0;
    return static_cast<int>(realmShapers.size());
}

std::vector<RealmShaper *> ShaperTree::getTree()
{
    return realmShapers;
}

bool ShaperTree::isValidIndex(int index)
{
    // TODO: Check if the index is valin in the tree

    return ((0 <= index) && (index <= (static_cast<int>(realmShapers.size()) - 1)));
}

void ShaperTree::insert(RealmShaper *shaper)
{
    // TODO: Insert shaper to the tree
    realmShapers.push_back(shaper);
    
}

int ShaperTree::remove(RealmShaper *shaper)
{
    // TODO: Remove the player from tree if it exists
    // Make sure tree protects its form (complate binary tree) after deletion of a node
    // return index if found and removed
    // else
    int result = findIndex(shaper);
    if (result != -1) {
        realmShapers.erase(realmShapers.begin() + result);
        return result;
    }
    return -1;
}

int ShaperTree::findIndex(RealmShaper *shaper)
{
    // return index in the tree if found
    // else
    int counter = 0;
    for (RealmShaper* rs : realmShapers) {
        if(rs == shaper) {
            return counter;
        }
        counter++;
    }

    return -1;
}

int ShaperTree::getDepth(RealmShaper *shaper)
{
    // return depth of the node in the tree if found
    // else
    int index = findIndex(shaper);
    if (index != -1) {
        if (realmShapers.empty()) {
            return 0;
        }
        int depth = 1;
        while (true) {
            int lowerBound = (static_cast<int>(pow(2, (depth -1)))) - 1;
            int upperBound = (static_cast<int>(pow(2, depth))) - 1;
            if (lowerBound <= index && index <= upperBound) {
                return depth;
            }
            depth++;
        }
    }
    return -1;
}

int ShaperTree::getDepth()
{
    // return total|max depth|height of the tree
    if (realmShapers.empty()) {
        return 0;
    }
    int depth = 1;
    int size = static_cast<int>(realmShapers.size());
    while (true) {
        int lowerBound = (static_cast<int>(pow(2, (depth -1)))) - 1;
        int upperBound = (static_cast<int>(pow(2, depth))) - 1;
        if (lowerBound <= size && size <= upperBound) {
            return depth;
        }
        depth++;
    }
}

RealmShaper ShaperTree::duel(RealmShaper *challenger, bool result)
{
    // TODO: Implement duel logic, return the victor
    // Use   std::cout << "[Duel] " << victorName << " won the duel" << std::endl;
    // Use   std::cout << "[Honour] " << "New honour points: ";
    // Use   std::cout << challengerName << "-" << challengerHonour << " ";
    // Use   std::cout << opponentName << "-" << opponentHonour << std::endl;
    // Use   std::cout << "[Duel] " << loserName << " lost all honour, delete" << std::endl;
    RealmShaper* parent = getParent(challenger);
    if (parent == nullptr) {
        return *challenger;
    }
    if (result) {
        challenger->gainHonour();
        parent->loseHonour();

        std::cout << "[Duel] " << challenger->getName() << " won the duel" << std::endl;
        std::cout << "[Honour] " << "New honour points: ";
        std::cout << challenger->getName() << "-" << challenger->getHonour() << " ";
        std::cout << parent->getName() << "-" << parent->getHonour() << std::endl;

        replace(challenger, parent);
        if (parent->getHonour() <= 0) {
            std::cout << "[Duel] " << parent->getName() << " lost all honour, delete" << std::endl;
            remove(parent);
        }
        return *challenger;
    } else {
        challenger->loseHonour();
        parent->gainHonour();

        std::cout << "[Duel] " << parent->getName() << " won the duel" << std::endl;
        std::cout << "[Honour] " << "New honour points: ";
        std::cout << challenger->getName() << "-" << challenger->getHonour() << " ";
        std::cout << parent->getName() << "-" << parent->getHonour() << std::endl;

        if (challenger->getHonour() <= 0) {
            std::cout << "[Duel] " << challenger->getName() << " lost all honour, delete" << std::endl;
            remove(challenger);
        }
        return *parent;
    }
}

RealmShaper *ShaperTree::getParent(RealmShaper *shaper)
{
    RealmShaper *parent = nullptr;

    // TODO: return parent of the shaper

    int index = findIndex(shaper);
    int parentIndex = (index - 1) / 2;
    parent = realmShapers[parentIndex];
    return parent;
}

void ShaperTree::replace(RealmShaper *player_low, RealmShaper *player_high)
{
    // TODO: Change player_low and player_high's positions on the tree
    int lowerIndex = findIndex(player_low);
    int highIndex = findIndex(player_high);

    realmShapers[lowerIndex] = player_high;
    realmShapers[highIndex] = player_low;
}

RealmShaper *ShaperTree::findPlayer(RealmShaper shaper)
{
    RealmShaper *foundShaper = nullptr;

    // TODO: Search shaper by object
    // Return the shaper if found
    // Return nullptr if shaper not found
    for (RealmShaper* rs : realmShapers) {
        if (rs->getName() == shaper.getName()) {
            return rs;
        }
    }

    return foundShaper;
}

// Find shaper by name
RealmShaper *ShaperTree::findPlayer(std::string name)
{
    RealmShaper *foundShaper = nullptr;

    // TODO: Search shaper by name
    // Return the shaper if found
    // Return nullptr if shaper not found
    for (RealmShaper* rs : realmShapers) {
        if (rs->getName() == name) {
            return rs;
        }
    }

    return foundShaper;
}

std::vector<std::string> ShaperTree::inOrderTraversal(int index)
{
    std::vector<std::string> result = {};
    // TODO: Implement inOrderTraversal in tree
    // Add all to a string vector
    // Return the vector

    // Define and implement as many helper functions as necessary for recursive implementation

    // Note: Since SheperTree is not an binary search tree,
    // in-order traversal will not give rankings in correct order
    // for correct order you need to implement level-order traversal
    // still you are to implement this function as well
    
    int tempIndex = index;
    resultAddList.clear();
    while (true) {
        if (isValidIndex((2 * tempIndex) + 1)) {
            if (!isItAdded((2 * tempIndex) + 1)) {
                tempIndex = (2 * tempIndex) + 1;
                continue;
            }
        } else {
            if (!isItAdded(tempIndex)) {
                result.push_back(realmShapers[tempIndex]->getName());
                resultAddList.push_back(tempIndex);
            }
            tempIndex = findIndex(getParent(realmShapers[tempIndex]));
        }

        if (!isItAdded(tempIndex)) {
            result.push_back(realmShapers[tempIndex]->getName());
            resultAddList.push_back(tempIndex);
        } else {
            if (getParent(realmShapers[tempIndex]) != nullptr) {
                if (static_cast<int>(result.size()) == static_cast<int>(realmShapers.size())) {
                    break;
                }
                tempIndex = findIndex(getParent(realmShapers[tempIndex]));
                continue;
            } 
        }

        if (isValidIndex((2 * tempIndex) + 2)) {
            if (!isItAdded((2 * tempIndex) + 2)) {
                tempIndex = (2 * tempIndex) + 2;
                continue;
            }
        } else {
            if (!isItAdded(tempIndex)) {
                result.push_back(realmShapers[tempIndex]->getName());
                resultAddList.push_back(tempIndex);
            }
            tempIndex = findIndex(getParent(realmShapers[tempIndex]));
        }

        if (static_cast<int>(result.size()) == static_cast<int>(realmShapers.size())) {
            break;
        }
    }
    
    return result;
}

bool ShaperTree::isItAdded(int index) {
    for (int i : resultAddList) {
        if (i == index) {
            return true;
        }
    }
    return false;
}

std::vector<std::string> ShaperTree::preOrderTraversal(int index)
{
    std::vector<std::string> result = {};
    // TODO: Implement preOrderTraversal in tree
    // Add all to a string vector
    // Return the vector

    // Define and implement as many helper functions as necessary for recursive implementation

    int tempIndex = index;
    resultAddList.clear();

    while (true) {
        if (!isItAdded(tempIndex)) {
            result.push_back(realmShapers[tempIndex]->getName());
            resultAddList.push_back(tempIndex);
        } 

        if (isValidIndex((2 * tempIndex) + 1)) {
            if (!isItAdded((2 * tempIndex) + 1)) {
                tempIndex = (2 * tempIndex) + 1;
                continue;
            }
        }

        if (isValidIndex((2 * tempIndex) + 2)) {
            if (!isItAdded((2 * tempIndex) + 2)) {
                tempIndex = (2 * tempIndex) + 2;
                continue;
            }
        }

        if (getParent(realmShapers[tempIndex]) != nullptr) {
            if (static_cast<int>(result.size()) == static_cast<int>(realmShapers.size())) {
                break;
            }
            tempIndex = findIndex(getParent(realmShapers[tempIndex]));
            continue;
        } 
    }

    return result;
}

std::vector<std::string> ShaperTree::postOrderTraversal(int index)
{
    std::vector<std::string> result = {};
    // TODO: Implement postOrderTraversal in tree
    // Add all to a string vector
    // Return the vector

    // Define and implement as many helper functions as necessary for recursive implementation

    int tempIndex = index;
    resultAddList.clear();

    while (true) {
        if (isValidIndex((2 * tempIndex) + 1)) {
            if (!isItAdded((2 * tempIndex) + 1)) {
                tempIndex = (2 * tempIndex) + 1;
                continue;
            }
        } 

        if (isValidIndex((2 * tempIndex) + 2)) {
            if (!isItAdded((2 * tempIndex) + 2)) {
                tempIndex = (2 * tempIndex) + 2;
                continue;
            }
        }

        if (!isItAdded(tempIndex)) {
            result.push_back(realmShapers[tempIndex]->getName());
            resultAddList.push_back(tempIndex);
        } 

        if (getParent(realmShapers[tempIndex]) != nullptr) {
            if (static_cast<int>(result.size()) == static_cast<int>(realmShapers.size())) {
                break;
            }
            tempIndex = findIndex(getParent(realmShapers[tempIndex]));
        }
    }

    return result;
}

void ShaperTree::preOrderTraversal(int index, std::ofstream &outFile)
{
    // TODO: Implement preOrderTraversal in tree
    // write nodes to output file

    // Define and implement as many helper functions as necessary for recursive implementation
}

void ShaperTree::breadthFirstTraversal(std::ofstream &outFile)
{
    // TODO: Implement level-order traversal
    // write nodes to output file

    // Define and implement as many helper functions as necessary
}

void ShaperTree::displayTree()
{
    std::cout << "[Shaper Tree]" << std::endl;
    printTree(0, 0, "");
}

// Helper function to print tree with indentation
void ShaperTree::printTree(int index, int level, const std::string &prefix)
{
    if (!isValidIndex(index))
        return;

    std::cout << prefix << (level > 0 ? "   └---- " : "") << *realmShapers[index] << std::endl;
    int left = (2 * index) + 1;  // TODO: Calculate left index
    int right = (2 * index) + 2; // TODO: Calculate right index

    if (isValidIndex(left) || isValidIndex(right))
    {
        printTree(left, level + 1, prefix + (level > 0 ? "   │   " : "")); // ╎
        printTree(right, level + 1, prefix + (level > 0 ? "   │   " : ""));
    }
}

void ShaperTree::writeShapersToFile(const std::string &filename)
{
    // TODO: Write the shapers to filename output level by level
    //!!! current realm shapers
    std::ofstream outShapers(filename);

    if (!outShapers.is_open()) {
        std::cout << "This file cannot be opened!" << std::endl;
        return;
    }
    
    int indexNumber = 1;
    for (RealmShaper* rs : realmShapers) {
        outShapers << indexNumber << "\t" << rs->getName() << "\n";
    }
    outShapers.close();
    std::cout << "[Output] " << "Shapers have been written to " << filename << " according to rankings." << std::endl;
}

void ShaperTree::writeToFile(const std::string &filename)
{
    // TODO: Write the tree to filename output pre-order
    //!!! current shaper tree
    // Use std::cout << "[Output] " << "Tree have been written to " << filename << " in pre-order." << std::endl;
}
