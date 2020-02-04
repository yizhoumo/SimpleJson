#ifndef SIMPLEJSON_READERTEST_H
#define SIMPLEJSON_READERTEST_H

#define EXPECT_PARSE_ERROR(expected, doc)        \
    do {                                         \
        root = Bool();                           \
        EXPECT_FALSE(reader.parse(doc, root));   \
        EXPECT_EQ(expected, reader.result());    \
        EXPECT_EQ(ValueType::Null, root.type()); \
    } while (false)

#define EXPECT_PARSE_BOOL(expected, doc)         \
    do {                                         \
        root = Null();                           \
        EXPECT_TRUE(reader.parse(doc, root));    \
        EXPECT_EQ(ValueType::Bool, root.type()); \
        EXPECT_EQ(expected, root.asBool());      \
    } while (false)

#define EXPECT_PARSE_NUMBER(expected, doc)         \
    do {                                           \
        root = Null();                             \
        EXPECT_TRUE(reader.parse(doc, root));      \
        EXPECT_EQ(ValueType::Number, root.type()); \
        EXPECT_EQ(expected, root.asNumber());      \
    } while (false)

#endif  // SIMPLEJSON_READERTEST_H
