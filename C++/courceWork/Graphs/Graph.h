//
// Created by hal4dan on 20.09.2019.
//

#ifndef COURCEWORK_GRAPH_H
#define COURCEWORK_GRAPH_H
#include <stdio.h>
#include <iostream>
#include "Weight.h"

using namespace std;

class Graph {
private:
    int** graph;
    int size;

public:
    Graph(const int);
    ~Graph();
    Graph(int**, const int);
    Graph(Graph*);

    void addElem(int, int, int);
    void setElement(int, int, int);
    const int getSize();
    int** getGraph();
    int getElem(int, int);
    bool bfs(int**, int, int, int[]);
    int fordFulkerson(int, int);
    int localEdgeConnectivity(int, int);
    int globalEdgeConnectivity();
    Graph* separationVertex();
    Graph* adjacencyMatrix();
    int getPowerVertex(int);
    int localVertexConnectivity(int, int);
    int globalVertexConnectivity();
    void fill(double, vector<Weight>);
    void nullGraph();
    friend ostream& operator<<(ostream& os, Graph* gr) {
        for (int i = 0; i < gr->size; i++) {
            for (int j = 0; j < gr->size; j++) {
                os << gr->graph[i][j] << "  ";
            }
            os << endl;
        }
        return os;
    };

};



#endif //COURCEWORK_GRAPH_H
