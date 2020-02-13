#ifndef SIMPLEJSON_WRITER_H
#define SIMPLEJSON_WRITER_H

#include <string>

#include "simplejson/Value.h"

namespace SimpleJson {

class Writer {
public:
    std::string write(const Value& root);

private:
    void stringifyValue(const Value& root);
    void stringifyReal(Real number);
    void stringifyString(std::string_view str);
    void stringifyArray(const Value& root);
    void stringifyObject(const Value& root);

private:
    std::string _strBuf;
};

}  // namespace SimpleJson

#endif  // SIMPLEJSON_WRITER_H
