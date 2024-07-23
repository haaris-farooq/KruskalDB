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
    std::ostringstream oss;
    oss << static_cast<int>(getType()) << ":";
    
    switch (getType()) {
        case Type::Boolean:
            oss << (asBool() ? "true" : "false");
            break;
        case Type::Integer:
            oss << asInt();
            break;
        case Type::Double:
            oss << asDouble();
            break;
        case Type::String:
            oss << asString();
            break;
        default:
            throw std::runtime_error("Unknown property type");
    }
    
    return oss.str();
}

Property Property::deserialize(const std::string& data) {
    std::istringstream iss(data);
    std::string typeStr, valueStr;
    
    std::getline(iss, typeStr, ':');
    std::getline(iss, valueStr);
    
    int typeInt = std::stoi(typeStr);
    Type type = static_cast<Type>(typeInt);
    
    switch (type) {
        case Type::Boolean:
            return Property(valueStr == "true");
        case Type::Integer:
            return Property(std::stoi(valueStr));
        case Type::Double:
            return Property(std::stod(valueStr));
        case Type::String:
            return Property(valueStr);
        default:
            throw std::runtime_error("Unknown property type");
    }
}