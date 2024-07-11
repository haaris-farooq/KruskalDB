#ifndef NODE_HPP
#define NODE_HPP

#include <string>
#include <unordered_map>
#include "property.hpp"

class Node {
public:
    Node(int id, const std::unordered_map<std::string, Property>& properties);
    int getId() const;
    Property getProperty(const std::string& key) const;
    void setProperty(const std::string& key, const Property& value);

private:
    int id;
    std::unordered_map<std::string, Property> properties;
};

#endif // NODE_HPP
