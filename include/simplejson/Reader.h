#ifndef SIMPLEJSON_READER_H
#define SIMPLEJSON_READER_H

#include <string>

#include "Value.h"

namespace SimpleJson {

enum class ParseResult { Ok, ExpectValue, InvalidValue, RootNotSingular };

class Reader {
public:
    bool parse(std::string_view document, Value& root);
    bool good() const { return _result == ParseResult::Ok; }
    ParseResult result() const { return _result; }

private:
    void skipWhitespace();
    Value parseValue();
    Value parseLiteral(std::string_view literal, Value value);
    Value error(ParseResult errorType);

private:
    // Context, valid only during parsing
    std::string_view _document;
    std::string_view::const_iterator _it;
    // Result of last round of parsing
    ParseResult _result;
};

}  // namespace SimpleJson

#endif  // SIMPLEJSON_READER_H
