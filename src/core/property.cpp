// src/core/property.cpp

#include "include/core/property.hpp"

Property::Property() : value(std::monostate{}) {}
Property::Property(bool value) : value(value) {}
Property::Property(int value) : value(value) {}
Property::Property(double value) : value(value) {}
Property::Property(const std::string& value) : value(value) {}

Property::Type Property::getType() const {
    return static_cast<Type>(value.index());
}

bool Property::asBool() const {
    if (getType() != Type::Boolean) throw std::bad_variant_access();
    return std::get<bool>(value);
}

int Property::asInt() const {
    if (getType() != Type::Integer) throw std::bad_variant_access();
    return std::get<int>(value);
}

double Property::asDouble() const {
    if (getType() != Type::Double) throw std::bad_variant_access();
    return std::get<double>(value);
}

std::string Property::asString() const {
    if (getType() != Type::String) throw std::bad_variant_access();
    return std::get<std::string>(value);
}

std::string Property::serialize() const {
    // Implement serialization logic
    // This is a placeholder implementation
    return "Property serialization not implemented";
}

Property Property::deserialize(const std::string& data) {
    // Implement deserialization logic
    // This is a placeholder implementation
    return Property();
}