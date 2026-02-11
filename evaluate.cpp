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

struct BenchResult {
    double time_ms;
    int inserts;
    int extracts;
    int decreaseKeys;
};

//run dijkstra and time it
//pass "fibonacci" or "pairing" for heapType
BenchResult runDijkstra(const Graph& g, int source, const string& heapType) {
    BenchResult res;

    if (heapType == "fibonacci") {
        FibonacciHeap<long long, int> pq;
        auto start = chrono::high_resolution_clock::now();
        dijkstra(g, source, pq);
        auto end = chrono::high_resolution_clock::now();
        res.time_ms = chrono::duration<double, milli>(end - start).count();
        res.inserts = pq.insertCount;
        res.extracts = pq.extractCount;
        res.decreaseKeys = pq.decreaseKeyCount;
    } else {
        PairingHeap<long long, int> pq;
        auto start = chrono::high_resolution_clock::now();
        dijkstra(g, source, pq);
        auto end = chrono::high_resolution_clock::now();
        res.time_ms = chrono::duration<double, milli>(end - start).count();
        res.inserts = pq.insertCount;
        res.extracts = pq.extractCount;
        res.decreaseKeys = pq.decreaseKeyCount;
    }

    return res;
}

//same thing but for prim
BenchResult runPrim(const Graph& g, int source, const string& heapType) {
    BenchResult res;

    if (heapType == "fibonacci") {
        FibonacciHeap<long long, int> pq;
        auto start = chrono::high_resolution_clock::now();
        prim_mst(g, source, pq);
        auto end = chrono::high_resolution_clock::now();
        res.time_ms = chrono::duration<double, milli>(end - start).count();
        res.inserts = pq.insertCount;
        res.extracts = pq.extractCount;
        res.decreaseKeys = pq.decreaseKeyCount;
    } else {
        PairingHeap<long long, int> pq;
        auto start = chrono::high_resolution_clock::now();
        prim_mst(g, source, pq);
        auto end = chrono::high_resolution_clock::now();
        res.time_ms = chrono::duration<double, milli>(end - start).count();
        res.inserts = pq.insertCount;
        res.extracts = pq.extractCount;
        res.decreaseKeys = pq.decreaseKeyCount;
    }

    return res;
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
    cout << "algorithm,heap,graph_type,n,edges,time_ms,inserts,extracts,decrease_keys" << endl;

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
            BenchResult r;

            r = runDijkstra(sparseDi, 0, heap);
            cout << "dijkstra," << heap << ",sparse," << n << "," << countEdges(sparseDi, true) << "," << r.time_ms << "," << r.inserts << "," << r.extracts << "," << r.decreaseKeys << endl;

            r = runDijkstra(denseDi, 0, heap);
            cout << "dijkstra," << heap << ",dense," << n << "," << countEdges(denseDi, true) << "," << r.time_ms << "," << r.inserts << "," << r.extracts << "," << r.decreaseKeys << endl;

            int gridN = gridSide * gridSide;
            r = runDijkstra(gridDi, 0, heap);
            cout << "dijkstra," << heap << ",grid," << gridN << "," << countEdges(gridDi, true) << "," << r.time_ms << "," << r.inserts << "," << r.extracts << "," << r.decreaseKeys << endl;
        }

        //prim tests
        for (const string& heap : heaps) {
            BenchResult r;

            r = runPrim(sparseUn, 0, heap);
            cout << "prim," << heap << ",sparse," << n << "," << countEdges(sparseUn, false) << "," << r.time_ms << "," << r.inserts << "," << r.extracts << "," << r.decreaseKeys << endl;

            r = runPrim(denseUn, 0, heap);
            cout << "prim," << heap << ",dense," << n << "," << countEdges(denseUn, false) << "," << r.time_ms << "," << r.inserts << "," << r.extracts << "," << r.decreaseKeys << endl;

            int gridN = gridSide * gridSide;
            r = runPrim(gridUn, 0, heap);
            cout << "prim," << heap << ",grid," << gridN << "," << countEdges(gridUn, false) << "," << r.time_ms << "," << r.inserts << "," << r.extracts << "," << r.decreaseKeys << endl;
        }
    }

    return 0;
}