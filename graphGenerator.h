#ifndef GRAPHGEN_H
#define GRAPHGEN_H

#include "graph.h"
#include <random>
#include <algorithm>
#include <vector>
#include <set>
#include <cmath>

using namespace std;

//random sparse graph
inline Graph generateSparse(int n, bool directed, int multiplier = 3, int maxWeight = 1000) {
    Graph g(n, directed);
    mt19937 rng(67); //67
    uniform_int_distribution<int> weightDist(1, maxWeight); //random edge weight range

    //track existing edges to avoid duplicates
    set<pair<int,int>> edges;

    //random order for verticies
    vector<int> order(n);
    for (int i = 0; i < n; i++) order[i] = i;
    shuffle(order.begin(), order.end(), rng);

    //build spanning tree
    for (int i = 1; i < n; i++) {
        uniform_int_distribution<int> parentDist(0, i - 1);
        int u = order[i];
        int v = order[parentDist(rng)];
        int w = weightDist(rng);

        g.add_edge(u, v, w);
        edges.insert({min(u,v), max(u,v)});
    }

    //add a ton of edges
    int targetEdges = multiplier * n;
    int currentEdges = n - 1;

    while (currentEdges < targetEdges) {
        int u = rng() % n;
        int v = rng() % n;
        if (u == v) continue;

        auto key = make_pair(min(u,v), max(u,v));
        if (edges.count(key)) continue;

        g.add_edge(u, v, weightDist(rng));
        edges.insert(key);
        currentEdges++;
    }

    return g;
}

//random dense graph
inline Graph generateDense(int n, bool directed, int divisor = 4, int maxWeight = 1000) {
    // TODO
}

//grid graph
inline Graph generateGrid(int rows, int cols, bool directed, int maxWeight = 1000) {
    // TODO
}

//worst case graph
inline Graph generateWorstCase(int n, bool directed, int maxWeight = 1000) {
    // TODO
}

#endif