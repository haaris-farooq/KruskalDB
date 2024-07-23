// include/core/edge.hpp

#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include "property.hpp"

class Edge {
public:
    Edge(int id, int sourceNodeId, int targetNodeId, const std::string& type);
    
    int getId() const;
    void setId(int newId);
    int getSourceNodeId() const;
    int getTargetNodeId() const;
    std::string getType() const;
    
    void setProperty(const std::string& key, const Property& value);
    Property getProperty(const std::string& key) const;
    bool hasProperty(const std::string& key) const;
    void removeProperty(const std::string& key);
    std::vector<std::string> getPropertyKeys() const;
    
    std::string serialize() const;
    static Edge deserialize(const std::string& data);

private:
    int id;
    int sourceNodeId;
    int targetNodeId;
    std::string type;
    std::unordered_map<std::string, Property> properties;
};