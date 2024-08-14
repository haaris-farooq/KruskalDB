// src/core/edge.cpp
#include "core/edge.hpp"
#include <stdexcept>
#include <algorithm>

Edge::Edge(int id, int sourceNodeId, int targetNodeId, const std::string& type)
    : id(id), sourceNodeId(sourceNodeId), targetNodeId(targetNodeId), type(type), dirty(false) {}

int Edge::getId() const { return id; }

void Edge::setId(int newId) {
    id = newId;
    setDirty(true);
}

int Edge::getSourceNodeId() const { return sourceNodeId; }

int Edge::getTargetNodeId() const { return targetNodeId; }

std::string Edge::getType() const { return type; }

bool Edge::hasProperty(const std::string& key) const {
    return properties.find(key) != properties.end();
}

void Edge::removeProperty(const std::string& key) {
    properties.erase(key);
    setDirty(true);
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

    std::vector<std::pair<std::string, std::string>> sortedProperties;
    for (const auto& [key, value] : properties) {
        std::string serializedValue = std::visit([](const auto& prop) { return prop.serialize(); }, value);
        sortedProperties.emplace_back(key, serializedValue);
    }
    std::sort(sortedProperties.begin(), sortedProperties.end());

    for (const auto& [key, value] : sortedProperties) {
        oss << key << ":" << value << "|";
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
        
        // Determine property type and deserialize
        char typeChar = value[0];
        switch (typeChar) {
            case '0':
                edge.setProperty(key, BoolProperty::deserialize(value).getValue());
                break;
            case '1':
                edge.setProperty(key, IntProperty::deserialize(value).getValue());
                break;
            case '2':
                edge.setProperty(key, DoubleProperty::deserialize(value).getValue());
                break;
            case '3':
                edge.setProperty(key, StringProperty::deserialize(value).getValue());
                break;
            default:
                throw std::runtime_error("Unknown property type during deserialization");
        }
    }
    
    return edge;
}

bool Edge::isDirty() const {
    return dirty;
}

void Edge::setDirty(bool dirty) {
    this->dirty = dirty;
}