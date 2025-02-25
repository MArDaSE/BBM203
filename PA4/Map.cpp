#include "Map.h"

Map::Map()
{
    this->root = nullptr;
}
Map::~Map()
{
    // TODO: Free any dynamically allocated memory if necessary
}

void Map::initializeMap(std::vector<Isle *> isles)
{
    // TODO: Insert innitial isles to the tree
    // Then populate with Goldium and Einstainium items
    isleSize = static_cast<int>(isles.size());
    for (Isle* i : isles) {
        insert(i);
    }
}

MapNode *Map::rotateRight(MapNode *current)
{
    // TODO: Perform right rotation according to AVL
    // return necessary new root
    // Use std::cerr << "[Right Rotation] " << "Called on invalid node!" << std::endl;
    if (current == nullptr) {
        return current;
    }
    MapNode* newRoot = current->left;
    current->left = newRoot->right;
    newRoot->right = current;

    current->height = 1 + std::max(height(current->left), height(current->right));
    newRoot->height = 1 + std::max(height(newRoot->left), height(newRoot->right));

    return newRoot;
}

MapNode *Map::rotateLeft(MapNode *current)
{
    // TODO: Perform left rotation according to AVL
    // return necessary new root
    // Use std::cerr << "[Left Rotation] " << "Called on invalid node!" << std::endl;
    if (current == nullptr) {
        return current;
    }
    MapNode* newRoot = current->right;
    current->right = newRoot->left;
    newRoot->left = current;

    current->height = 1 + std::max(height(current->left), height(current->right));
    newRoot->height = 1 + std::max(height(newRoot->left), height(newRoot->right));

    return newRoot;
}

MapNode *Map::balanceTree(MapNode* node) {
    if (node == nullptr) {
        return node;
    }
    int balanceSituation = height(node->left) - height(node->right);
    if (balanceSituation < -1) { // right is heavy
        if (height(node->right->right) >= height(node->right->left)) { // for single rotation
            node = rotateLeft(node);
        } else { // for double rotation
            node->right = rotateRight(node->right);
            node = rotateLeft(node);            
        }
    } else if (1 < balanceSituation) { // left is heavy
        if (height(node->left->left) >= height(node->left->right)) { // for single rotation
            node = rotateRight(node);
        } else { // for double rotation
            node->left = rotateLeft(node->left);
            node = rotateRight(node);
        }
    } else { // no problemaaa O7 return current node
        return node;
    }

    node->height = 1 + std::max(height(node->left), height(node->right));
    return node;
}

int calculateMinMapDepthAccess(int playerDepth, int totalShaperTreeHeight, int totalMapDepth)
{
    return (int)totalMapDepth * ((double)playerDepth / totalShaperTreeHeight);
}

int Map::height(MapNode *node)
{
    // TODO: Return height of the node
    if (node == nullptr) {
        return 0;
    }
    return node->height;
}

MapNode *Map::insert(MapNode *node, Isle *isle)
{
    // TODO: Recursively insert isle to the tree
    // returns inserted node

    if (node == nullptr) {
        MapNode* newNode = new MapNode(isle);
        return newNode;
    }
    if (*isle < *(node->isle)) {
        node->left = insert(node->left, isle);
    } else if (*isle > *(node->isle)) {
        node->right = insert(node->right, isle);
    } else {
        return node;
    }
    node->height = 1 + std::max(height(node->left), height(node->right));
    return balanceTree(node);
}

void Map::insert(Isle *isle)
{
    root = insert((root), isle);

    // you might need to insert some checks / functions here depending on your implementation
}

MapNode *Map::remove(MapNode *node, Isle *isle)
{
    // TODO: Recursively delete isle from the tree
    // Will be called if there is overcrowding
    // returns node
    // Use std::cout << "[Remove] " << "Tree is Empty" << std::endl;
    if (node == nullptr) {
        std::cout << "[Remove] Tree is Empty" << std::endl;
        return node;
    }

}

void Map::remove(Isle *isle)
{   
    root = remove((root), isle);
    // you might need to insert some checks / functions here depending on your implementation
}

void Map::preOrderItemDrop(MapNode *current, int &count)
{
    // TODO: Drop EINSTEINIUM according to rules
    // Use std::cout << "[Item Drop] " << "EINSTEINIUM dropped on Isle: " << current->isle->getName() << std::endl;
}

// to Display the values by Post Order Method .. left - right - node
void Map::postOrderItemDrop(MapNode *current, int &count)
{
    // TODO: Drop GOLDIUM according to rules
    // Use  std::cout << "[Item Drop] " << "GOLDIUM dropped on Isle: " << current->isle->getName() << std::endl;
}

MapNode *Map::findFirstEmptyIsle(MapNode *node)
{
    // TODO: Find first Isle with no item
    if (node == nullptr) {
        return nullptr;
    }
    std::queue<MapNode*> nodeQueue;
    nodeQueue.push(node);
    while (true) {
        MapNode* tempNode = nodeQueue.front();
        nodeQueue.pop();
        if (tempNode->isle->getItem() == EMPTY) {
            return tempNode;
        }
        if (tempNode->left != nullptr) {
            nodeQueue.push(tempNode->left);
        }
        if (tempNode->right != nullptr) {
            nodeQueue.push(tempNode->right);
        }
        if (nodeQueue.empty()) {
            break;
        }
    }
    return nullptr;
}

void Map::dropItemBFS()
{
    // TODO: Drop AMAZONITE according to rules
    // Use std::cout << "[BFS Drop] " << "AMAZONITE dropped on Isle: " << targetNode->isle->getName() << std::endl;
    // Use std::cout << "[BFS Drop] " << "No eligible Isle found for AMAZONITE drop." << std::endl;
}

void Map::displayMap()
{
    std::cout << "[World Map]" << std::endl;
    display(root, 0, 0);
}

int Map::getDepth(MapNode *node)
{
    // TODO: Return node depth if found, else
    if (node == nullptr) {
        return -1;
    }
    if (node == root) {
        return 0;
    }

    int counter = 0;
    MapNode* tempNode = root;
    while (true)
    {
        if ((node < tempNode) && (tempNode->left != nullptr)) {
            counter++;
            tempNode = tempNode->left;
            continue;
        }
        if ((tempNode < node) && (tempNode->right != nullptr)) {
            counter++;
            tempNode = tempNode->right;
            continue;
        }
        if (tempNode == node) {
            return counter;
        } else {
            return -1;
        }
    }
}

// Function to calculate the depth of a specific node in the AVL tree
int Map::getIsleDepth(Isle *isle)
{
    // TODO: Return node depth by isle if found, else
    if (isle == nullptr) {
        return -1;
    }

    if (isle == root->isle) {
        return 0;
    }

    int counter = 0;
    MapNode* tempNode = root;
    while (true)
    {
        if ((isle < tempNode->isle) && (tempNode->left != nullptr)) {
            counter++;
            tempNode = tempNode->left;
            continue;
        }

        if ((tempNode->isle < isle) && (tempNode->right != nullptr)) {
            counter++;
            tempNode = tempNode->right;
            continue;
        }

        if (tempNode->isle == isle) {
            return counter;
        } else {
            return -1;
        }
    }
}

int Map::getDepth()
{
    // TODO: Return max|total depth of tree
    if (root == nullptr) {
        return -1;
    } else {
        return height(root);
    }
}

void Map::populateWithItems()
{
    // TODO: Distribute fist GOLDIUM than EINSTEINIUM
}

Isle *Map::findIsle(Isle isle)
{
    // TODO: Find isle by value 
    for (MapNode* mn : nodes) {
        if (*mn->isle == isle) {
            return mn->isle;
        }
    }
    return nullptr;
}

Isle *Map::findIsle(std::string name)
{
    // TODO: Find isle by name
    for (MapNode* mn : nodes) {
        if (mn->isle->getName() == name) {
            return mn->isle;
        }
    }
    return nullptr;
}

MapNode *Map::findNode(Isle isle)
{
    // TODO: Find node by value
    for (MapNode* mn : nodes) {
        if (*mn->isle == isle) {
            return mn;
        }
    }
    return nullptr;
}

MapNode *Map::findNode(std::string name)
{
    // TODO: Find node by name
    for (MapNode* mn : nodes) {
        if (mn->isle->getName() == name) {
            return mn;
        }
    }
    return nullptr;
}

void Map::display(MapNode *current, int depth, int state)
{
    // SOURCE:

    if (current->left)
        display(current->left, depth + 1, 1);

    for (int i = 0; i < depth; i++)
        printf("     ");

    if (state == 1) // left
        printf("   ┌───");
    else if (state == 2) // right
        printf("   └───");

    std::cout << "[" << *current->isle << "] - (" << current->height << ")\n"
              << std::endl;

    if (current->right)
        display(current->right, depth + 1, 2);
}

void Map::writeToFile(const std::string &filename)
{
    // TODO: Write the tree to filename output level by level
}

void Map::writeIslesToFile(const std::string &filename)
{
    // TODO: Write Isles to output file in alphabetical order
    // Use std::cout << "[Output] " << "Isles have been written to " << filename << " in in alphabetical order." << std::endl;
}

void Map::preorderTraversal() {
    if (root == nullptr) {
        return;
    }
    MapNode* tempNode = root;

    while (true) {
        if (!isItAdded(preorderNodes, tempNode)) {
            preorderNodes.push_back(tempNode);
        } 

        if (tempNode->left != nullptr) {
            if (!isItAdded(preorderNodes, tempNode->left)) {
                tempNode = tempNode->left;
                continue;
            }
        }

        if (tempNode->right != nullptr) {
            if (!isItAdded(preorderNodes, tempNode->right)) {
                tempNode = tempNode->right;
                continue;
            }
        }

        if (getParent(preorderNodes, tempNode) != nullptr) {
            if (static_cast<int>(preorderNodes.size()) == isleSize) {
                break;
            }
            tempNode = getParent(preorderNodes, tempNode);
        } 
    }
}

void Map::postorderTraversal() {
    if (root == nullptr) {
        return;
    }
    MapNode* tempNode = root;

    while (true) {
        if (tempNode->right != nullptr) {
            if (!isItAdded(preorderNodes, tempNode->right)) {
                tempNode = tempNode->right;
                continue;
            }
        }

        if (tempNode->left != nullptr) {
            if (!isItAdded(preorderNodes, tempNode->left)) {
                tempNode = tempNode->left;
                continue;
            }
        }

        if (!isItAdded(preorderNodes, tempNode)) {
            preorderNodes.push_back(tempNode);
        } 

        if (getParent(preorderNodes, tempNode) != nullptr) {
            if (static_cast<int>(preorderNodes.size()) == isleSize) {
                break;
            }
            tempNode = getParent(preorderNodes, tempNode);
        } 
    }
}

bool Map::isItAdded(std::vector<MapNode*> nodeVector, MapNode* node) {
    for (MapNode* mn : nodeVector) {
        if (mn == node) {
            return true;
        }
    }
    return false;
}

MapNode* Map::getParent(std::vector<MapNode*> nodeVector, MapNode* node) {
    if (node == root) {
        return nullptr;
    }

    for (int i = 1; i < 3; i++) {
        if (height(nodeVector[static_cast<int>(nodeVector.size()) - 1 - i]) != height(node)) {
            return nodeVector[static_cast<int>(nodeVector.size()) - 1 - i];
        } 
    }
}