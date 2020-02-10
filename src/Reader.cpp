#include "simplejson/Reader.h"

#include <cassert>
#include <cerrno>
#include <cmath>
#include <cstdlib>

enum class NumberType { Nan, Integer, Real };

// helpers
namespace {

/// check what type of number is `str`, and set `end` to the char past it
NumberType validateNumber(const char* str, const char*& end);

/// parse str as length-digit hex, return -1 if str is invalid
int parseHex(const char* str, size_t length);

}  // namespace

namespace SimpleJson {

/// JSON = ws value ws
bool Reader::parse(const char* const pDocument, Value& root) {
    if (pDocument == nullptr || *pDocument == 0) {
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
        if (*_pCur != 0) {
            root = error(ParseResult::RootNotSingular);
        }
    }

    _pCur = nullptr;
    return good();
}

/// ws = *(%x20 / %x09 / %x0A / %x0D)
void Reader::skipWhitespace() {
    assert(_pCur != nullptr);
    auto p = _pCur;
    while (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r') {
        ++p;
    }
    _pCur = p;
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
            return parseLiteral("true", true);
        case 'f':
            return parseLiteral("false", false);
        case '"':
            return parseString();
        case '[':
            return parseArray();
        default:
            return parseNumber();
    }
}

Value Reader::parseLiteral(std::string_view literal, Value value) {
    assert(_pCur != nullptr);
    assert(!literal.empty());
    assert(*_pCur == literal[0]);

    auto p = _pCur;
    for (const char c : literal) {
        if (*p != c) {
            return error(ParseResult::InvalidValue);
        }
        ++p;
    }
    _pCur = p;
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
    // char = unescaped / escape (...)
    // escape = %x5C         ; \    reverse solidus
    // quotation-mark = %x22 ; "
    // unescaped = %x20-21 / %x23-5B / %x5D-10FFFF

    assert(_strBuf.empty());
    _strBuf.clear();

    // quotation-mark
    ++_pCur;

    while (true) {
        const char c = *_pCur;
        if (c == '\0') {
            // end of document
            return error(ParseResult::MissQuotationMark);
        }
        if (static_cast<unsigned char>(c) < '\x20') {
            // invalid char
            return error(ParseResult::InvalidStringChar);
        }

        // c is valid char
        if (c == '"') {
            // end of string
            ++_pCur;
            auto value = Value(_strBuf);
            _strBuf.clear();
            return value;
        }
        if (c == '\\') {
            // escaped
            const auto res = parseEscaped();
            if (res != ParseResult::Ok) {
                return error(res);
            }
        } else {
            // unescaped
            _strBuf.push_back(c);
            ++_pCur;
        }
    }
    // never goto here
}

ParseResult Reader::parseEscaped() {
    assert(_pCur != nullptr);
    assert(*_pCur == '\\');

    // escaped =
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

    char c = 0;
    switch (_pCur[1]) {
        case '"':
        case '\\':
        case '/':
            c = _pCur[1];
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
        case 'u':
            return parseUnicode();
        default:
            return ParseResult::InvalidStringEscape;
    }

    // valid escape (except unicode)
    _strBuf.push_back(c);
    _pCur += 2;
    return ParseResult::Ok;
}

ParseResult Reader::parseUnicode() {
    assert(_pCur != nullptr);
    assert(_pCur[0] == '\\');
    assert(_pCur[1] == 'u');

    constexpr auto HEX_DIGIT_LEN = 4;
    constexpr auto SURROGATE_BIT_LEN = 10;
    constexpr auto HIGH_SURROGATE_MIN = 0xD800;
    constexpr auto HIGH_SURROGATE_MAX = 0xDBFF;
    constexpr auto LOW_SURROGATE_MIN = 0xDC00;
    constexpr auto LOW_SURROGATE_MAX = 0xDFFF;
    constexpr auto SURROGATE_PAIR_MIN = 0x1'0000;
    constexpr auto SURROGATE_PAIR_MAX = 0x10'FFFF;

    auto p = _pCur;
    p += 2;

    // parse code point
    int codePoint = parseHex(p, HEX_DIGIT_LEN);
    if (codePoint == -1) {
        return ParseResult::InvalidUnicodeHex;
    }
    assert(codePoint >= 0 && codePoint <= 0xFFFF);
    p += HEX_DIGIT_LEN;

    // surrogate pair
    if (codePoint >= HIGH_SURROGATE_MIN && codePoint <= HIGH_SURROGATE_MAX) {
        const int high = codePoint;
        if (p[0] != '\\' || p[1] != 'u') {
            return ParseResult::InvalidUnicodeSurrogate;
        }
        p += 2;

        const int low = parseHex(p, HEX_DIGIT_LEN);
        if (low == -1) {
            return ParseResult::InvalidUnicodeHex;
        }
        p += HEX_DIGIT_LEN;

        if (low >= LOW_SURROGATE_MIN && low <= LOW_SURROGATE_MAX) {
            codePoint = SURROGATE_PAIR_MIN +
                        ((high - HIGH_SURROGATE_MIN) << SURROGATE_BIT_LEN) +
                        (low - LOW_SURROGATE_MIN);
        } else {
            return ParseResult::InvalidUnicodeSurrogate;
        }
        assert(codePoint >= SURROGATE_PAIR_MIN &&
               codePoint <= SURROGATE_PAIR_MAX);
    }

    // valid code point
    encodeUnicode(codePoint);
    _pCur = p;
    return ParseResult::Ok;
}

void Reader::encodeUnicode(const unsigned codePoint) {
    assert(codePoint <= 0x10FFFF);

    if (codePoint <= 0x007F) {
        // 7-bit code point, 1-byte code unit
        // 0xx'xxxx
        _strBuf.push_back(0b11'1111 & codePoint);
    } else if (codePoint <= 0x07FF) {
        // 11-bit code point, 2-byte code unit
        // 110x'xxxx 10xx'xxxx
        _strBuf.push_back(0b1100'0000 | (0b01'1111 & (codePoint >> 6)));
        _strBuf.push_back(0b1000'0000 | (0b11'1111 & codePoint));
    } else if (codePoint <= 0xFFFF) {
        // 16-bit code point, 3-byte code unit
        // 1110'xxxx 10xx'xxxx 10xx'xxxx
        _strBuf.push_back(0b1110'0000 | (0b1111 & (codePoint >> 12)));
        _strBuf.push_back(0b1000'0000 | (0b11'1111 & (codePoint >> 6)));
        _strBuf.push_back(0b1000'0000 | (0b11'1111 & codePoint));
    } else if (codePoint <= 0x10FFFF) {
        // 21-bit code point, 4-byte code unit
        // 1111'0xxx 10xx'xxxx 10xx'xxxx 10xx'xxxx
        _strBuf.push_back(0b1111'0000 | (0b0111 & (codePoint >> 18)));
        _strBuf.push_back(0b1000'0000 | (0b11'1111 & (codePoint >> 12)));
        _strBuf.push_back(0b1000'0000 | (0b11'1111 & (codePoint >> 6)));
        _strBuf.push_back(0b1000'0000 | (0b11'1111 & codePoint));
    }
}

/// array = %x5B ws [ value *( ws %x2C ws value ) ] ws %x5D
Value Reader::parseArray() {
    assert(_pCur != nullptr);
    assert(*_pCur == '[');

    // '['
    ++_pCur;

    auto value = Value(ValueType::Array);
    while (true) {
        skipWhitespace();
        const char c = *_pCur;
        if (c == '\0') {
            // end of document
            return error(ParseResult::MissSquareBracket);
        }
        if (c == ']') {
            // end of array
            ++_pCur;
            return value;
        }
        if (!value.empty()) {
            // expect comma
            if (c != ',') {
                return error(ParseResult::MissComma);
            }
            ++_pCur;
            skipWhitespace();
        }
        // parse element
        auto element = parseValue();
        if (!good()) {
            return Value();
        }
        value.append(std::move(element));
    }
    // never goto here
}

}  // namespace SimpleJson

// ===== helpers =====
namespace {

NumberType validateNumber(const char* const str, const char*& end) {
    // number = [ "-" ] int [ frac ] [ exp ]
    assert(str != nullptr);
    assert(*str != 0);
    if (str == nullptr) {
        // not possible
        return NumberType::Nan;
    }

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
    } else if (*p >= '1' && *p <= '9') {
        ++p;
        while (*p >= '0' && *p <= '9') {
            ++p;
        }
    } else {
        return NumberType::Nan;
    }

    // frac = "." 1*digit
    if (*p == '.') {
        ++p;
        isReal = true;
        if (*p >= '0' && *p <= '9') {
            ++p;
            while (*p >= '0' && *p <= '9') {
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
        if (*p >= '0' && *p <= '9') {
            ++p;
            while (*p >= '0' && *p <= '9') {
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

int parseHex(const char* str, const size_t length) {
    assert(str != nullptr);
    if (str == nullptr) {
        // not possible
        return -1;
    }

    int res = 0;
    for (size_t i = 0; i < length; ++i) {
        const char c = str[i];

        int val = 0;
        if (c >= '0' && c <= '9') {
            val = c - '0';
        } else if (c >= 'A' && c <= 'F') {
            val = c - 'A' + 10;
        } else if (c >= 'a' && c <= 'f') {
            val = c - 'a' + 10;
        } else {
            return -1;
        }

        res *= 16;
        res += val;
    }

    return res;
}

}  // namespace
