#include "TestHelper.h"

std::ostream& operator<<(std::ostream& out, SimpleJson::ValueType val) {
    switch (val) {
        case SimpleJson::ValueType::Null:
            return out << "[Null]";
        case SimpleJson::ValueType::Bool:
            return out << "[Bool]";
        case SimpleJson::ValueType::Integer:
            return out << "[Integer]";
        case SimpleJson::ValueType::Real:
            return out << "[Real]";
        case SimpleJson::ValueType::String:
            return out << "[String]";
        case SimpleJson::ValueType::Array:
            return out << "[Array]";
        case SimpleJson::ValueType::Object:
            return out << "[Object]";
    }

    // not possible
    return out << "[N/A]";
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
        case SimpleJson::ParseResult::NumberOverflow:
            return out << "[NumberOverflow]";
        case SimpleJson::ParseResult::MissQuotationMark:
            return out << "[MissQuotationMark]";
        case SimpleJson::ParseResult::InvalidStringEscape:
            return out << "[InvalidStringEscape]";
        case SimpleJson::ParseResult::InvalidStringChar:
            return out << "[InvalidStringChar]";
        case SimpleJson::ParseResult::InvalidUnicodeHex:
            return out << "[InvalidUnicodeHex]";
        case SimpleJson::ParseResult::InvalidUnicodeSurrogate:
            return out << "[InvalidUnicodeSurrogate]";
        case SimpleJson::ParseResult::MissComma:
            return out << "[MissComma]";
        case SimpleJson::ParseResult::MissSquareBracket:
            return out << "[MissSquareBracket]";
        case SimpleJson::ParseResult::MissKey:
            return out << "[MissKey]";
        case SimpleJson::ParseResult::MissColon:
            return out << "[MissColon]";
        case SimpleJson::ParseResult::MissCurlyBracket:
            return out << "[MissCurlyBracket]";
    }

    // not possible
    return out << "[N/A]";
}
