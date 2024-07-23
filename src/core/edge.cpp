// edge.cpp

#include "include/core/edge.hpp"
#include <stdexcept>

Edge::Edge(int id, int sourceNodeId, int targetNodeId, const std::string& type)
    : id(id), sourceNodeId(sourceNodeId), targetNodeId(targetNodeId), type(type) {}

int Edge::getId() const { return id; }
void Edge::setId(int newId) { id = newId; }
int Edge::getSourceNodeId() const { return sourceNodeId; }
int Edge::getTargetNodeId() const { return targetNodeId; }
std::string Edge::getType() const { return type; }

void Edge::setProperty(const std::string& key, const Property& value) {
    properties[key] = value;
}

Property Edge::getProperty(const std::string& key) const {
    auto it = properties.find(key);
    if (it != properties.end()) {
        return it->second;
    }
    throw std::out_of_range("Property not found");
}

bool Edge::hasProperty(const std::string& key) const {
    return properties.find(key) != properties.end();
}

void Edge::removeProperty(const std::string& key) {
    properties.erase(key);
}

std::vector<std::string> Edge::getPropertyKeys() const {
    std::vector<std::string> keys;
    keys.reserve(properties.size());
    for (const auto& pair : properties) {
        keys.push_back(pair.first);
    }
    return keys;
}

std::string Edge::serialize() const {
    // Implement serialization logic
    // This is a placeholder implementation
    return "Edge serialization not implemented";
}

Edge Edge::deserialize(const std::string& data) {
    // Implement deserialization logic
    // This is a placeholder implementation
    return Edge(0, 0, 0, "");
}