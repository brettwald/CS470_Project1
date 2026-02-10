#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include "graph.h"
#include "priorityQueue.h"

#include <vector>
#include <limits>
#include <stdexcept>
#include <utility>

struct DijkstraResult {
    std::vector<long long> dist;
    std::vector<int> parent; //predecessor on shortest path, -1 if none
};

//single source shortest paths for non-negative weights
inline DijkstraResult dijkstra(const Graph& g, int source, PriorityQueue<long long, int>& pq) {
    const int n = g.num_vertices();
    if (source < 0 || source >= n) throw std::out_of_range("dijkstra: source out of range");

    const long long INF = std::numeric_limits<long long>::max() / 4;

    DijkstraResult res;
    res.dist.assign(n, INF);
    res.parent.assign(n, -1);

    //handles for decrease-key
    std::vector<Node<long long, int>*> handle(n, nullptr);

    //insert all vertices with INF then decrease source to 0
    for (int v = 0; v < n; ++v) {
        handle[v] = pq.insert(INF, v);
    }

    res.dist[source] = 0;
    pq.decrease_key(handle[source], 0);

    while (!pq.is_empty()) {
        auto [du, u] = pq.extract_min();
        if (du >= INF) {
            //remaining vertices unreachable
            break;
        }

        //relax edges
        for (const auto& e : g.neighbors(u)) {
            const int v = e.to;
            const long long nd = du + static_cast<long long>(e.weight);
            if (nd < res.dist[v]) {
                res.dist[v] = nd;
                res.parent[v] = u;
                pq.decrease_key(handle[v], nd);
            }
        }
    }

    return res;
}

#endif