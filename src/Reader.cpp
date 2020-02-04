#include "simplejson/Reader.h"

#include <cassert>
#include <cctype>
#include <cmath>
#include <cstdlib>

/// return the pointer to the char past the number
static const char* validateNumber(const char* str);

namespace SimpleJson {

/// JSON = ws value ws
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
            root = Null();
        }
    }

    return good();
}

void Reader::skipWhitespace() {
    assert(_it <= _document.end());
    while (_it < _document.end() && std::isspace(*_it)) {
        ++_it;
    }
}

/// value = null / true / false / number
Value Reader::parseValue() {
    assert(_it <= _document.end());
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
            return parseNumber();
    }
}

Value Reader::parseLiteral(std::string_view literal, Value value) {
    assert(_it < _document.end());
    assert(!literal.empty());
    assert(*_it == literal[0]);

    for (const char c : literal) {
        if (_it >= _document.end() || *_it != c) {
            return error(ParseResult::InvalidValue);
        }
        ++_it;
    }
    return value;
}

/// number = [ "-" ] int [ frac ] [ exp ]
Value Reader::parseNumber() {
    assert(_it < _document.end());

    const char* endNumber = validateNumber(_it);
    if (endNumber == _it) {
        return error(ParseResult::InvalidValue);
    }

    // TODO: _it might be not null-terminated!
    const double value = std::strtod(_it, nullptr);

    // overflow
    if (value == HUGE_VAL || value == -HUGE_VAL) {
        return error(ParseResult::NumberTooBig);
    }

    // success
    _it = endNumber;
    return value;
}

Value Reader::error(ParseResult errorType) {
    assert(errorType != ParseResult::Ok);
    _result = errorType;
    return {};
}

}  // namespace SimpleJson

// ===== helpers =====

static const char* validateNumber(const char* const str) {
    // number = [ "-" ] int [ frac ] [ exp ]
    assert(str != nullptr);
    assert(*str != 0);

    auto p = str;

    // [ "-" ]
    if (*p == '-') {
        ++p;
    }

    // int = "0" / digit1-9 *digit
    if (*p == '0') {
        ++p;
    } else if (std::isdigit(*p)) {
        while (std::isdigit(*p)) {
            ++p;
        }
    } else {
        return str;
    }

    // frac = "." 1*digit
    if (*p == '.') {
        ++p;
        if (std::isdigit(*p)) {
            while (std::isdigit(*p)) {
                ++p;
            }
        } else {
            return str;
        }
    }

    // exp = ("e" / "E") ["-" / "+"] 1*digit
    if (*p == 'e' || *p == 'E') {
        ++p;
        if (*p == '-' || *p == '+') {
            ++p;
        }
        if (std::isdigit(*p)) {
            while (std::isdigit(*p)) {
                ++p;
            }
        } else {
            return str;
        }
    }

    return p;
}
