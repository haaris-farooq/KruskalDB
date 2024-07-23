// src/core/node.cpp

#include "core/node.hpp"
#include <algorithm>
#include <stdexcept>

Node::Node(int id) : id(id), dirty(false) {}

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
    std::ostringstream oss;
    
    // Serialize ID
    oss << id << "|";
    
    // Serialize properties
    oss << properties.size() << "|";
    for (const auto& [key, value] : properties) {
        oss << key << ":" << value.serialize() << "|";
    }
    
    // Serialize incoming edges
    oss << incomingEdges.size() << "|";
    for (int edgeId : incomingEdges) {
        oss << edgeId << ",";
    }
    oss << "|";
    
    // Serialize outgoing edges
    oss << outgoingEdges.size() << "|";
    for (int edgeId : outgoingEdges) {
        oss << edgeId << ",";
    }
    
    return oss.str();
}

Node Node::deserialize(const std::string& data) {
    std::istringstream iss(data);
    std::string token;
    
    // Deserialize ID
    std::getline(iss, token, '|');
    Node node(std::stoi(token));
    
    // Deserialize properties
    std::getline(iss, token, '|');
    int propertyCount = std::stoi(token);
    for (int i = 0; i < propertyCount; ++i) {
        std::string keyValue;
        std::getline(iss, keyValue, '|');
        size_t colonPos = keyValue.find(':');
        std::string key = keyValue.substr(0, colonPos);
        std::string value = keyValue.substr(colonPos + 1);
        node.setProperty(key, Property::deserialize(value));
    }
    
    // Deserialize incoming edges
    std::getline(iss, token, '|');
    int incomingCount = std::stoi(token);
    std::getline(iss, token, '|');
    std::istringstream incomingStream(token);
    std::string edgeId;
    while (std::getline(incomingStream, edgeId, ',') && !edgeId.empty()) {
        node.addEdge(std::stoi(edgeId), false);
    }
    
    // Deserialize outgoing edges
    std::getline(iss, token, '|');
    int outgoingCount = std::stoi(token);
    std::getline(iss, token);
    std::istringstream outgoingStream(token);
    while (std::getline(outgoingStream, edgeId, ',') && !edgeId.empty()) {
        node.addEdge(std::stoi(edgeId), true);
    }
    
    return node;
}

bool Node::isDirty() const {
    return dirty;
}

void Node::setDirty(bool dirty) {
    this->dirty = dirty;
}