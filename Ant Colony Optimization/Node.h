#pragma once
#include <cmath>

struct Node {
    int x, y;
    static float Distance(const Node& a, const Node& b);
};

float Node::Distance(const Node& a, const Node& b) {
    if ((a.x == b.x) && (a.y == b.y))
        return 0;
    return (float) std::sqrt(std::pow(a.x - b.x, 2) + std::pow(a.y - b.y, 2));
}