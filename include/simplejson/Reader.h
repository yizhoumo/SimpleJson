#ifndef SIMPLEJSON_READER_H
#define SIMPLEJSON_READER_H

#include <string>

#include "Value.h"

namespace SimpleJson {

enum class ParseResult {
    Ok,
    ExpectValue,
    InvalidValue,
    RootNotSingular,
    NumberOverflow,
    MissQuotationMark,
    InvalidStringEscape,
    InvalidStringChar
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
    Value error(ParseResult errorType);
    Value parseValue();
    Value parseLiteral(std::string_view literal, Value value);
    Value parseNumber();
    Value parseInteger(const char* numberEnd);
    Value parseReal(const char* numberEnd);
    Value parseString();

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
