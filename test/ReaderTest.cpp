#include "ReaderTest.h"

#include "TestHelper.h"
#include "gtest/gtest.h"
#include "simplejson/Reader.h"

namespace SimpleJson {

class ReaderTest : public testing::Test {
protected:
    Reader reader;
};

TEST_F(ReaderTest, ParseNull) {
    auto value = Value(false);
    EXPECT_TRUE(reader.parse("null", value));
    EXPECT_EQ(ValueType::Null, value.type());
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

TEST_F(ReaderTest, ParseInteger) {
    EXPECT_PARSE_INTEGER(0, " 0");
    EXPECT_PARSE_INTEGER(0, "-0");
    EXPECT_PARSE_INTEGER(1, "  1");
    EXPECT_PARSE_INTEGER(-1, "-1");
    EXPECT_PARSE_INTEGER(INT32_MAX, " 2147483647");
    EXPECT_PARSE_INTEGER(INT32_MIN, "-2147483648");
    EXPECT_PARSE_INTEGER(INT64_MAX, " 9223372036854775807");
    EXPECT_PARSE_INTEGER(INT64_MIN, "-9223372036854775808");
}

TEST_F(ReaderTest, ParseRealNumber) {
    EXPECT_PARSE_REAL(0.0, " 0.0");
    EXPECT_PARSE_REAL(0.0, "-0.0");
    EXPECT_PARSE_REAL(1.0, "  1.0");
    EXPECT_PARSE_REAL(-1.0, "-1.0");
    EXPECT_PARSE_REAL(1.5, "  1.5");
    EXPECT_PARSE_REAL(-1.5, "-1.5");
    EXPECT_PARSE_REAL(3.1416, "  3.1416");
    EXPECT_PARSE_REAL(-3.1416, "-3.1416");
    EXPECT_PARSE_REAL(1E10, "1E10");
    EXPECT_PARSE_REAL(1e10, "1e10");
    EXPECT_PARSE_REAL(1E+10, "1E+10");
    EXPECT_PARSE_REAL(1E-10, "1E-10");
    EXPECT_PARSE_REAL(-1E10, "-1E10");
    EXPECT_PARSE_REAL(-1e10, "-1e10");
    EXPECT_PARSE_REAL(-1E+10, "-1E+10");
    EXPECT_PARSE_REAL(-1E-10, "-1E-10");
    EXPECT_PARSE_REAL(1.234E+10, "1.234E+10");
    EXPECT_PARSE_REAL(1.234E-10, "1.234E-10");
    EXPECT_PARSE_REAL(1E10, "1E010");

    // must underflow
    EXPECT_PARSE_REAL(0.0, "1E-10000");
    // Min. subnormal positive double
    EXPECT_PARSE_REAL(4.9406564584124654E-324, "  4.9406564584124654E-324");
    EXPECT_PARSE_REAL(-4.9406564584124654E-324, "-4.9406564584124654E-324");
    // Min. normal positive double
    EXPECT_PARSE_REAL(2.2250738585072014E-308, "  2.2250738585072014E-308");
    EXPECT_PARSE_REAL(-2.2250738585072014E-308, "-2.2250738585072014E-308");
    // Max. Double
    EXPECT_PARSE_REAL(1.7976931348623157E308, "  1.7976931348623157E308");
    EXPECT_PARSE_REAL(-1.7976931348623157E308, "-1.7976931348623157E308");
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

TEST_F(ReaderTest, ParseNumberOverflow) {
    EXPECT_PARSE_ERROR(ParseResult::NumberOverflow, " 9223372036854775808");
    EXPECT_PARSE_ERROR(ParseResult::NumberOverflow, "-9223372036854775809");

    EXPECT_PARSE_ERROR(ParseResult::NumberOverflow, " 1e309");
    EXPECT_PARSE_ERROR(ParseResult::NumberOverflow, "-1e309");
}

}  // namespace SimpleJson
