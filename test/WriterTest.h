#ifndef SIMPLEJSON_WRITERTEST_H
#define SIMPLEJSON_WRITERTEST_H

#define EXPECT_ROUNDTRIP(doc)                        \
    do {                                             \
        const auto expected = doc;                   \
        Value root;                                  \
        reader.parse(expected, root);                \
        ASSERT_EQ(ParseResult::Ok, reader.result()); \
        const auto actual = writer.write(root);      \
        EXPECT_EQ(expected, actual);                 \
    } while (false)

#endif  // SIMPLEJSON_WRITERTEST_H
