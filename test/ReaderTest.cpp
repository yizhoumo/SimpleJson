#include "ReaderTest.h"

#include "TestHelper.h"
#include "gtest/gtest.h"
#include "simplejson/Reader.h"

namespace SimpleJson {

class ReaderTest : public testing::Test {
protected:
    Reader reader;
    Value root;
};

TEST_F(ReaderTest, ParseNull) {
    root = Bool();
    EXPECT_TRUE(reader.parse("null", root));
    EXPECT_TRUE(root.isNull());
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
    EXPECT_PARSE_ERROR(ParseResult::RootNotSingular, "null x");
    EXPECT_PARSE_ERROR(ParseResult::RootNotSingular, "null false");
    EXPECT_PARSE_ERROR(ParseResult::RootNotSingular, "true null");
}

TEST_F(ReaderTest, ParseBool) {
    EXPECT_PARSE_BOOL(true, "true");
    EXPECT_PARSE_BOOL(true, " true ");
    EXPECT_PARSE_BOOL(false, "false");
    EXPECT_PARSE_BOOL(false, "\tfalse\n");
}

TEST_F(ReaderTest, ParseNumberOk) {
    EXPECT_PARSE_NUMBER(0.0, "0");
    EXPECT_PARSE_NUMBER(0.0, "0.0");
    EXPECT_PARSE_NUMBER(0.0, "-0");
    EXPECT_PARSE_NUMBER(0.0, "-0.0");
    EXPECT_PARSE_NUMBER(1.0, "1");
    EXPECT_PARSE_NUMBER(-1.0, "-1");
    EXPECT_PARSE_NUMBER(1.5, "1.5");
    EXPECT_PARSE_NUMBER(-1.5, "-1.5");
    EXPECT_PARSE_NUMBER(3.1416, "3.1416");
    EXPECT_PARSE_NUMBER(-3.1416, "-3.1416");
    EXPECT_PARSE_NUMBER(1E10, "1E10");
    EXPECT_PARSE_NUMBER(1e10, "1e10");
    EXPECT_PARSE_NUMBER(1E+10, "1E+10");
    EXPECT_PARSE_NUMBER(1E-10, "1E-10");
    EXPECT_PARSE_NUMBER(-1E10, "-1E10");
    EXPECT_PARSE_NUMBER(-1e10, "-1e10");
    EXPECT_PARSE_NUMBER(-1E+10, "-1E+10");
    EXPECT_PARSE_NUMBER(-1E-10, "-1E-10");
    EXPECT_PARSE_NUMBER(1.234E+10, "1.234E+10");
    EXPECT_PARSE_NUMBER(1.234E-10, "1.234E-10");
    EXPECT_PARSE_NUMBER(1E10, "1E010");

    // must underflow
    EXPECT_PARSE_NUMBER(0.0, "1E-10000");
    // Min. subnormal positive double
    EXPECT_PARSE_NUMBER(4.9406564584124654E-324, "4.9406564584124654E-324");
    EXPECT_PARSE_NUMBER(-4.9406564584124654E-324, "-4.9406564584124654E-324");
    // Min. normal positive double
    EXPECT_PARSE_NUMBER(2.2250738585072014E-308, "2.2250738585072014E-308");
    EXPECT_PARSE_NUMBER(-2.2250738585072014E-308, "-2.2250738585072014E-308");
    // Max. Double
    EXPECT_PARSE_NUMBER(1.7976931348623157E308, "1.7976931348623157E308");
    EXPECT_PARSE_NUMBER(-1.7976931348623157E308, "-1.7976931348623157E308");
}

TEST_F(ReaderTest, ParseNumberInvalid) {
    EXPECT_PARSE_ERROR(ParseResult::InvalidValue, "-");
    EXPECT_PARSE_ERROR(ParseResult::InvalidValue, "+0");
    EXPECT_PARSE_ERROR(ParseResult::InvalidValue, "+1");
    EXPECT_PARSE_ERROR(ParseResult::InvalidValue, ".123");
    EXPECT_PARSE_ERROR(ParseResult::InvalidValue, "1.");
    EXPECT_PARSE_ERROR(ParseResult::InvalidValue, "1e");
    EXPECT_PARSE_ERROR(ParseResult::InvalidValue, "INF");
    EXPECT_PARSE_ERROR(ParseResult::InvalidValue, "inf");
    EXPECT_PARSE_ERROR(ParseResult::InvalidValue, "NAN");
    EXPECT_PARSE_ERROR(ParseResult::InvalidValue, "nan");

    // root not singular
    EXPECT_PARSE_ERROR(ParseResult::RootNotSingular, "0123");
    EXPECT_PARSE_ERROR(ParseResult::RootNotSingular, "0x0");
    EXPECT_PARSE_ERROR(ParseResult::RootNotSingular, "0x123");
}

TEST_F(ReaderTest, ParseNumberTooBig) {
    EXPECT_PARSE_ERROR(ParseResult::NumberTooBig, "1e309");
    EXPECT_PARSE_ERROR(ParseResult::NumberTooBig, "-1e309");
}

}  // namespace SimpleJson
