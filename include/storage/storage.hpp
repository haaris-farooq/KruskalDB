#ifndef STORAGE_HPP
#define STORAGE_HPP

#include <node.hpp>
#include <edge.hpp>
#include <property.hpp>

class Storage {
public:
    void addNode(const Node& node);
    Node getNode(int id) const;
    void updateNode(int id, const Node& node);
    void deleteNode(int id);

    void addEdge(const Edge& edge);
    Edge getEdge(int id) const;
    void updateEdge(int id, const Edge& edge);
    void deleteEdge(int id);

private:
    std::unordered_map<int, Node> nodes;
    std::unordered_map<int, Edge> edges;
};


#endif // STORAGE_HPP
