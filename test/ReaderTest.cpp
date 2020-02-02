#include "gtest/gtest.h"
#include "simplejson/Reader.h"

namespace SimpleJson {

class ReaderTest : public ::testing::Test {
protected:
    // void SetUp() override {}
    // void TearDown() override {}

    Reader reader;
    Value root = Bool();
};

TEST_F(ReaderTest, ParseNull) {
    EXPECT_TRUE(reader.parse("null", root));
    EXPECT_EQ(ParseResult::Ok, reader.result());
    EXPECT_TRUE(reader.good());
    EXPECT_TRUE(root.isNull());
}

TEST_F(ReaderTest, ParseTrue) {
    root = Null();
    EXPECT_TRUE(reader.parse("true", root));
    EXPECT_TRUE(reader.good());
    EXPECT_TRUE(root.isBool());
    EXPECT_EQ(true, root.asBool());
}

TEST_F(ReaderTest, ParseFalse) {
    root = Null();
    EXPECT_TRUE(reader.parse("false", root));
    EXPECT_TRUE(reader.good());
    EXPECT_TRUE(root.isBool());
    EXPECT_EQ(false, root.asBool());
}

TEST_F(ReaderTest, ParseExpectValue) {
    EXPECT_FALSE(reader.parse("", root));
    EXPECT_EQ(ParseResult::ExpectValue, reader.result());
    EXPECT_TRUE(root.isNull());

    root = Bool();
    EXPECT_FALSE(reader.parse(" ", root));
    EXPECT_EQ(ParseResult::ExpectValue, reader.result());
    EXPECT_TRUE(root.isNull());
}

TEST_F(ReaderTest, ParseInvalidValue) {
    EXPECT_FALSE(reader.parse("nul", root));
    EXPECT_EQ(ParseResult::InvalidValue, reader.result());
    EXPECT_TRUE(root.isNull());

    root = Bool();
    EXPECT_FALSE(reader.parse("?", root));
    EXPECT_EQ(ParseResult::InvalidValue, reader.result());
    EXPECT_TRUE(root.isNull());
}

TEST_F(ReaderTest, ParseRootNotSingular) {
    EXPECT_FALSE(reader.parse("null false", root));
    EXPECT_EQ(ParseResult::RootNotSingular, reader.result());
    EXPECT_TRUE(root.isNull());
}

}  // namespace SimpleJson
