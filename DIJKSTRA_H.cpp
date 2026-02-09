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
    std::vector<int> parent; // parent[v] is predecessor of v on shortest path, or -1
};

// Dijkstra: single-source shortest paths (non-negative weights)
inline DijkstraResult dijkstra(const Graph& g, int source, PriorityQueue<long long, int>& pq) {
    const int n = g.num_vertices();
    if (source < 0 || source >= n) throw std::out_of_range("dijkstra: source out of range");

    const long long INF = std::numeric_limits<long long>::max() / 4;

    DijkstraResult res;
    res.dist.assign(n, INF);
    res.parent.assign(n, -1);

    // Store handles so we can decrease-key in O(1) amortized for Fib/Pairing heaps.
    std::vector<Node<long long, int>*> handle(n, nullptr);

    // Insert all vertices with INF so every vertex has a handle.
    // Then decrease-key(source, 0).
    for (int v = 0; v < n; ++v) {
        handle[v] = pq.insert(INF, v);
    }

    res.dist[source] = 0;
    pq.decrease_key(handle[source], 0);

    while (!pq.is_empty()) {
        auto [du, u] = pq.extract_min();
        if (du >= INF) {
            // Remaining vertices are unreachable
            break;
        }

        // If du is larger than our recorded distance, it means something inconsistent happened.
        // With handle-based decrease-key + "insert all once", du should equal res.dist[u].
        // We'll still guard.
        if (du != res.dist[u]) {
            // This can happen if someone changes strategy to "insert on discovery".
            // For now, we just continue safely.
            // (No lazy-deletion duplicates should exist in this strategy.)
        }

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
