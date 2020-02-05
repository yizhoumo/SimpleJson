#include "simplejson/Reader.h"

#include <cassert>
#include <cctype>
#include <cerrno>
#include <cmath>
#include <cstdlib>

enum class NumberType { Nan, Integer, Real };

/// check what type of number is `str`, and set `end` to the char past it
static NumberType validateNumber(const char* str, const char*& end);

namespace SimpleJson {

/// JSON = ws value ws
bool Reader::parse(const char* const pDocument, Value& root) {
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

Value Reader::error(const ParseResult errorType) {
    assert(errorType != ParseResult::Ok);
    _result = errorType;
    return Value();
}

/// value = null / true / false / number / string
Value Reader::parseValue() {
    assert(_pCur != nullptr);

    switch (*_pCur) {
        case '\0':
            return error(ParseResult::ExpectValue);
        case 'n':
            return parseLiteral("null", Value());
        case 't':
            return parseLiteral("true", Value(true));
        case 'f':
            return parseLiteral("false", Value(false));
        case '"':
            return parseString();
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
    assert(*_pCur != 0);

    const char* numberEnd = nullptr;
    const auto numberType = validateNumber(_pCur, numberEnd);
    assert(numberEnd >= _pCur);

    switch (numberType) {
        case NumberType::Nan:
            return error(ParseResult::InvalidValue);
        case NumberType::Integer:
            return parseInteger(numberEnd);
        case NumberType::Real:
            return parseReal(numberEnd);
    }

    // not possible
    return error(ParseResult::InvalidValue);
}

Value Reader::parseInteger(const char* const numberEnd) {
    assert(_pCur != nullptr);
    assert(*_pCur != 0);
    assert(_pCur < numberEnd);

    // parse
    char* actualEnd;
    errno = 0;
    const Integer value = std::strtoll(_pCur, &actualEnd, 10);

    assert(actualEnd > _pCur);
    if (actualEnd <= _pCur) {
        // not possible
        return error(ParseResult::InvalidValue);
    }

    // overflow
    if (errno == ERANGE) {
        return error(ParseResult::NumberOverflow);
    }

    // success
    _pCur = numberEnd;
    return Value(value);
}

Value Reader::parseReal(const char* const numberEnd) {
    assert(_pCur != nullptr);
    assert(*_pCur != 0);
    assert(_pCur < numberEnd);

    // parse
    char* actualEnd;
    errno = 0;
    const Real value = std::strtod(_pCur, &actualEnd);

    assert(actualEnd > _pCur);
    if (actualEnd <= _pCur) {
        // not possible
        return error(ParseResult::InvalidValue);
    }

    // overflow
    if (errno == ERANGE && (value == HUGE_VAL || value == -HUGE_VAL)) {
        return error(ParseResult::NumberOverflow);
    }

    // success
    _pCur = numberEnd;
    return Value(value);
}

/// string = quotation-mark *char quotation-mark
Value Reader::parseString() {
    assert(_pCur != nullptr);
    assert(*_pCur == '"');
    // string = quotation-mark *char quotation-mark
    // char = unescaped /
    //   escape (
    //       %x22 /          ; "    quotation mark  U+0022
    //       %x5C /          ; \    reverse solidus U+005C
    //       %x2F /          ; /    solidus         U+002F
    //       %x62 /          ; b    backspace       U+0008
    //       %x66 /          ; f    form feed       U+000C
    //       %x6E /          ; n    line feed       U+000A
    //       %x72 /          ; r    carriage return U+000D
    //       %x74 /          ; t    tab             U+0009
    //       %x75 4HEXDIG )  ; uXXXX                U+XXXX
    // escape = %x5C         ; \    reverse solidus
    // quotation-mark = %x22 ; "
    // unescaped = %x20-21 / %x23-5B / %x5D-10FFFF

    // quotation-mark
    ++_pCur;

    std::string value;
    while (true) {
        char c = *_pCur;
        if (c == '\0') {
            // end of document
            return error(ParseResult::MissQuotationMark);
        }
        if ((unsigned char)c < '\x20') {
            // invalid char
            return error(ParseResult::InvalidStringChar);
        }
        // valid char
        ++_pCur;
        if (c == '"') {
            // end of string
            return Value(std::move(value));
        }
        if (c == '\\') {
            // escape
            switch (*_pCur) {
                case '"':
                case '\\':
                case '/':
                    c = *_pCur;
                    break;
                case 'b':
                    c = '\b';
                    break;
                case 'f':
                    c = '\f';
                    break;
                case 'n':
                    c = '\n';
                    break;
                case 'r':
                    c = '\r';
                    break;
                case 't':
                    c = '\t';
                    break;
                default:
                    return error(ParseResult::InvalidStringEscape);
            }
            // valid escape
            ++_pCur;
        }
        value.push_back(c);
    }
    // never goto here
}

}  // namespace SimpleJson

// ===== helpers =====

static NumberType validateNumber(const char* const str, const char*& end) {
    // number = [ "-" ] int [ frac ] [ exp ]
    assert(str != nullptr);
    assert(*str != 0);

    end = str;
    auto p = str;
    bool isReal = false;

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
        return NumberType::Nan;
    }

    // frac = "." 1*digit
    if (*p == '.') {
        ++p;
        isReal = true;
        if (std::isdigit(*p)) {
            while (std::isdigit(*p)) {
                ++p;
            }
        } else {
            return NumberType::Nan;
        }
    }

    // exp = ("e" / "E") ["-" / "+"] 1*digit
    if (*p == 'e' || *p == 'E') {
        ++p;
        isReal = true;
        if (*p == '-' || *p == '+') {
            ++p;
        }
        if (std::isdigit(*p)) {
            while (std::isdigit(*p)) {
                ++p;
            }
        } else {
            return NumberType::Nan;
        }
    }

    end = p;
    assert(end >= str);

    return isReal ? NumberType::Real : NumberType::Integer;
}
