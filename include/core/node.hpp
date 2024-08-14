// include/core/node.hpp
#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <variant>
#include <algorithm>
#include <sstream>
#include "core/property.hpp"

class Node {
public:
    Node();
    explicit Node(int id);

    int getId() const;
    void setId(int newId);

    template<typename T>
    void setProperty(const std::string& key, const T& value) {
        properties[key] = Property<T>(value);
        setDirty(true);
    }

    template<typename T>
    T getProperty(const std::string& key) const {
        auto it = properties.find(key);
        if (it != properties.end()) {
            return std::get<Property<T>>(it->second).getValue();
        }
        throw std::out_of_range("Property not found");
    }

    bool hasProperty(const std::string& key) const;
    void removeProperty(const std::string& key);
    std::vector<std::string> getPropertyKeys() const;

    void addEdge(int edgeId, bool isOutgoing);
    void removeEdge(int edgeId, bool isOutgoing);
    const std::vector<int>& getIncomingEdges() const;
    const std::vector<int>& getOutgoingEdges() const;

    std::string serialize() const;
    static Node deserialize(const std::string& data);

    bool isDirty() const;
    void setDirty(bool dirty);

private:
    int id;
    bool dirty;
    std::unordered_map<std::string, std::variant<BoolProperty, IntProperty, DoubleProperty, StringProperty>> properties;
    std::vector<int> incomingEdges;
    std::vector<int> outgoingEdges;
};