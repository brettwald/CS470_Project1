#include <iostream>
#include <vector>

#include "graph.h"
#include "dijkstra.h"
#include "prim.h"

#include "fibonacciHeap.h"
#include "pairingHeap.h"

// Helper: print Dijkstra distances
static void print_dists(const std::vector<long long>& dist) {
    for (size_t i = 0; i < dist.size(); ++i) {
        std::cout << "dist[" << i << "] = ";
        if (dist[i] > (1LL<<60)) std::cout << "INF";
        else std::cout << dist[i];
        std::cout << "\n";
    }
}

int main() {
    // -------------------------
    // Test 1: Dijkstra on a small directed graph
    // -------------------------
    {
        std::cout << "=== Test 1: Dijkstra (directed) ===\n";

        Graph g(5, true); // 5 vertices, directed

        // A known example
        // 0->1 (10), 0->2 (3)
        // 2->1 (1), 2->3 (2)
        // 1->3 (4)
        // 3->4 (2)
        // 2->4 (8)
        g.add_edge(0, 1, 10);
        g.add_edge(0, 2, 3);
        g.add_edge(2, 1, 1);
        g.add_edge(2, 3, 2);
        g.add_edge(1, 3, 4);
        g.add_edge(3, 4, 2);
        g.add_edge(2, 4, 8);

        // Expected shortest distances from 0:
        // dist[0]=0
        // dist[2]=3
        // dist[1]=4 (0->2->1)
        // dist[3]=5 (0->2->3)
        // dist[4]=7 (0->2->3->4)

        {
            std::cout << "\n-- Using FibonacciHeap --\n";
            FibonacciHeap<long long, int> pq;
            auto res = dijkstra(g, 0, pq);
            print_dists(res.dist);
        }

        {
            std::cout << "\n-- Using PairingHeap --\n";
            PairingHeap<long long, int> pq;
            auto res = dijkstra(g, 0, pq);
            print_dists(res.dist);
        }
    }

    // -------------------------
    // Test 2: Prim MST on a small undirected graph
    // -------------------------
    {
        std::cout << "\n=== Test 2: Prim MST (undirected) ===\n";

        Graph g(6, false); // 6 vertices, undirected

        // Classic MST example (weights chosen for easy checking)
        g.add_edge(0, 1, 4);
        g.add_edge(0, 2, 3);
        g.add_edge(1, 2, 1);
        g.add_edge(1, 3, 2);
        g.add_edge(2, 3, 4);
        g.add_edge(3, 4, 2);
        g.add_edge(4, 5, 6);

        // One MST total weight should be:
        // edges: (1-2)=1, (1-3)=2, (3-4)=2, (0-2)=3, (4-5)=6 -> total = 14

        {
            std::cout << "\n-- Using FibonacciHeap --\n";
            FibonacciHeap<long long, int> pq;
            auto res = prim_mst(g, 0, pq);
            std::cout << "MST total weight = " << res.total_weight << "\n";
            std::cout << "connected = " << (res.connected ? "true" : "false") << "\n";
        }

        {
            std::cout << "\n-- Using PairingHeap --\n";
            PairingHeap<long long, int> pq;
            auto res = prim_mst(g, 0, pq);
            std::cout << "MST total weight = " << res.total_weight << "\n";
            std::cout << "connected = " << (res.connected ? "true" : "false") << "\n";
        }
    }

    std::cout << "\nAll tests finished.\n";
    return 0;
}
