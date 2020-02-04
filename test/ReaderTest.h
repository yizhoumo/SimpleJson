#ifndef SIMPLEJSON_READERTEST_H
#define SIMPLEJSON_READERTEST_H

#define EXPECT_PARSE_ERROR(expected, doc)         \
    do {                                          \
        auto value = Value(false);                \
        EXPECT_FALSE(reader.parse(doc, value));   \
        EXPECT_EQ(expected, reader.result());     \
        EXPECT_EQ(ValueType::Null, value.type()); \
    } while (false)

#define EXPECT_PARSE_BOOL(expected, doc)          \
    do {                                          \
        Value value;                              \
        EXPECT_TRUE(reader.parse(doc, value));    \
        EXPECT_EQ(ValueType::Bool, value.type()); \
        if (value.isBool()) {                     \
            EXPECT_EQ(expected, value.asBool());  \
        }                                         \
    } while (false)

#define EXPECT_PARSE_NUMBER(expected, doc)          \
    do {                                            \
        Value value;                                \
        EXPECT_TRUE(reader.parse(doc, value));      \
        EXPECT_EQ(ValueType::Number, value.type()); \
        if (value.isNumber()) {                     \
            EXPECT_EQ(expected, value.asNumber());  \
        }                                           \
    } while (false)

#endif  // SIMPLEJSON_READERTEST_H
