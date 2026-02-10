#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <stdexcept>

class Graph {
public:
    struct Edge {
        int to;
        int weight; // non-negative for Dijkstra; Prim can handle non-negative or even negative, but your project likely uses non-negative
    };

    Graph(int n = 0, bool directed = false)
        : n_(n), directed_(directed), adj_(n) {
        if (n < 0) throw std::invalid_argument("Graph: n must be >= 0");
    }

    int num_vertices() const { return n_; }
    bool directed() const { return directed_; }

    // Add an edge u -> v (and v -> u if undirected graph)
    void add_edge(int u, int v, int w) {
        if (u < 0 || u >= n_ || v < 0 || v >= n_) {
            throw std::out_of_range("Graph::add_edge: vertex out of range");
        }
        // Dijkstra requires non-negative weights
        if (w < 0) {
            throw std::invalid_argument("Graph::add_edge: negative weights not allowed for Dijkstra");
        }

        adj_[u].push_back({v, w});
        if (!directed_) {
            adj_[v].push_back({u, w});
        }
    }

    const std::vector<Edge>& neighbors(int u) const {
        if (u < 0 || u >= n_) throw std::out_of_range("Graph::neighbors: vertex out of range");
        return adj_[u];
    }

private:
    int n_;
    bool directed_;
    std::vector<std::vector<Edge>> adj_;
};

#endif
