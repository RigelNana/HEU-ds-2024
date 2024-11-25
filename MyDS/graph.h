#ifndef GRAPH_H
#define GRAPH_H
#include "../MyDS/arraylist.h"
#include "../MyDS/circularqueue.h"
#include "../MyDS/pair.h"
#ifdef __cplusplus
# include <limits>
# include <ostream>
# include <utility>
#endif
template <typename T>
class Graph {
public:
    struct Edge {
        size_t from;
        size_t to;
        T weight;

        Edge() : from(0), to(0), weight(T()) {}

        Edge(size_t from, size_t to, T w = T())
            : from(from),
              to(to),
              weight(w) {}

        bool operator==(Edge const &other) const {
            return from == other.from && to == other.to &&
                   weight == other.weight;
        }

        Edge &operator=(Edge const &other) {
            from = other.from;
            to = other.to;
            weight = other.weight;
            return *this;
        }

        Edge &operator=(Edge &&other) noexcept {
            from = other.from;
            to = other.to;
            weight = other.weight;
            return *this;
        }

        friend std::ostream &operator<<(std::ostream &os, Edge const &edge) {
            os << edge.from << "->" << edge.to << '(' << edge.weight << ')';
            return os;
        }
    };

    explicit Graph(bool directed = false) : directed(directed), count(0) {}

    size_t add_vertex() {
        adj.push_back(ArrayList<Edge>());
        return count++;
    }

    void add_edge(size_t from, size_t to, T weight = T()) {
        check_vertex(from);
        check_vertex(to);
        adj[from].push_back(Edge(from, to, weight));
        if (!directed && from != to) {
            adj[to].push_back(Edge(to, from, weight));
        }
    }

    void remove_edge(size_t from, size_t to) {
        check_vertex(from);
        check_vertex(to);
        for (size_t i = 0; i < adj[from].size(); i++) {
            if (adj[from][i].to == to) {
                adj[from].erase(i);
                break;
            }
        }
        if (!directed && from != to) {
            for (size_t i = 0; i < adj[to].size(); i++) {
                if (adj[to][i].to == from) {
                    adj[to].erase(i);
                    break;
                }
            }
        }
    }

    ArrayList<Edge> edges() const {
        ArrayList<Edge> lists;
        for (size_t i = 0; i < count; i++) {
            for (auto const &edge: adj[i]) {
                lists.push_back(edge);
            }
        }
        return lists;
    }

    ArrayList<Edge> const &adjacent(size_t v) const {
        check_vertex(v);
        return adj[v];
    }

    ArrayList<size_t> dfs(size_t vertex, ArrayList<bool> &visited,
                          ArrayList<size_t> &result) const {
        visited[vertex] = true;
        result.push_back(vertex);
        for (auto const &edge: adj[vertex]) {
            if (!visited[edge.to]) {
                dfs(edge.to, visited, result);
            }
        }
        return result;
    }

    ArrayList<size_t> dfs(size_t vertex) const {
        ArrayList<bool> visited(count, false);
        ArrayList<size_t> result;
        return dfs(vertex, visited, result);
    }

    ArrayList<size_t> bfs(size_t vertex) const {
        check_vertex(vertex);
        ArrayList<bool> visited(count, false);
        ArrayList<size_t> result;
        CircularQueue<size_t> queue;
        queue.enqueue(vertex);
        visited[vertex] = true;
        result.push_back(vertex);
        while (!queue.empty()) {
            size_t v = queue.front();
            queue.dequeue();
            for (auto const &edge: adj[v]) {
                if (!visited[edge.to]) {
                    queue.enqueue(edge.to);
                    visited[edge.to] = true;
                    result.push_back(edge.to);
                }
            }
        }
        return result;
    }

    Pair<ArrayList<T>, ArrayList<size_t>> dijkstra(size_t vertex) const {
        check_vertex(vertex);
        ArrayList<T> dist(count, std::numeric_limits<T>::max());
        ArrayList<size_t> prev(count, count);
        ArrayList<bool> visited(count, false);
        dist[vertex] = T();
        for (size_t i = 0; i < count; i++) {
            T min_dist = std::numeric_limits<T>::max();
            size_t u = count;
            for (size_t j = 0; j < count; j++) {
                if (!visited[j] && dist[j] < min_dist) {
                    min_dist = dist[j];
                    u = j;
                }
            }
            if (u == count) {
                break;
            }
            visited[u] = true;
            for (auto const &edge: adj[u]) {
                if (dist[edge.to] > dist[u] + edge.weight) {
                    dist[edge.to] = dist[u] + edge.weight;
                    prev[edge.to] = u;
                }
            }
        }
        return Pair(dist, prev);
    }

    size_t size() const {
        return count;
    }

    bool is_directed() const {
        return directed;
    }

    size_t indegree(size_t vertex) const {
        check_vertex(vertex);
        size_t in = 0;
        if (directed) {
            for (size_t i = 0; i < count; i++) {
                for (auto const &edge: adj[i]) {
                    if (edge.to == vertex) {
                        in++;
                    }
                }
            }
        } else {
            in = adj[vertex].size();
            for (auto const &edge: adj[vertex]) {
                if (edge.to == vertex) {
                    in--;
                }
            }
        }
        return in;
    }

    size_t outdegree(size_t vertex) const {
        check_vertex(vertex);
        return adj[vertex].size();
    }

    size_t degree(size_t vertex) const {
        if (directed) {
            return indegree(vertex) + outdegree(vertex);
        } else {
            return adj[vertex].size();
        }
    }

    ArrayList<size_t> topological_sort() const {
        if (!directed) {
            throw std::logic_error(
                "topological sort is only for directed graph");
        }
        ArrayList<size_t> result;
        ArrayList<size_t> in(count, 0);
        for (size_t i = 0; i < count; i++) {
            for (auto const &edge: adj[i]) {
                in[edge.to]++;
            }
        }
        CircularQueue<size_t> queue;
        for (size_t i = 0; i < count; i++) {
            if (in[i] == 0) {
                queue.enqueue(i);
            }
        }
        while (!queue.empty()) {
            size_t v = queue.front();
            queue.dequeue();
            result.push_back(v);
            for (auto const &edge: adj[v]) {
                in[edge.to]--;
                if (in[edge.to] == 0) {
                    queue.enqueue(edge.to);
                }
            }
        }
        if (result.size() != count) {
            throw std::logic_error("graph has a cycle");
        }
        return result;
    }

private:
    bool directed;
    size_t count;
    ArrayList<ArrayList<Edge>> adj;

    void check_vertex(size_t v) const {
        if (v >= count) {
            throw std::out_of_range("vertex out of range");
        }
    }
};

#endif // !GRAPH_H
