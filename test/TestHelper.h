#ifndef SIMPLEJSON_TESTHELPER_H
#define SIMPLEJSON_TESTHELPER_H

#include <simplejson/Reader.h>
#include <simplejson/Value.h>

#include <ostream>

// for gtest to print SimpleJson::ValueType
std::ostream& operator<<(std::ostream& out, SimpleJson::ValueType val);

// for gtest to print SimpleJson::ParseResult
std::ostream& operator<<(std::ostream& out, SimpleJson::ParseResult val);

#endif  // SIMPLEJSON_TESTHELPER_H
