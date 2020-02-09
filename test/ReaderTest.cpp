#include "ReaderTest.h"

#include <cstdint>

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
}

TEST_F(ReaderTest, ParseRealNumberEdgeCase) {
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

TEST_F(ReaderTest, ParseString) {
    EXPECT_PARSE_STRING("", R"("")");
    EXPECT_PARSE_STRING("Hello", R"("Hello")");
    EXPECT_PARSE_STRING("Hello\nWorld", R"("Hello\nWorld")");

    EXPECT_PARSE_STRING("\" \\ / \b \f \n \r \t",
                        R"("\" \\ \/ \b \f \n \r \t")");
}

TEST_F(ReaderTest, ParseMissQuotationMark) {
    EXPECT_PARSE_ERROR(ParseResult::MissQuotationMark, R"(")");
    EXPECT_PARSE_ERROR(ParseResult::MissQuotationMark, R"("abc)");
}

TEST_F(ReaderTest, ParseInvalidStringEscape) {
    EXPECT_PARSE_ERROR(ParseResult::InvalidStringEscape, R"("\v")");
    EXPECT_PARSE_ERROR(ParseResult::InvalidStringEscape, R"("\'")");
    EXPECT_PARSE_ERROR(ParseResult::InvalidStringEscape, R"("\0")");
    EXPECT_PARSE_ERROR(ParseResult::InvalidStringEscape, R"("\x12")");
}

TEST_F(ReaderTest, ParseInvalidStringChar) {
    EXPECT_PARSE_ERROR(ParseResult::InvalidStringChar, "\"\x01\"");
    EXPECT_PARSE_ERROR(ParseResult::InvalidStringChar, "\"\x1F\"");
}

TEST_F(ReaderTest, ParseStringUnicode) {
    using namespace std::string_literals;
    EXPECT_PARSE_STRING("Hello\0World"s, R"("Hello\u0000World")");

    // Dollar sign U+0024
    EXPECT_PARSE_STRING("\x24", R"("\u0024")");
    // Cents sign U+00A2
    EXPECT_PARSE_STRING("\xC2\xA2", R"("\u00A2")");
    // Euro sign U+20AC
    EXPECT_PARSE_STRING("\xE2\x82\xAC", R"("\u20AC")");

    // G clef sign U+1D11E
    EXPECT_PARSE_STRING("\xF0\x9D\x84\x9E", R"("\uD834\uDD1E")");
    EXPECT_PARSE_STRING("\xF0\x9D\x84\x9E", R"("\ud834\udd1e")");
}

TEST_F(ReaderTest, ParseInvalidUnicodeHex) {
    EXPECT_PARSE_ERROR(ParseResult::InvalidUnicodeHex, R"("\u")");
    EXPECT_PARSE_ERROR(ParseResult::InvalidUnicodeHex, R"("\u0")");
    EXPECT_PARSE_ERROR(ParseResult::InvalidUnicodeHex, R"("\u01")");
    EXPECT_PARSE_ERROR(ParseResult::InvalidUnicodeHex, R"("\u012")");
    EXPECT_PARSE_ERROR(ParseResult::InvalidUnicodeHex, R"("\u/000")");
    EXPECT_PARSE_ERROR(ParseResult::InvalidUnicodeHex, R"("\uG000")");
    EXPECT_PARSE_ERROR(ParseResult::InvalidUnicodeHex, R"("\u0/00")");
    EXPECT_PARSE_ERROR(ParseResult::InvalidUnicodeHex, R"("\u0G00")");
    EXPECT_PARSE_ERROR(ParseResult::InvalidUnicodeHex, R"("\u00/0")");
    EXPECT_PARSE_ERROR(ParseResult::InvalidUnicodeHex, R"("\u00G0")");
    EXPECT_PARSE_ERROR(ParseResult::InvalidUnicodeHex, R"("\u000/")");
    EXPECT_PARSE_ERROR(ParseResult::InvalidUnicodeHex, R"("\u000G")");
}

TEST_F(ReaderTest, ParseInvalidUnicodeSurrogate) {
    EXPECT_PARSE_ERROR(ParseResult::InvalidUnicodeSurrogate, R"("\uD800")");
    EXPECT_PARSE_ERROR(ParseResult::InvalidUnicodeSurrogate, R"("\uD8FF")");
    EXPECT_PARSE_ERROR(ParseResult::InvalidUnicodeSurrogate, R"("\uD800\\")");
    EXPECT_PARSE_ERROR(ParseResult::InvalidUnicodeSurrogate,
                       R"("\uD800\uDBFF")");
    EXPECT_PARSE_ERROR(ParseResult::InvalidUnicodeSurrogate,
                       R"("\uD800\uE000")");
}

}  // namespace SimpleJson
