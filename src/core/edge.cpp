// edge.cpp

#include "include/core/edge.hpp"
#include <stdexcept>

Edge::Edge(int id, int sourceNodeId, int targetNodeId, const std::string& type)
    : id(id), sourceNodeId(sourceNodeId), targetNodeId(targetNodeId), type(type), dirty(false) {}

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
    std::ostringstream oss;
    
    // Serialize ID, source node ID, target node ID, and type
    oss << id << "|" << sourceNodeId << "|" << targetNodeId << "|" << type << "|";
    
    // Serialize properties
    oss << properties.size() << "|";
    for (const auto& [key, value] : properties) {
        oss << key << ":" << value.serialize() << "|";
    }
    
    return oss.str();
}

Edge Edge::deserialize(const std::string& data) {
    std::istringstream iss(data);
    std::string token;
    
    // Deserialize ID, source node ID, target node ID, and type
    std::getline(iss, token, '|');
    int id = std::stoi(token);
    
    std::getline(iss, token, '|');
    int sourceNodeId = std::stoi(token);
    
    std::getline(iss, token, '|');
    int targetNodeId = std::stoi(token);
    
    std::getline(iss, token, '|');
    std::string type = token;
    
    Edge edge(id, sourceNodeId, targetNodeId, type);
    
    // Deserialize properties
    std::getline(iss, token, '|');
    int propertyCount = std::stoi(token);
    for (int i = 0; i < propertyCount; ++i) {
        std::string keyValue;
        std::getline(iss, keyValue, '|');
        size_t colonPos = keyValue.find(':');
        std::string key = keyValue.substr(0, colonPos);
        std::string value = keyValue.substr(colonPos + 1);
        edge.setProperty(key, Property::deserialize(value));
    }
    
    return edge;
}

bool Edge::isDirty() const {
    return dirty;
}

void Edge::setDirty(bool dirty) {
    this->dirty = dirty;
}