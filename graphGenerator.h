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
inline Graph generateRandom(int n, bool directed, int targetEdges, int maxWeight = 1000) {
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

    //add target edges (lot for dense, few for sparse)
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

//grid
inline Graph generateGrid(int rows, int cols, bool directed, int maxWeight = 1000) {
    int n = rows * cols;
    Graph g(n, directed);
    mt19937 rng(42);
    uniform_int_distribution<int> weightDist(1, maxWeight);

    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            int u = r * cols + c;

            //connect to right neighbor
            if (c + 1 < cols) {
                g.add_edge(u, u + 1, weightDist(rng));
            }

            //connect to bottom neighbor
            if (r + 1 < rows) {
                g.add_edge(u, u + cols, weightDist(rng));
            }
        }
    }

    return g;
}

//worst case graph
//uses this weird concept i found online 
//skip edges give bad distances first
//chain gives better distances after, forcing decrease-key
inline Graph generateWorstCase(int n, bool directed, int maxWeight = 1000) {
    Graph g(n, directed);

    for (int i = 0; i < n - 1; i++) {
        g.add_edge(i, i + 1, 1);
    }

    for (int skip = 2; skip <= 10; skip++) {
        for (int i = 0; i < n - skip; i++) {
            g.add_edge(i, i + skip, skip + 1);
        }
    }

    return g;
}

#endif