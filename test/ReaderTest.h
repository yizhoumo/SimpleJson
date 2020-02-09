#ifndef SIMPLEJSON_READERTEST_H
#define SIMPLEJSON_READERTEST_H

#define EXPECT_PARSE_ERROR(expected, doc)         \
    do {                                          \
        auto value = Value(false);                \
        reader.parse(doc, value);                 \
        EXPECT_EQ(expected, reader.result());     \
        EXPECT_EQ(ValueType::Null, value.type()); \
    } while (false)

#define EXPECT_PARSE_BOOL(expected, doc)             \
    do {                                             \
        Value value;                                 \
        reader.parse(doc, value);                    \
        EXPECT_EQ(ParseResult::Ok, reader.result()); \
        EXPECT_EQ(ValueType::Bool, value.type());    \
        if (value.isBool()) {                        \
            EXPECT_EQ(expected, value.asBool());     \
        }                                            \
    } while (false)

#define EXPECT_PARSE_INTEGER(expected, doc)          \
    do {                                             \
        Value value;                                 \
        reader.parse(doc, value);                    \
        EXPECT_EQ(ParseResult::Ok, reader.result()); \
        EXPECT_EQ(ValueType::Integer, value.type()); \
        if (value.isInteger()) {                     \
            EXPECT_EQ(expected, value.asInteger());  \
        }                                            \
    } while (false)

#define EXPECT_PARSE_REAL(expected, doc)             \
    do {                                             \
        Value value;                                 \
        reader.parse(doc, value);                    \
        EXPECT_EQ(ParseResult::Ok, reader.result()); \
        EXPECT_EQ(ValueType::Real, value.type());    \
        if (value.isReal()) {                        \
            EXPECT_EQ(expected, value.asReal());     \
        }                                            \
    } while (false)

#define EXPECT_PARSE_STRING(expected, doc)           \
    do {                                             \
        Value value;                                 \
        reader.parse(doc, value);                    \
        EXPECT_EQ(ParseResult::Ok, reader.result()); \
        EXPECT_EQ(ValueType::String, value.type());  \
        if (value.isString()) {                      \
            EXPECT_EQ(expected, value.asString());   \
        }                                            \
    } while (false)

#endif  // SIMPLEJSON_READERTEST_H
