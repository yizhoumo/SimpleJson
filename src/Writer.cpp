#include "simplejson/Writer.h"

#include <cassert>
#include <cstdio>

namespace SimpleJson {

std::string Writer::write(const Value& root) {
    _strBuf.clear();
    stringifyValue(root);
    return _strBuf;
}

void Writer::stringifyValue(const Value& root) {
    switch (root.type()) {
        case ValueType::Null:
            _strBuf += "null";
            break;
        case ValueType::Bool:
            _strBuf += root.asBool() ? "true" : "false";
            break;
        case ValueType::Integer:
            _strBuf += std::to_string(root.asInteger());
            break;
        case ValueType::Real:
            stringifyReal(root.asReal());
            break;
        case ValueType::String:
            stringifyString(root.asStringView());
            break;
        case ValueType::Array:
            stringifyArray(root);
            break;
        case ValueType::Object:
            stringifyObject(root);
            break;
    }
}

void Writer::stringifyReal(const Real number) {
    constexpr auto bufSize = 32;

    const auto currentLength = _strBuf.size();
    _strBuf.resize(currentLength + bufSize);

    const auto pBuf = _strBuf.data() + currentLength;
    const auto res = std::snprintf(pBuf, bufSize, "%.17g", number);
    if (res > 0) {
        _strBuf.resize(currentLength + res);
    } else {
        _strBuf.resize(currentLength);
        _strBuf += std::to_string(number);
    }
}

void Writer::stringifyString(std::string_view str) {
    static constexpr auto HEX_DIGITS = "0123456789ABCDEF";

    _strBuf.reserve(_strBuf.size() + str.size() + 2);

    // begin of string
    _strBuf.push_back('"');

    // char
    for (const char c : str) {
        switch (c) {
            case '"':
            case '\\':
            case '/':
                _strBuf.push_back('\\');
                _strBuf.push_back(c);
                break;
            case '\b':
                _strBuf += "\\b";
                break;
            case '\f':
                _strBuf += "\\f";
                break;
            case '\n':
                _strBuf += "\\n";
                break;
            case '\r':
                _strBuf += "\\r";
                break;
            case '\t':
                _strBuf += "\\t";
                break;
            default:
                if (static_cast<unsigned char>(c) < '\x20') {
                    _strBuf += "\\u00";
                    _strBuf.push_back(HEX_DIGITS[c >> 4]);
                    _strBuf.push_back(HEX_DIGITS[c & 0xF]);
                } else {
                    _strBuf.push_back(c);
                }
                break;
        }
    }

    // end of string
    _strBuf.push_back('"');
}

void Writer::stringifyArray(const Value& root) {
    assert(root.isArray());

    // begin of array
    _strBuf.push_back('[');

    for (size_t i = 0; i < root.size(); ++i) {
        if (i != 0) {
            _strBuf.push_back(',');
        }
        stringifyValue(root[i]);
    }

    // begin of array
    _strBuf.push_back(']');
}

void Writer::stringifyObject(const Value& root) {
    assert(root.isObject());

    // begin of object
    _strBuf.push_back('{');

    const auto keys = root.getMemberNames();
    for (size_t i = 0; i < keys.size(); ++i) {
        if (i != 0) {
            _strBuf.push_back(',');
        }
        stringifyString(keys[i]);
        _strBuf.push_back(':');
        stringifyValue(root[keys[i]]);
    }

    // begin of object
    _strBuf.push_back('}');
}

}  // namespace SimpleJson
