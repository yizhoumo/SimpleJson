#include "gtest/gtest.h"
#include "simplejson/Reader.h"

#define EXPECT_PARSE_ERROR(expected, doc)      \
    do {                                       \
        root = Bool();                         \
        EXPECT_FALSE(reader.parse(doc, root)); \
        EXPECT_EQ(expected, reader.result());  \
        EXPECT_TRUE(root.isNull());            \
    } while (false)

#define EXPECT_PARSE_BOOL(expected, doc)      \
    do {                                      \
        root = Null();                        \
        EXPECT_TRUE(reader.parse(doc, root)); \
        EXPECT_TRUE(root.isBool());           \
        EXPECT_EQ(expected, root.asBool());   \
    } while (false)

namespace SimpleJson {

class ReaderTest : public ::testing::Test {
protected:
    Reader reader;
    Value root;
};

TEST_F(ReaderTest, ParseNull) {
    root = Bool();
    EXPECT_TRUE(reader.parse("null", root));
    EXPECT_TRUE(root.isNull());
}

TEST_F(ReaderTest, ParseTrue) {
    EXPECT_PARSE_BOOL(true, "true");
    EXPECT_PARSE_BOOL(true, " true ");
}

TEST_F(ReaderTest, ParseFalse) {
    EXPECT_PARSE_BOOL(false, "false");
    EXPECT_PARSE_BOOL(false, "\tfalse\n");
}

TEST_F(ReaderTest, ParseExpectValue) {
    EXPECT_PARSE_ERROR(ParseResult::ExpectValue, "");
    EXPECT_PARSE_ERROR(ParseResult::ExpectValue, " ");
    EXPECT_PARSE_ERROR(ParseResult::ExpectValue, "\t \n");
}

TEST_F(ReaderTest, ParseInvalidValue) {
    EXPECT_PARSE_ERROR(ParseResult::InvalidValue, "nul");
    EXPECT_PARSE_ERROR(ParseResult::InvalidValue, "?");
}

TEST_F(ReaderTest, ParseRootNotSingular) {
    EXPECT_PARSE_ERROR(ParseResult::RootNotSingular, "null false");
    EXPECT_PARSE_ERROR(ParseResult::RootNotSingular, "true null");
}

}  // namespace SimpleJson
