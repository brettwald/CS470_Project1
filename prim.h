#ifndef PRIM_H
#define PRIM_H

#include "graph.h"
#include "priorityQueue.h"

#include <vector>
#include <limits>
#include <stdexcept>
#include <utility>

struct PrimResult {
    long long total_weight = 0;
    std::vector<int> parent;      //parent[v] in MST, -1 for root or disconnected
    std::vector<long long> key;   //weight of edge connecting v to MST
    bool connected = true;        //false if graph is disconnected
};

//minimum spanning tree using prim's algorithm
//returns spanning forest if graph is disconnected
inline PrimResult prim_mst(const Graph& g, int start, PriorityQueue<long long, int>& pq) {
    const int n = g.num_vertices();
    if (start < 0 || start >= n) throw std::out_of_range("prim_mst: start out of range");
    if (g.directed()) throw std::invalid_argument("prim_mst: Prim requires an undirected graph");

    const long long INF = std::numeric_limits<long long>::max() / 4;

    PrimResult res;
    res.parent.assign(n, -1);
    res.key.assign(n, INF);

    std::vector<bool> inMST(n, false);
    std::vector<Node<long long, int>*> handle(n, nullptr);

    //insert all vertices with INF then decrease start to 0
    for (int v = 0; v < n; ++v) {
        handle[v] = pq.insert(INF, v);
    }
    res.key[start] = 0;
    pq.decrease_key(handle[start], 0);

    long long total = 0;
    int picked = 0;

    while (!pq.is_empty()) {
        auto [ku, u] = pq.extract_min();

        if (ku >= INF) {
            //remaining vertices disconnected
            break;
        }

        if (inMST[u]) {
            continue;
        }

        inMST[u] = true;
        picked++;
        total += ku;

        //check neighbors and update keys
        for (const auto& e : g.neighbors(u)) {
            const int v = e.to;
            const long long w = static_cast<long long>(e.weight);
            if (!inMST[v] && w < res.key[v]) {
                res.key[v] = w;
                res.parent[v] = u;
                pq.decrease_key(handle[v], w);
            }
        }
    }

    res.total_weight = total;
    res.connected = (picked == n);
    return res;
}

#endif
