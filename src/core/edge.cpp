#include "core/edge.hpp"

Edge::Edge(int id, int startNode, int endNode, const std::unordered_map<std::string, Property>& properties)
    : id(id), startNode(startNode), endNode(endNode), properties(properties) {}

int Edge::getId() const {
    return id;
}

int Edge::getStartNode() const {
    return startNode;
}

int Edge::getEndNode() const {
    return endNode;
}

Property Edge::getProperty(const std::string& key) const {
    auto it = properties.find(key);
    return it != properties.end() ? it->second : Property();
}

void Edge::setProperty(const std::string& key, const Property& value) {
    properties[key] = value;
}

