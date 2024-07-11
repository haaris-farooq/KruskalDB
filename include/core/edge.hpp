#ifndef EDGE_HPP
#define EDGE_HPP

#include <string>
#include <unordered_map>
#include "property.hpp"

class Edge {
public:
    Edge(int id, int startNode, int endNode, const std::unordered_map<std::string, Property>& properties);
    int getId() const;
    int getStartNode() const;
    int getEndNode() const;
    Property getProperty(const std::string& key) const;
    void setProperty(const std::string& key, const Property& value);

private:
    int id;
    int startNode;
    int endNode;
    std::unordered_map<std::string, Property> properties;
};

#endif // EDGE_HPP