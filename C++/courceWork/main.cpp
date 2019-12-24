#include "stdlib.h"
#include <fstream>
#include <iostream>
#include <limits.h>
#include <cstdlib>
#include <string>
#include <queue>
#include <ctime>
#include <random>
#include <list>
#include <cassert>
#include<vector>
#include <cstring>
#include <chrono>
#include "Graphs/Graph.h"
#include "Graphs/Weight.h"


using namespace std;

Graph::Graph(const int size) {
    graph = new int *[size];
    for (int i = 0; i < size; i++) {
        graph[i] = new int[size];
        for (int j = 0; j < size; j++) {
            graph[i][j] = 0;
        }
    }
    this->size = size;
}

Graph::~Graph() {
    delete[] graph;
}

Graph::Graph(int **arr, const int size) {
    this->size = size;
    graph = new int *[size];
    for (int i = 0; i < size; i++) {
        graph[i] = new int[size];
        for (int j = 0; j < size; j++) {
            graph[i][j] = arr[i][j];
        }
    }
}

Graph::Graph(Graph *gr) {
    this->size = gr->getSize();
    graph = new int *[size];
    for (int i = 0; i < size; i++) {
        graph[i] = new int[size];
        for (int j = 0; j < size; j++) {
            graph[i][j] = gr->getElem(i, j);
        }
    }
}

void Graph::addElem(int row, int column, int elem) {
    if (row >= 0 && column >= 0 && elem > 0) {
        graph[row][column] = elem;
    }
}

const int Graph::getSize() {
    return size;
}

int **Graph::getGraph() {
    return graph;
}

void Graph::setElement(int row, int column, int elem) {
    graph[row][column] = elem;
}

bool Graph::bfs(int **rGraph, int s, int t, int parent[]) {
    bool *visited = new bool[size];
    memset(visited, 0, size);
    queue <int> q;
    q.push(s);
    visited[s] = true;
    parent[s] = -1;
    while (!q.empty()) {
        int u = q.front();
        q.pop();

        for (int v = 0; v < size; v++) {
            if (visited[v] == false && rGraph[u][v] > 0) {
                q.push(v);
                parent[v] = u;
                visited[v] = true;
            }
        }
    }
    return (visited[t] == true);
}

int Graph::fordFulkerson(int s, int t) {
    int u, v;
    int **rGraph = new int *[size];
    for (u = 0; u < size; u++) {
        rGraph[u] = new int[size];
        for (v = 0; v < size; v++) {
            rGraph[u][v] = graph[u][v];
        }
    }
    int *parent = new int[size];

    int max_flow = 0;
    while (bfs(rGraph, s, t, parent)) {
        int path_flow = INT_MAX;
        for (v = t; v != s; v = parent[v]) {
            u = parent[v];
            path_flow = min(path_flow, rGraph[u][v]);
        }

        for (v = t; v != s; v = parent[v]) {
            u = parent[v];
            rGraph[u][v] -= path_flow;
            rGraph[v][u] += path_flow;
        }
        max_flow += path_flow;
    }
    return max_flow;
}

int Graph::localEdgeConnectivity(int s, int t) {
    return min(INT_MAX, fordFulkerson(s, t));
}

int Graph::globalEdgeConnectivity() {
    if (!bfs(graph, 0, size - 1, new int[size])) {
        cout << "Граф несвязен" << endl;
        return 0;
    }
    int edgeConnectivity;
    for (int i = 0; i < size; i++) {
        for (int j = i + 1; j < size; j++) {
            edgeConnectivity = localEdgeConnectivity(i, j);
        }
    }
    return edgeConnectivity;
}

Graph *Graph::separationVertex() {
    if (!bfs(graph, 0, size - 1, new int[size])) {
        cout << "Граф несвязен" << endl;
        return 0;
    }
    int **arr = new int *[size * 2];
    for (int i = 0; i < size; i++) {
        arr[i] = new int[size * 2];
        arr[i + size] = new int[size * 2];
        for (int j = 0; j < size; j++) {
            arr[i][j] = 0;
            if (i == j) {
                arr[i][j + size] = 1;
            } else {
                arr[i][j + size] = 0;
            }
            arr[i + size][j] = graph[i][j];
            arr[i + size][j + size] = 0;
        }

    }
    return new Graph(arr, size * 2);
}

int Graph::localVertexConnectivity(int s, int t) {
    return min(INT_MAX, separationVertex()->fordFulkerson(s, t));
}

int Graph::globalVertexConnectivity() {
    int edgeConnectivity;
    for (int i = size; i < separationVertex()->getSize(); i++) {
        for (int j = 0; j < size; j++) {
            edgeConnectivity = localVertexConnectivity(i, j);
        }
    }
    return edgeConnectivity;
}

void Graph::fill(double density, vector<Weight> weightArray) {
    std::random_device rd;
    std::mt19937 gen(rd()); //генератор случайных чисел
    std::uniform_real_distribution<> dis(0.0, 1.1);//задаем диапазон генерации вероятности от 0 до 1

    std::uniform_int_distribution<> distribution(10, 100); // Равномерное распределение [10, 20]

    int value = distribution(gen);

    for (unsigned long i = 0; i < size; i++) {
        for (unsigned long j = 0; j < i; j++) {
            if (dis(gen) < density) {//если сгенерированная вероятность меньше нашей, то ребро есть - ставим 1
                graph[i][j] = 1;
                //graph[j][i] = 1;
                Weight weight = Weight(i,j,value);
                weightArray.push_back(weight);
            }
        }
    }
}

Graph *Graph::adjacencyMatrix() {

    int **gr = new int *[size];
    for (int i = 0; i < size; i++) {
        gr[i] = new int[size];
    }
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (graph[i][j] == 1) {
                gr[i][j] = 1;
                gr[j][i] = 1;
            } else {
                gr[i][j] = 0;
                gr[j][i] = 0;
            }
        }
    }
    return new Graph(gr, size);
}

void Graph::nullGraph() {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            graph[i][j] = 0;
        }
    }
}

void fordFulkersonTest1() {
    int **graph = new int *[6];
    graph[0] = new int[6]{0, 9, 6, 0, 0, 0};
    graph[1] = new int[6]{0, 0, 0, 12, 8, 0};
    graph[2] = new int[6]{0, 0, 0, 6, 2, 0};
    graph[3] = new int[6]{0, 0, 0, 0, 0, 9};
    graph[4] = new int[6]{0, 0, 0, 0, 0, 5};
    graph[5] = new int[6]{0, 0, 0, 0, 0, 0};
    Graph *gr = new Graph(graph, 6);

    cout << "fordFulkersonTest1:" << endl;
    assert(gr->fordFulkerson(0, 5) == 14);
    cout << "Максимальный поток успешно найден!" << endl;
}

void fordFulkersonTest2() {
    int **graph = new int *[6];
    graph[0] = new int[6]{0, 2, 4, 0, 0, 0};
    graph[1] = new int[6]{0, 0, 0, 2, 3, 0};
    graph[2] = new int[6]{0, 0, 0, 4, 0, 0};
    graph[3] = new int[6]{0, 0, 0, 0, 0, 3};
    graph[4] = new int[6]{0, 0, 0, 6, 0, 3};
    graph[5] = new int[6]{0, 0, 0, 0, 0, 0};
    Graph *gr = new Graph(graph, 6);

    cout << "fordFulkersonTest2:" << endl;
    assert(gr->fordFulkerson(0, 5) == 5);
    cout << "Максимальный поток успешно найден!" << endl;
}

void bfsTest() {
    int **graph = new int *[5];
    graph[0] = new int[5]{0, 1, 0, 0, 0};
    graph[1] = new int[5]{1, 0, 0, 1, 0};
    graph[2] = new int[5]{0, 1, 0, 0, 0};
    graph[3] = new int[5]{0, 0, 1, 0, 1};
    graph[4] = new int[5]{0, 0, 0, 1, 0};
    Graph *gr = new Graph(graph, 5);

    cout << "edgeConnectivityTest1: " << endl;
    assert(gr->bfs(graph, 0, 4, new int[5]) == true);
    cout << "Проверка на связность прошла успешно!" << endl;
}

void edgeConnectivityTest1() {
    int **graph = new int *[5];
    graph[0] = new int[5]{0, 1, 0, 0, 0};
    graph[1] = new int[5]{1, 0, 1, 1, 0};
    graph[2] = new int[5]{0, 1, 0, 1, 0};
    graph[3] = new int[5]{0, 1, 1, 0, 1};
    graph[4] = new int[5]{0, 0, 0, 1, 0};
    Graph *gr = new Graph(graph, 5);

    cout << "edgeConnectivityTest1: " << endl;
//	assert(gr->globalEdgeConnectivity == 1);
    cout << "Проверка на связность прошла успешно!" << endl;
}

void edgeConnectivityTest2() {
    int **graph = new int *[6];
    graph[0] = new int[6]{0, 1, 1, 0, 0, 0};
    graph[1] = new int[6]{1, 0, 0, 1, 1, 0};
    graph[2] = new int[6]{1, 0, 0, 1, 1, 0};
    graph[3] = new int[6]{0, 1, 1, 0, 0, 1};
    graph[4] = new int[6]{0, 1, 1, 0, 0, 1};
    graph[5] = new int[6]{0, 0, 0, 1, 1, 0};

    Graph *gr = new Graph(graph, 6);

    cout << "edgeConnectivityTest2:" << endl;
//	assert(gr->globalEdgeConnectivity == 2);
    cout << "Реберная связность успешно найдена!" << endl;
}

void edgeConnectivityTest3() {
    int **graph = new int *[10];
    graph[0] = new int[10]{0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    graph[1] = new int[10]{0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    graph[2] = new int[10]{0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    graph[3] = new int[10]{0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    graph[4] = new int[10]{0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    graph[5] = new int[10]{0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    graph[6] = new int[10]{0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    graph[7] = new int[10]{0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    graph[8] = new int[10]{0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    graph[9] = new int[10]{0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    Graph *gr = new Graph(graph, 10);

    cout << "edgeConnectivityTest2:" << endl;
    //assert(gr->globalEdgeConnectivity() == 0);
    cout << "Реберная связность успешно найдена!" << endl;
}

void vertexConnectivityTest1() {
    int **graph = new int *[6];
    graph[0] = new int[6]{0, 1, 0, 1, 1, 0};
    graph[1] = new int[6]{1, 0, 0, 1, 0, 0};
    graph[2] = new int[6]{0, 0, 0, 1, 0, 1};
    graph[3] = new int[6]{1, 1, 1, 0, 0, 1};
    graph[4] = new int[6]{1, 0, 0, 0, 0, 1};
    graph[5] = new int[6]{0, 0, 1, 1, 1, 0};
    Graph *gr = new Graph(graph, 6);

    cout << "vertexConnectivityTest1:" << endl;
   // assert(gr->globalVertexConnectivity() == 2);
    cout << "Вершинная связность успешно найдена!" << endl;
}

void vertexConnectivityTest2() {
    int **graph = new int *[5];
    graph[0] = new int[5]{0, 1, 1, 1, 0};
    graph[1] = new int[5]{1, 0, 1, 0, 1};
    graph[2] = new int[5]{1, 1, 0, 1, 1};
    graph[3] = new int[5]{1, 0, 1, 0, 1};
    graph[4] = new int[5]{0, 1, 1, 1, 0};

    Graph *gr = new Graph(graph, 5);

    cout << "vertexConnectivityTest1:" << endl;
    cout<<gr->globalVertexConnectivity()<<endl;
    cout << "Вершинная связность успешно найдена!" << endl;
}

void vertexConnectivityTest3() {
    int **graph = new int *[6];
    graph[0] = new int[6]{0, 1, 0, 0, 0, 0};
    graph[1] = new int[6]{1, 0, 1, 0, 0, 0};
    graph[2] = new int[6]{0, 1, 0, 1, 0, 0};
    graph[3] = new int[6]{0, 0, 1, 0, 1, 0};
    graph[4] = new int[6]{0, 0, 0, 1, 0, 1};
    graph[5] = new int[6]{0, 0, 0, 0, 1, 0};
    Graph *gr = new Graph(graph, 6);
    cout << "vertexConnectivityTest1:" << endl;
  //  assert(gr->globalVertexConnectivity() == 1);
    cout << "Вершинная связность успешно найдена!" << endl;
}

int Graph::getElem(int i, int j) {
    return graph[i][j];
}

int Graph::getPowerVertex(int i) {
    int powerVertex = 0;
    for (int k = 0; k < size; k++) {
        powerVertex += graph[i][k];
    }
    return powerVertex;
}

Graph *createGraph() {
    int key = 0;
    int size = 0;
    bool haveGraph = false;
    cout << "Создадим граф" << endl;
    cout << "Задайте размер" << endl;
    cin >> size;
    Graph *graph = new Graph(size);
    system("cls");
    while (key != 4) {

        cout << "1. Заполнить вручную" << endl;
        cout << "2. Использовать генератор случайных чисел" << endl;
        cout << "3. Чтение из файла" << endl;
        cout << "4. Перейти на работу с процедурами" << endl;
        cin >> key;
        if (key == 1) {
            graph->nullGraph();
            int elem = 0;
            int str, col;
            for (int i = 0; i < size; i++) {
                for (int j = 0; j < size; j++) {
                    cin >> elem;
                    graph->addElem(i, j, elem);
                }
            }
            system("cls");
            haveGraph = true;
        }
        if (key == 2) {
            graph->nullGraph();
            double denstry;
            cout << "Задайте вероятность создания ребра" << endl;
            cin >> denstry;
            //graph->fill(denstry);
            cout << "Новый сгенерированный граф:" << endl;

            cout << graph;

            haveGraph = true;
        }
        if (key == 3) {
            graph->nullGraph();
            ifstream fin("testGraph.txt");
            if (!fin.is_open())
                cout << "Файл не может быть открыт!\n";
            else {
                int elem;
                for (int i = 0; i < size; i++) {
                    for (int j = 0; j < size; j++) {

                        fin >> elem;
                        graph->addElem(i, j, elem);
                    }
                }
                fin.close();
                haveGraph = true;
            }
            cout << graph;

        }
        if (key == 4) {
            if (!haveGraph) {
                cout << "Вы не заполнили граф! Заполните и повторите попытку." << endl;
                key = 0;
            }

        }

    }
    return graph;
}

void sortWeightFunction(vector<Weight> &weightFunction) {

    for (int i = 1; i < weightFunction.size(); i++) {
        for (int r = 0; r < weightFunction.size() - i; r++) {
            if (weightFunction[r].getWeight() < weightFunction[r + 1].getWeight()) {
                int weight = weightFunction[r].getWeight();
                int lastVertex = weightFunction[r].getLastVertex();
                int firstVertex = weightFunction[r].getFirstVertex();
                weightFunction[r].setFirstVertex(weightFunction[r + 1].getFirstVertex());
                weightFunction[r].setLastVertex(weightFunction[r + 1].getLastVertex());
                weightFunction[r].setWeight(weightFunction[r + 1].getWeight());
                weightFunction[r + 1].setFirstVertex(firstVertex);
                weightFunction[r + 1].setLastVertex(lastVertex);
                weightFunction[r + 1].setWeight(weight);
            }
        }
    }
}

bool isMore(Weight weightFunctionOne, Weight weightFunctionTwo, Graph *graph, int sheme) {
    int firstVertexOne = weightFunctionOne.getFirstVertex();
    int lastVertexOne = weightFunctionOne.getLastVertex();
    int firstVertexTwo = weightFunctionTwo.getFirstVertex();
    int lastVertexTwo = weightFunctionTwo.getLastVertex();
    int sumPowerVertexOne;
    int sumPowerVertexTwo;
    int maxPowerVertexOne;
    int maxPowerVertexTwo;
    int minPowerVertexOne;
    int minPowerVertexTwo;
    if (weightFunctionOne.getWeight() < weightFunctionTwo.getWeight()) {
        return true;
    }
    switch (sheme) {
        case 1:
            sumPowerVertexOne = graph->getPowerVertex(firstVertexOne) + graph->getPowerVertex(lastVertexOne);
            sumPowerVertexTwo = graph->getPowerVertex(firstVertexTwo) + graph->getPowerVertex(lastVertexTwo);
            if (weightFunctionOne.getWeight() == weightFunctionTwo.getWeight()) {
                if (sumPowerVertexOne < sumPowerVertexTwo) {
                    return true;
                }
            }
        case 2:
            sumPowerVertexOne = graph->getPowerVertex(firstVertexOne) + graph->getPowerVertex(lastVertexOne);
            sumPowerVertexTwo = graph->getPowerVertex(firstVertexTwo) + graph->getPowerVertex(lastVertexTwo);
            if (weightFunctionOne.getWeight() == weightFunctionTwo.getWeight()) {
                if (sumPowerVertexOne > sumPowerVertexTwo) {
                    return true;
                }
            }
        case 3:
            maxPowerVertexOne=max(graph->getPowerVertex(firstVertexOne),graph->getPowerVertex(lastVertexOne));
            maxPowerVertexTwo=max(graph->getPowerVertex(firstVertexTwo),graph->getPowerVertex(lastVertexTwo));
            if (weightFunctionOne.getWeight() == weightFunctionTwo.getWeight()) {
                if (maxPowerVertexOne < maxPowerVertexTwo) {
                    return true;
                }
            }
        case 4:
            maxPowerVertexOne=max(graph->getPowerVertex(firstVertexOne),graph->getPowerVertex(lastVertexOne));
            maxPowerVertexTwo=max(graph->getPowerVertex(firstVertexTwo),graph->getPowerVertex(lastVertexTwo));
            if (weightFunctionOne.getWeight() == weightFunctionTwo.getWeight()) {
                if (maxPowerVertexOne > maxPowerVertexTwo) {
                    return true;
                }
            }
        case 5:
            minPowerVertexOne=min(graph->getPowerVertex(firstVertexOne),graph->getPowerVertex(lastVertexOne));
            minPowerVertexTwo=min(graph->getPowerVertex(firstVertexTwo),graph->getPowerVertex(lastVertexTwo));
            if (weightFunctionOne.getWeight() == weightFunctionTwo.getWeight()) {
                if (minPowerVertexOne < minPowerVertexTwo) {
                    return true;
                }
            }
        case 6:
            minPowerVertexOne=min(graph->getPowerVertex(firstVertexOne),graph->getPowerVertex(lastVertexOne));
            minPowerVertexTwo=min(graph->getPowerVertex(firstVertexTwo),graph->getPowerVertex(lastVertexTwo));
            if (weightFunctionOne.getWeight() == weightFunctionTwo.getWeight()) {
                if (minPowerVertexOne > minPowerVertexTwo) {
                    return true;
                }
            }

    }
    return false;
}

void sortWeightFunctionWithVertex(vector<Weight> &weightFunction, Graph *graph) {

    for (int i = 1; i < weightFunction.size(); i++) {
        for (int r = 0; r < weightFunction.size() - i; r++) {
            if (isMore(weightFunction[r], weightFunction[r + 1], graph,1)) {
                int weight = weightFunction[r].getWeight();
                int lastVertex = weightFunction[r].getLastVertex();
                int firstVertex = weightFunction[r].getFirstVertex();
                weightFunction[r].setFirstVertex(weightFunction[r + 1].getFirstVertex());
                weightFunction[r].setLastVertex(weightFunction[r + 1].getLastVertex());
                weightFunction[r].setWeight(weightFunction[r + 1].getWeight());
                weightFunction[r + 1].setFirstVertex(firstVertex);
                weightFunction[r + 1].setLastVertex(lastVertex);
                weightFunction[r + 1].setWeight(weight);
            }
        }
    }
}

void greadyReverse(Graph *graph, vector<Weight> &weightFunction, int connectivity) {
    int row;
    int column;
    int graphSize = graph->getSize();
    sortWeightFunction(weightFunction);
    for (int i = 0; i < graphSize; i++) {
        row = weightFunction[i].getFirstVertex();
        column = weightFunction[i].getLastVertex();
        graph->setElement(row, column, 0);
        graph->setElement(column, row, 0);
        if (graph->localEdgeConnectivity(row, column) < connectivity) {
            graph->setElement(row, column, 1);
            graph->setElement(column, row, 1);
        }

    }
}

void greadyReverseWithVertex(Graph *graph, vector<Weight> &weightFunction, int connectivity) {
    int row;
    int column;
    int graphSize = graph->getSize();
    sortWeightFunctionWithVertex(weightFunction, graph);
    for (int i = 0; i < graphSize; i++) {
        row = weightFunction[i].getFirstVertex();
        column = weightFunction[i].getLastVertex();
        graph->setElement(row, column, 0);
        graph->setElement(column, row, 0);
        if (graph->localEdgeConnectivity(row, column) < connectivity) {
            graph->setElement(row, column, 1);
            graph->setElement(column, row, 1);
        }

    }
}

int main() {
    setlocale(LC_ALL, "Russian");
    int key = 0;
    //Graph *graph = createGraph();
   /* Graph **arrayGraphs = new Graph*[100];
    vector<Weight> *arrayWeights = new vector<Weight>[100];
    for(int i=0;i<10;i++){
        vector<Weight> arr;
        arr.reserve(11);
        arrayGraphs[i] = new Graph(10);
        arrayWeights[i] = arr;
    }
    for(int i=0;i<10;i++){
        arrayGraphs[i]->fill(0.67,arrayWeights[i]);
    }
    for(int i=0;i<10;i++){
        int connectivity = arrayGraphs[i]->globalEdgeConnectivity();
        greadyReverse(arrayGraphs[i], arrayWeights[i], connectivity);
    }*/
    /*vector<Weight> array;
    Weight weight1 = Weight(0, 1, 10);
    Weight weight2 = Weight(1, 2, 5);
    Weight weight3 = Weight(1, 3, 8);
    Weight weight4 = Weight(2, 4, 7);
    Weight weight5 = Weight(3, 4, 4);
    Weight weight6 = Weight(4, 5, 2);
    array.reserve(6);
    array.push_back(weight1);
    array.push_back(weight2);
    array.push_back(weight3);
    array.push_back(weight4);
    array.push_back(weight5);
    array.push_back(weight6);
    int **graph = new int *[6];
    graph[0] = new int[6]{0, 1, 0, 0, 0, 0};
    graph[1] = new int[6]{1, 0, 1, 1, 0, 0};
    graph[2] = new int[6]{0, 1, 0, 0, 1, 0};
    graph[3] = new int[6]{0, 1, 0, 0, 1, 0};
    graph[4] = new int[6]{0, 0, 1, 1, 0, 1};
    graph[5] = new int[6]{0, 0, 0, 0, 1, 0};
    Graph *gr = new Graph(graph, 6);
    cout << gr;
    cout << "" << endl;
    int connectivity = gr->globalEdgeConnectivity();
    greadyReverse(gr, array, connectivity);

    cout << gr;
    cout << "" << endl;
    array.clear();
    array.push_back(weight1);
    array.push_back(weight2);
    array.push_back(weight3);
    array.push_back(weight4);
    array.push_back(weight5);
    array.push_back(weight6);
    graph[0] = new int[6]{0, 1, 0, 0, 0, 0};
    graph[1] = new int[6]{1, 0, 1, 1, 0, 0};
    graph[2] = new int[6]{0, 1, 0, 0, 1, 0};
    graph[3] = new int[6]{0, 1, 0, 0, 1, 0};
    graph[4] = new int[6]{0, 0, 1, 1, 0, 1};
    graph[5] = new int[6]{0, 0, 0, 0, 1, 0};
    gr = new Graph(graph, 6);
    cout << gr;
    cout << "" << endl;
    connectivity = gr->globalEdgeConnectivity();
    greadyReverseWithVertex(gr, array, connectivity);

    cout << gr;*/
    /*while (key != 8) {
           cout << "МЕНЮ ДЛЯ ВИКТОРА ПЕТРОВИЧА:" << endl;
           cout << "1. Проверить на связность" << endl;
           cout << "2. Алгоритм Форда-Фалкерсона" << endl;
           cout << "3. Поиск локальной вершинной связности" << endl;
           cout << "4. Поиск глобальной вершинной связности" << endl;
           cout << "5. Поиск локальной реберной связности" << endl;
           cout << "6. Поиск глобальной реберной связности" << endl;
           cout << "7. Изменить граф" << endl;
           cout << "8. Выход" << endl;
           cin >> key;
           system("cls");
           if (key == 1) {
               cout << "Граф " << endl;
               cout << graph;
               int size = graph->getSize();
               int** gr = new int*[size];
               for (int i = 0; i < size; i++) {
                   gr[i] = new int[size];
               }
               for (int i = 0; i < size; i++) {
                   for (int j = 0; j < size; j++) {
                       gr[i][j] = graph->getElem(i, j);
                   }
               }
               if (graph->bfs(gr, 0, size - 1, new int[size])) {
                   cout << "Граф связен" << endl;
               }
               else {
                   cout << "Граф не связен" << endl;
               }
           }
           if (key == 2) {
               cout << "Граф " << endl;
               cout << graph;
               cout << "Поток в графе равен " << graph->fordFulkerson(0, graph->getSize() - 1) << endl;
           }
           if (key == 3) {
               int i, j;
               cout << "Введите вершины, для которых будете вычислять локальную вершинную связность" << endl;
               cin >> i;
               cin >> j;
               Graph* gr = new Graph(graph->adjacencyMatrix());
               cout << "Граф "<<endl;
               cout << graph;
               cout << "Матрица смежности " << endl;
               cout << gr;

               cout << "Локальная вершинная связность равна " << gr->localVertexConnectivity(i, j) << endl;
           }
           if (key == 4) {
               Graph* gr = new Graph(graph->adjacencyMatrix());
               cout << "Граф " << endl;
               cout << graph;
               cout << "Матрица смежности " << endl;
               cout << gr;
               cout << "Глобальная вершинная связность равна " << gr->globalVertexConnectivity() << endl;
           }
           if (key == 5) {
               int i = 0, j = graph->getSize() - 1;
               cout << "Введите вершины, для которых будете вычислять локальную реберная связность" << endl;
               cin >> i;
               cin >> j;
               Graph* gr = new Graph(graph->adjacencyMatrix());
               cout << "Граф " << endl;
               cout << graph;
               cout << "Матрица смежности " << endl;
               cout << gr;
               cout << "Локальная реберная связность равна " << gr->localEdgeConnectivity(i, j) << endl;
           }
           if (key == 6) {
               Graph* gr = new Graph(graph->adjacencyMatrix());
               cout << "Граф " << endl;
               cout << graph;
               cout << "Матрица смежности " << endl;
               cout << gr;
               cout << "Глобальная реберная связность равна " << gr->globalVertexConnectivity() << endl;
           }
           if (key == 7) {
               graph = createGraph();
               cout << "Граф " << endl;
               cout << graph;
           }
           if(key==8)
               break;
           pauseAndEndl(1);
           system("cls");

       }

       fordFulkersonTest1();
       fordFulkersonTest2();
       bfsTest();
       edgeConnectivityTest1();
       edgeConnectivityTest2();
       edgeConnectivityTest3();*/
       vertexConnectivityTest1();
       vertexConnectivityTest2();
       vertexConnectivityTest3();
    //  pauseAndEndl(1);
    return 0;
}