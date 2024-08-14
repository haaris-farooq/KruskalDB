// include/core/property.hpp
#pragma once
#include <string>
#include <sstream>
#include <stdexcept>
#include <type_traits>

template<typename T>
class Property {
public:
    Property() : value_() {} // Default constructor
    Property(const T& value) : value_(value) {}

    T getValue() const { return value_; }

    std::string serialize() const {
        std::ostringstream oss;
        oss << getTypeId() << ":";
        if constexpr (std::is_same_v<T, bool>) {
            oss << (value_ ? "true" : "false");
        } else {
            oss << value_;
        }
        return oss.str();
    }

    static Property<T> deserialize(const std::string& data) {
        std::istringstream iss(data);
        std::string typeStr, valueStr;
        std::getline(iss, typeStr, ':');
        std::getline(iss, valueStr);

        if (std::stoi(typeStr) != getTypeId()) {
            throw std::runtime_error("Type mismatch during deserialization");
        }

        if constexpr (std::is_same_v<T, bool>) {
            return Property<T>(valueStr == "true");
        } else if constexpr (std::is_same_v<T, std::string>) {
            return Property<T>(valueStr);
        } else {
            return Property<T>(fromString<T>(valueStr));
        }
    }

private:
    T value_;

    static constexpr int getTypeId() {
        if constexpr (std::is_same_v<T, bool>) return 0;
        else if constexpr (std::is_same_v<T, int>) return 1;
        else if constexpr (std::is_same_v<T, double>) return 2;
        else if constexpr (std::is_same_v<T, std::string>) return 3;
        else static_assert(always_false<T>::value, "Unsupported type");
    }

    template<typename U>
    static U fromString(const std::string& str) {
        std::istringstream iss(str);
        U result;
        iss >> result;
        return result;
    }

    // Helper to make static_assert dependent on T
    template<typename>
    struct always_false : std::false_type {};
};

// Specializations for supported types
using BoolProperty = Property<bool>;
using IntProperty = Property<int>;
using DoubleProperty = Property<double>;
using StringProperty = Property<std::string>;