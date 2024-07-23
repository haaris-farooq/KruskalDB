// src/core/node.cpp

#include "core/node.hpp"
#include <algorithm>
#include <stdexcept>

Node::Node(int id) : id(id) {}

int Node::getId() const {
    return id;
}

void Node::setId(int newId) {
    id = newId;
}

void Node::setProperty(const std::string& key, const Property& value) {
    properties[key] = value;
}

Property Node::getProperty(const std::string& key) const {
    auto it = properties.find(key);
    if (it != properties.end()) {
        return it->second;
    }
    throw std::out_of_range("Property not found");
}

bool Node::hasProperty(const std::string& key) const {
    return properties.find(key) != properties.end();
}

void Node::removeProperty(const std::string& key) {
    properties.erase(key);
}

std::vector<std::string> Node::getPropertyKeys() const {
    std::vector<std::string> keys;
    keys.reserve(properties.size());
    for (const auto& pair : properties) {
        keys.push_back(pair.first);
    }
    return keys;
}

void Node::addEdge(int edgeId, bool isOutgoing) {
    auto& edges = isOutgoing ? outgoingEdges : incomingEdges;
    if (std::find(edges.begin(), edges.end(), edgeId) == edges.end()) {
        edges.push_back(edgeId);
    }
}

void Node::removeEdge(int edgeId, bool isOutgoing) {
    auto& edges = isOutgoing ? outgoingEdges : incomingEdges;
    edges.erase(std::remove(edges.begin(), edges.end(), edgeId), edges.end());
}

const std::vector<int>& Node::getIncomingEdges() const {
    return incomingEdges;
}

const std::vector<int>& Node::getOutgoingEdges() const {
    return outgoingEdges;
}

std::string Node::serialize() const {
    // Implement serialization logic
    // This is a placeholder implementation
    return "Node serialization not implemented";
}

Node Node::deserialize(const std::string& data) {
    // Implement deserialization logic
    // This is a placeholder implementation
    return Node(0);
}