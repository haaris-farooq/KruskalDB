#ifndef PROPERTY_HPP
#define PROPERTY_HPP

#include <string>

enum class PropertyType { STRING, INTEGER, FLOAT, BOOLEAN };

class Property {
public:
    PropertyType type;
    std::string stringValue;
    int intValue;
    float floatValue;
    bool boolValue;

    Property() = default;
    Property(const std::string& value) : type(PropertyType::STRING), stringValue(value) {}
    Property(int value) : type(PropertyType::INTEGER), intValue(value) {}
    Property(float value) : type(PropertyType::FLOAT), floatValue(value) {}
    Property(bool value) : type(PropertyType::BOOLEAN), boolValue(value) {}

    std::string toString() const {
        switch (type) {
            case PropertyType::STRING: return stringValue;
            case PropertyType::INTEGER: return std::to_string(intValue);
            case PropertyType::FLOAT: return std::to_string(floatValue);
            case PropertyType::BOOLEAN: return boolValue ? "true" : "false";
        }
        return "";
    }
};

#endif // PROPERTY_HPP
