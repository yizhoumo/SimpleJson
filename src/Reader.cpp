#include "simplejson/Reader.h"

#include <cassert>
#include <cctype>
#include <cmath>
#include <cstdlib>

/// return the pointer to the char past the number
static const char* validateNumber(const char* str);

namespace SimpleJson {

/// JSON = ws value ws
bool Reader::parse(const char* pDocument, Value& root) {
    if (!pDocument || *pDocument == 0) {
        root = error(ParseResult::ExpectValue);
        return false;
    }

    // set context
    _pCur = pDocument;
    _result = ParseResult::Ok;

    // parsing
    skipWhitespace();
    root = parseValue();
    if (good()) {
        skipWhitespace();
        if (*_pCur) {
            root = error(ParseResult::RootNotSingular);
        }
    }

    _pCur = nullptr;
    return good();
}

void Reader::skipWhitespace() {
    assert(_pCur != nullptr);
    while (std::isspace(*_pCur)) {
        ++_pCur;
    }
}

/// value = null / true / false / number
Value Reader::parseValue() {
    assert(_pCur != nullptr);
    if (*_pCur == 0) {
        return error(ParseResult::ExpectValue);
    }

    switch (*_pCur) {
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
    assert(_pCur != nullptr);
    assert(!literal.empty());
    assert(*_pCur == literal[0]);

    for (const char c : literal) {
        if (*_pCur != c) {
            return error(ParseResult::InvalidValue);
        }
        ++_pCur;
    }
    return value;
}

/// number = [ "-" ] int [ frac ] [ exp ]
Value Reader::parseNumber() {
    assert(_pCur != nullptr);
    assert(*_pCur);

    const char* endNumber = validateNumber(_pCur);
    if (endNumber == _pCur) {
        return error(ParseResult::InvalidValue);
    }

    // parse
    const double value = std::strtod(_pCur, nullptr);

    // overflow
    if (value == HUGE_VAL || value == -HUGE_VAL) {
        return error(ParseResult::NumberTooBig);
    }

    // success
    _pCur = endNumber;
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
