// src/core/node.cpp
#include "core/node.hpp"

Node::Node() : id(0), dirty(false) {}

Node::Node(int id) : id(id), dirty(false) {}

int Node::getId() const {
    return id;
}

void Node::setId(int newId) {
    id = newId;
    setDirty(true);
}

bool Node::hasProperty(const std::string& key) const {
    return properties.find(key) != properties.end();
}

void Node::removeProperty(const std::string& key) {
    properties.erase(key);
    setDirty(true);
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
        setDirty(true);
    }
}

void Node::removeEdge(int edgeId, bool isOutgoing) {
    auto& edges = isOutgoing ? outgoingEdges : incomingEdges;
    edges.erase(std::remove(edges.begin(), edges.end(), edgeId), edges.end());
    setDirty(true);
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

    std::vector<std::pair<std::string, std::string>> sortedProperties;
    for (const auto& [key, value] : properties) {
        std::string serializedValue = std::visit([](const auto& prop) { return prop.serialize(); }, value);
        sortedProperties.emplace_back(key, serializedValue);
    }
    std::sort(sortedProperties.begin(), sortedProperties.end());

    for (const auto& [key, value] : sortedProperties) {
        oss << key << ":" << value << "|";
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
    oss << "|";
    
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
        
        // Determine property type and deserialize
        char typeChar = value[0];
        switch (typeChar) {
            case '0':
                node.setProperty(key, BoolProperty::deserialize(value).getValue());
                break;
            case '1':
                node.setProperty(key, IntProperty::deserialize(value).getValue());
                break;
            case '2':
                node.setProperty(key, DoubleProperty::deserialize(value).getValue());
                break;
            case '3':
                node.setProperty(key, StringProperty::deserialize(value).getValue());
                break;
            default:
                throw std::runtime_error("Unknown property type during deserialization");
        }
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
    std::getline(iss, token, '|');
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