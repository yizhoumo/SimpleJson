#include "TestHelper.h"

#include "gtest/gtest.h"

std::ostream& operator<<(std::ostream& out, SimpleJson::ValueType val) {
    switch (val) {
        case SimpleJson::ValueType::Null:
            return out << "[Null]";
        case SimpleJson::ValueType::Bool:
            return out << "[Bool]";
        case SimpleJson::ValueType::Number:
            return out << "[Number]";
        case SimpleJson::ValueType::String:
            return out << "[String]";
        case SimpleJson::ValueType::Array:
            return out << "[Array]";
        case SimpleJson::ValueType::Object:
            return out << "[Object]";
        default:
            return out << testing::PrintToString(val);
    }
}

std::ostream& operator<<(std::ostream& out, SimpleJson::ParseResult val) {
    switch (val) {
        case SimpleJson::ParseResult::Ok:
            return out << "[Ok]";
        case SimpleJson::ParseResult::ExpectValue:
            return out << "[ExpectValue]";
        case SimpleJson::ParseResult::InvalidValue:
            return out << "[InvalidValue]";
        case SimpleJson::ParseResult::RootNotSingular:
            return out << "[RootNotSingular]";
        case SimpleJson::ParseResult::NumberTooBig:
            return out << "[NumberTooBig]";
        default:
            return out << testing::PrintToString(val);
    }
}
