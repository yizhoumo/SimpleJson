#ifndef SIMPLEJSON_READER_H
#define SIMPLEJSON_READER_H

#include <string>

#include "Value.h"

namespace SimpleJson {

enum class [[nodiscard]] ParseResult{
    Ok,
    ExpectValue,
    InvalidValue,
    RootNotSingular,
    NumberOverflow,
    MissQuotationMark,
    InvalidStringEscape,
    InvalidStringChar,
    InvalidUnicodeHex,
    InvalidUnicodeSurrogate,
    MissComma,
    MissSquareBracket,
};

class Reader {
public:
    bool parse(const char* pDocument, Value& root);
    bool parse(const std::string& document, Value& root) {
        return parse(document.data(), root);
    }

public:
    [[nodiscard]] bool good() const { return _result == ParseResult::Ok; }
    [[nodiscard]] ParseResult result() const { return _result; }

private:
    void skipWhitespace();
    [[nodiscard]] Value error(ParseResult errorType);
    [[nodiscard]] Value parseValue();
    [[nodiscard]] Value parseLiteral(std::string_view literal, Value value);
    [[nodiscard]] Value parseNumber();
    [[nodiscard]] Value parseInteger(const char* numberEnd);
    [[nodiscard]] Value parseReal(const char* numberEnd);
    [[nodiscard]] Value parseString();
    [[nodiscard]] ParseResult parseEscaped();
    [[nodiscard]] ParseResult parseUnicode();
    void encodeUnicode(unsigned codePoint);
    [[nodiscard]] Value parseArray();

private:
    // Current location of document, valid only during parsing
    const char* _pCur = nullptr;
    // Result of last round of parsing
    ParseResult _result = ParseResult::Ok;
    // Buffer of string
    std::string _strBuf;
};

}  // namespace SimpleJson

#endif  // SIMPLEJSON_READER_H
