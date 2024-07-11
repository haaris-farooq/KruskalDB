#include <node.hpp>
#include <property.hpp>


Node::Node(int id, const std::unordered_map<std::string, Property>& properties)
    : id(id), properties(properties) {}


int Node::getId() const {
    return id;
}

Property Node::getProperty(const std::string& key) const {
    auto it = properties.find(key);
    return it != properties.end() ? it->second : Property();
}

void Node::setProperty(const std::string& key, const Property& value) {
    properties[key] = value;
}
