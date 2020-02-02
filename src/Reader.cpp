#include "simplejson/Reader.h"

#include <cassert>
#include <cctype>

namespace SimpleJson {

// JSON = ws value ws
bool Reader::parse(std::string_view document, Value& root) {
    // set context
    _document = document;
    _it = _document.begin();

    // reset result
    _result = ParseResult::Ok;

    // parsing
    skipWhitespace();
    root = parseValue();
    if (good()) {
        skipWhitespace();
        if (_it < _document.end()) {
            _result = ParseResult::RootNotSingular;
        }
    }

    return good();
}

void Reader::skipWhitespace() {
    while (_it < _document.end() && std::isspace(*_it)) {
        ++_it;
    }
}

// value = null / false / true
Value Reader::parseValue() {
    if (_it >= _document.end()) {
        return error(ParseResult::ExpectValue);
    }

    switch (*_it) {
        case 'n':
            return parseLiteral("null", Null());
        case 't':
            return parseLiteral("true", Bool(true));
        case 'f':
            return parseLiteral("false", Bool(false));
        default:
            return error(ParseResult::InvalidValue);
    }
}

Value Reader::parseLiteral(std::string_view literal, Value value) {
    for (const char c : literal) {
        if (_it >= _document.end() || *_it != c) {
            return error(ParseResult::InvalidValue);
        }
        ++_it;
    }
    return value;
}

Value Reader::error(ParseResult errorType) {
    assert(errorType != ParseResult::Ok);
    _result = errorType;
    return {};
}

}  // namespace SimpleJson
