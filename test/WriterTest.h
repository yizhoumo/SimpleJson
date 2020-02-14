#ifndef SIMPLEJSON_WRITERTEST_H
#define SIMPLEJSON_WRITERTEST_H

#define ROUNDTRIP_TEST(doc)                                 \
    do {                                                    \
        const auto expectedDoc = doc;                       \
        Value expectedValue;                                \
        reader.parse(expectedDoc, expectedValue);           \
        ASSERT_EQ(ParseResult::Ok, reader.result());        \
        const auto actualDoc = writer.write(expectedValue); \
        Value actualValue;                                  \
        reader.parse(actualDoc, actualValue);               \
        ASSERT_EQ(ParseResult::Ok, reader.result());        \
        EXPECT_EQ(expectedValue, actualValue);              \
    } while (false)

#endif  // SIMPLEJSON_WRITERTEST_H
