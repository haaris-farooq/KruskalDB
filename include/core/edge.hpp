// include/core/edge.hpp
#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <variant>
#include "core/property.hpp"

class Edge {
public:
    Edge(int id, int sourceNodeId, int targetNodeId, const std::string& type);
    
    int getId() const;
    void setId(int newId);
    int getSourceNodeId() const;
    int getTargetNodeId() const;
    std::string getType() const;

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

    std::string serialize() const;
    static Edge deserialize(const std::string& data);

    bool isDirty() const;
    void setDirty(bool dirty);

private:
    int id;
    int sourceNodeId;
    int targetNodeId;
    std::string type;
    bool dirty;
    std::unordered_map<std::string, std::variant<BoolProperty, IntProperty, DoubleProperty, StringProperty>> properties;
};