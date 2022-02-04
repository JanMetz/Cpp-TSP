#include <iostream>
#include <vector>
#include <fstream>
#include "AntColony.h"


std::vector<Node> LoadNodes(std::string testPath) {
    std::ifstream testFile;
    testFile.open(testPath);
    if (testFile.is_open() == false) {
        std::cout << "Coulnd't open the file\n";
        exit(1);
    }
    int N, vertexIndex;
    int vx, vy;
    testFile >> N;

    std::vector<Node> nodes(N);
    for (int i = 0; i < N; i++){
        testFile >> vertexIndex >> vx >> vy;
        nodes[i] = Node{ vx, vy };
    }
    return nodes;
}

int main() {
    srand(time(NULL));

    std::string testPath;  //path to chosen instance
    float maxTime; //maximum time of simulation in seconds

    std::cout << "Please, specify the path to instance of your choice:\n";
    std::getline(std::cin, testPath);
 
    std::cout << "Please, specify the time (in seconds) you wish the simulation to run:\n";
    std::cin >> maxTime;

    std::vector<Node> nodes = LoadNodes(testPath);

    AntColony a(nodes, maxTime);

    TSPResults results = a.TSPAnt();


    std::cout << "Minimal distance: \t " << results.distance << "\n";
    std::cout << "Number of nodes in test case: \t" << nodes.size() << "\n";
    std::cout << "Iterations: " << results.iterations<< "\n";
    std::cout << "Coordinates of consecutive points:\nX\tY\n";
    for (int node : results.path) {
        std::cout << nodes[node].x << ";" << nodes[node].y << "\n";
    }

}