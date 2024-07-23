// include/core/property.hpp

#pragma once

#include <string>
#include <variant>
#include <stdexcept>

class Property {
public:
    enum class Type {
        Null,
        Boolean,
        Integer,
        Double,
        String
    };

    Property();  // Creates a null property
    Property(bool value);
    Property(int value);
    Property(double value);
    Property(const std::string& value);

    Type getType() const;
    
    bool asBool() const;
    int asInt() const;
    double asDouble() const;
    std::string asString() const;

    std::string serialize() const;
    static Property deserialize(const std::string& data);

private:
    std::variant<std::monostate, bool, int, double, std::string> value;
};