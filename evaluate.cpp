#include <iostream>
#include <chrono>
#include <string>

#include "graph.h"
#include "graphGenerator.h"
#include "dijkstra.h"
#include "prim.h"
#include "fibonacciHeap.h"
#include "pairingHeap.h"

using namespace std;

//run dijkstra and time it
//pass "fibonacci" or "pairing" for heapType
double timeDijkstra(const Graph& g, int source, const string& heapType) {
    auto start = chrono::high_resolution_clock::now();

    if (heapType == "fibonacci") {
        FibonacciHeap<long long, int> pq;
        dijkstra(g, source, pq);
    } else {
        PairingHeap<long long, int> pq;
        dijkstra(g, source, pq);
    }

    auto end = chrono::high_resolution_clock::now();
    return chrono::duration<double, milli>(end - start).count();
}

//same thing but for prim
double timePrim(const Graph& g, int source, const string& heapType) {
    auto start = chrono::high_resolution_clock::now();

    if (heapType == "fibonacci") {
        FibonacciHeap<long long, int> pq;
        prim_mst(g, source, pq);
    } else {
        PairingHeap<long long, int> pq;
        prim_mst(g, source, pq);
    }

    auto end = chrono::high_resolution_clock::now();
    return chrono::duration<double, milli>(end - start).count();
}

//count edges, undirected edges get counted twice so divide by 2
int countEdges(const Graph& g, bool directed) {
    int total = 0;
    for (int i = 0; i < g.num_vertices(); i++) {
        total += g.neighbors(i).size();
    }
    return directed ? total : total / 2;
}

int main() {
    //csv header
    cout << "algorithm,heap,graph_type,n,edges,time_ms" << endl;

    vector<int> sizes = {1000, 5000, 10000, 50000};
    vector<string> heaps = {"fibonacci", "pairing"};

    for (int n : sizes) {
        //need directed for dijkstra and undirected for prim
        Graph sparseDi = generateRandom(n, true, 3 * n);
        Graph denseDi = generateRandom(n, true, n * n / 4);
        int gridSide = static_cast<int>(sqrt(n));
        Graph gridDi = generateGrid(gridSide, gridSide, true);

        Graph sparseUn = generateRandom(n, false, 3 * n);
        Graph denseUn = generateRandom(n, false, n * n / 4);
        Graph gridUn = generateGrid(gridSide, gridSide, false);

        //dijkstra tests
        for (const string& heap : heaps) {
            double t;

            t = timeDijkstra(sparseDi, 0, heap);
            cout << "dijkstra," << heap << ",sparse," << n << "," << countEdges(sparseDi, true) << "," << t << endl;

            t = timeDijkstra(denseDi, 0, heap);
            cout << "dijkstra," << heap << ",dense," << n << "," << countEdges(denseDi, true) << "," << t << endl;

            int gridN = gridSide * gridSide;
            t = timeDijkstra(gridDi, 0, heap);
            cout << "dijkstra," << heap << ",grid," << gridN << "," << countEdges(gridDi, true) << "," << t << endl;
        }

        //prim tests
        for (const string& heap : heaps) {
            double t;

            t = timePrim(sparseUn, 0, heap);
            cout << "prim," << heap << ",sparse," << n << "," << countEdges(sparseUn, false) << "," << t << endl;

            t = timePrim(denseUn, 0, heap);
            cout << "prim," << heap << ",dense," << n << "," << countEdges(denseUn, false) << "," << t << endl;

            int gridN = gridSide * gridSide;
            t = timePrim(gridUn, 0, heap);
            cout << "prim," << heap << ",grid," << gridN << "," << countEdges(gridUn, false) << "," << t << endl;
        }
    }

    return 0;
}