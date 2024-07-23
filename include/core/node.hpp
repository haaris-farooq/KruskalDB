// include/core/node.hpp

#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include "core/property.hpp"

class Node {
public:
    explicit Node(int id);
    
    int getId() const;
    void setId(int newId);
    
    void setProperty(const std::string& key, const Property& value);
    Property getProperty(const std::string& key) const;
    bool hasProperty(const std::string& key) const;
    void removeProperty(const std::string& key);
    std::vector<std::string> getPropertyKeys() const;
    
    void addEdge(int edgeId, bool isOutgoing);
    void removeEdge(int edgeId, bool isOutgoing);
    const std::vector<int>& getIncomingEdges() const;
    const std::vector<int>& getOutgoingEdges() const;
    
    std::string serialize() const;
    static Node deserialize(const std::string& data);

private:
    int id;
    std::unordered_map<std::string, Property> properties;
    std::vector<int> incomingEdges;
    std::vector<int> outgoingEdges;
};