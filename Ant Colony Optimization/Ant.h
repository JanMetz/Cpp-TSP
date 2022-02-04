#pragma once
#include <vector>

struct Ant {
    unsigned short currentNode;
    float cumulativeDistance;
    std::vector<unsigned short> visitedNodes;
    std::vector<unsigned short> unvisitedNodes;
};
