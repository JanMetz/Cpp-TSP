#pragma once

#include <vector>
#include <tuple>
#include <limits>
#include <cmath>
#include <algorithm>
#include <random>
#include <ctime>
#include <cstdlib>
#include <chrono>
#include <numeric>
#include "Node.h"
#include "TSPRes.h"
#include "Ant.h"

class AntColony {
    // ***** VARIABLES SECTION *****
private:
    const float pheromoneAddingRate = 100;  // Q
    const float evaporationRate = 0.6;     // Value between 0 and 1. 
                                            // Describes what percentage of pheromones will remain on a trail after evaporation process
    const float pheromoneFactor = 1;        // alpha - relative importance of trails' pheromone saturation
    const float distanceFactor = 4;         // beta - relative importance of trail length
    unsigned int antsCount;
    size_t nodeNum;                         // number of nodes in provided instance
    unsigned long long setRuntime;          // desired runtime (in miliseconds)

    std::vector<Node> nodes;
    std::vector<unsigned short> nodesID;
    std::vector<std::vector<float>> distances; // stores info about length of every possible route
    std::vector<std::vector<float>> edgeCosts; // stores info about cost of every possible route
    std::vector<std::vector<float>> edgePheromones; // stores ammount of pheromones on every possible route
    std::vector<Ant> ants; // stores every ant


    // ***** FUNCTIONS SECTION *****
public:
    AntColony(std::vector<Node> nds, float time) {nodes = nds; setRuntime = 1000*time; };
    TSPResults TSPAnt();

private:
    void Initialize();
};

void AntColony::Initialize() {
    nodeNum = nodes.size();
    antsCount = nodeNum / 6;

    // Resizing vectors to the desired size
    this->ants.resize(antsCount);
    this->nodes.resize(nodeNum);
    this->edgePheromones.resize(nodeNum, std::vector<float>(nodeNum, 1));
    this->edgeCosts.resize(nodeNum, std::vector<float>(nodeNum, 0));
    this->distances.resize(nodeNum, std::vector<float>(nodeNum, 0));
    this->nodesID.resize(nodeNum);

    std::iota(nodesID.begin(), nodesID.end(), 0);

    // Initializing every ant giving it random starting point and blank vector of visited nodes
    for (auto& a : ants) {
        a.currentNode = rand() % nodeNum;
        a.visitedNodes = std::vector<unsigned short>(nodeNum + 1, -1);
        a.unvisitedNodes = nodesID;
    }

    // Calculating distances and costs between every set of nodes
    float distance, cost;
    for (unsigned short i = 0; i < nodeNum; i++) {
        for (unsigned short j = 0; j < nodeNum; j++) {
            if (i == j)
                break;

            distance = Node::Distance(nodes[i], nodes[j]);
            cost = 1. / distance;

            distances[i][j] = distance;
            distances[j][i] = distance;
            

            edgeCosts[i][j] = cost;
            edgeCosts[j][i] = cost;
        }
    }
}

TSPResults AntColony::TSPAnt() {
    
    Initialize();

    float shortestDistance = std::numeric_limits<float>::max();
    std::vector<unsigned short> shortestPath(nodeNum + 1, -1);

    unsigned long long runtime;
    const auto clockStart = std::chrono::system_clock::now();
    long iterations = 0;

    do{
        // Ant movements
        for (Ant& ant : ants) {
            // Resetting values
            int tourStartNode;
            unsigned short i = 1;
            ant.cumulativeDistance = 0;
            ant.currentNode = rand() % nodeNum;
            ant.unvisitedNodes = nodesID;
            ant.visitedNodes[0] = ant.currentNode;
            ant.unvisitedNodes.erase(ant.unvisitedNodes.begin() + ant.currentNode);
            tourStartNode = ant.currentNode;
            

            while (ant.unvisitedNodes.size() > 0) {
                // Initializing values
                float maxAtt = -1;
                unsigned short from = ant.currentNode, nextNode = 0, x = 0, nodePos = 0;
                

                // Checking attractiveness of every possible route, and keeping the record of the best solution so far
                for (auto& to : ant.unvisitedNodes) {
                    float routeAttractiveness = 10000 * std::pow(edgePheromones[from][to], pheromoneFactor) * std::pow(edgeCosts[from][to], distanceFactor); 
                    if (routeAttractiveness > maxAtt) {
                        maxAtt = routeAttractiveness;
                        nextNode = to;
                        nodePos = x;
                    }
                    x++;
                }


                // Applying node choice, updating distance
                ant.currentNode = nextNode;
                ant.visitedNodes[i++] = nextNode;
                ant.cumulativeDistance += distances[from][nextNode];
                ant.unvisitedNodes.erase(ant.unvisitedNodes.begin()+nodePos);
            }

            // Coming back to the starting node, updating distance
            ant.currentNode = ant.visitedNodes[ant.visitedNodes.size() - 2];
            ant.visitedNodes[ant.visitedNodes.size() - 1] = tourStartNode;
            ant.cumulativeDistance += distances[ant.currentNode][tourStartNode];

            // Checking if provided solution is better than the current best one, if so: updating
            if (ant.cumulativeDistance < shortestDistance) {
                shortestDistance = ant.cumulativeDistance;
                shortestPath = ant.visitedNodes;
            }
        }


        // Updating pheromone levels on every edge - decreasing due to evaporation
        for (unsigned short i = 0; i < nodeNum; i++) {
            for (unsigned short j = 0; j < nodeNum; j++) {
                if (i == j)
                    break;

                edgePheromones[i][j] *= evaporationRate;
                edgePheromones[j][i] *= evaporationRate;
            }
        }

        // Updating pheromone levels on every edge - increasing due to trail usage
        for (auto& ant : ants) {
            for (auto b = ant.visitedNodes.begin(); b < std::prev(ant.visitedNodes.end()); b++) {
                edgePheromones[*b][*std::next(b)] += pheromoneAddingRate / ant.cumulativeDistance;
                edgePheromones[*std::next(b)][*b] += pheromoneAddingRate / ant.cumulativeDistance;
            }
        }
      

        runtime = std::chrono::duration<double, std::milli>(std::chrono::system_clock::now() - clockStart).count();
        iterations++;

    } while (runtime < setRuntime);


    return { iterations, shortestDistance, shortestPath };
}