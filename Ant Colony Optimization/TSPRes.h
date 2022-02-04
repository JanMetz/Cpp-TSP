#pragma once
#include <vector>

struct TSPResults {
    long iterations;
    float distance;
    std::vector<unsigned short> path;
};