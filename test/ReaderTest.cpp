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

TEST_F(ReaderTest, ParseStringMissQuotationMark) {
    EXPECT_PARSE_ERROR(ParseResult::MissQuotationMark, R"(")");
    EXPECT_PARSE_ERROR(ParseResult::MissQuotationMark, R"("abc)");
}

TEST_F(ReaderTest, ParseStringInvalidStringEscape) {
    EXPECT_PARSE_ERROR(ParseResult::InvalidStringEscape, R"("\v")");
    EXPECT_PARSE_ERROR(ParseResult::InvalidStringEscape, R"("\'")");
    EXPECT_PARSE_ERROR(ParseResult::InvalidStringEscape, R"("\0")");
    EXPECT_PARSE_ERROR(ParseResult::InvalidStringEscape, R"("\x12")");
}

TEST_F(ReaderTest, ParseStringInvalidStringChar) {
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

TEST_F(ReaderTest, ParseStringInvalidUnicodeHex) {
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
    EXPECT_PARSE_ERROR(ParseResult::InvalidUnicodeHex, R"("\u 123")");
    EXPECT_PARSE_ERROR(ParseResult::InvalidUnicodeHex, R"("\uD800\u")");
}

TEST_F(ReaderTest, ParseStringInvalidUnicodeSurrogate) {
    EXPECT_PARSE_ERROR(ParseResult::InvalidUnicodeSurrogate, R"("\uD800")");
    EXPECT_PARSE_ERROR(ParseResult::InvalidUnicodeSurrogate, R"("\uD8FF")");
    EXPECT_PARSE_ERROR(ParseResult::InvalidUnicodeSurrogate, R"("\uD800\\")");
    EXPECT_PARSE_ERROR(ParseResult::InvalidUnicodeSurrogate,
                       R"("\uD800\uDBFF")");
    EXPECT_PARSE_ERROR(ParseResult::InvalidUnicodeSurrogate,
                       R"("\uD800\uE000")");
}

TEST_F(ReaderTest, ParseArrayEmpty) {
    Value value;
    reader.parse("[ ]", value);
    EXPECT_EQ(ParseResult::Ok, reader.result());
    ASSERT_EQ(ValueType::Array, value.type());
    EXPECT_TRUE(value.empty());
    EXPECT_EQ(0, value.size());
}

TEST_F(ReaderTest, ParseArray) {
    Value value;
    auto doc = "[ null , false , true , 123 , \"abc\" ]";
    reader.parse(doc, value);
    EXPECT_EQ(ParseResult::Ok, reader.result());
    ASSERT_EQ(ValueType::Array, value.type());

    ASSERT_EQ(5, value.size());
    EXPECT_EQ(ValueType::Null, value[0].type());
    EXPECT_EQ(false, value[1].asBool());
    EXPECT_EQ(true, value[2].asBool());
    EXPECT_EQ(123, value[3].asInteger());
    EXPECT_EQ("abc", value[4].asStringView());
}

TEST_F(ReaderTest, ParseArray2D) {
    Value value;
    auto doc = "[ [ ] , [ 0 ] , [ 0 , 1 ] , [ 0 , 1 , 2 ] ]";
    reader.parse(doc, value);
    EXPECT_EQ(ParseResult::Ok, reader.result());
    ASSERT_EQ(ValueType::Array, value.type());

    ASSERT_EQ(4, value.size());

    for (int i = 0; i < 4; ++i) {
        ASSERT_EQ(ValueType::Array, value[i].type());
        ASSERT_EQ(i, value[i].size());
        for (int j = 0; j < i; ++j) {
            ASSERT_EQ(ValueType::Integer, value[i][j].type());
            EXPECT_EQ(j, value[i][j].asInteger());
        }
    }
}

TEST_F(ReaderTest, ParseArrayInvalidValue) {
    EXPECT_PARSE_ERROR(ParseResult::InvalidValue, "[1,]");
    EXPECT_PARSE_ERROR(ParseResult::InvalidValue, "[\"a\", nul]");
}

TEST_F(ReaderTest, ParseArrayMissComma) {
    EXPECT_PARSE_ERROR(ParseResult::MissComma, "[1 2 ]");
    EXPECT_PARSE_ERROR(ParseResult::MissComma, "[1 }");
}

TEST_F(ReaderTest, ParseArrayMissSquareBracket) {
    EXPECT_PARSE_ERROR(ParseResult::MissSquareBracket, "[1 ");
    EXPECT_PARSE_ERROR(ParseResult::MissSquareBracket, "[1 , 2 ");
    EXPECT_PARSE_ERROR(ParseResult::MissSquareBracket, "[ [ ] ");
}

TEST_F(ReaderTest, ParseObjectEmpty) {
    Value value;
    reader.parse("{ }", value);
    EXPECT_EQ(ParseResult::Ok, reader.result());
    ASSERT_EQ(ValueType::Object, value.type());
    EXPECT_TRUE(value.empty());
    EXPECT_EQ(0, value.size());
}

TEST_F(ReaderTest, ParseObject) {
    const auto doc = R"({
        "n" : null ,
        "f" : false ,
        "t" : true ,
        "i" : 123 ,
        "d" : 1.23 ,
        "s" : "hello" ,
        "a" : [ 1 , 2 , 3 ] ,
        "o" : { "1": 1 , "2" : 2 , "3" : 3 }
    })";

    Value value;
    reader.parse(doc, value);
    EXPECT_EQ(ParseResult::Ok, reader.result());
    ASSERT_EQ(ValueType::Object, value.type());
    EXPECT_EQ(8, value.size());

    EXPECT_EQ(ValueType::Null, value["n"].type());
    EXPECT_EQ(false, value["f"].asBool());
    EXPECT_EQ(true, value["t"].asBool());
    EXPECT_EQ(123, value["i"].asInteger());
    EXPECT_EQ(1.23, value["d"].asReal());
    EXPECT_EQ("hello", value["s"].asStringView());

    ASSERT_EQ(ValueType::Array, value["a"].type());
    ASSERT_EQ(3, value["a"].size());
    EXPECT_EQ(1, value["a"][0].asInteger());
    EXPECT_EQ(2, value["a"][1].asInteger());
    EXPECT_EQ(3, value["a"][2].asInteger());

    ASSERT_EQ(ValueType::Object, value["o"].type());
    EXPECT_EQ(3, value["o"].size());
    EXPECT_EQ(1, value["o"]["1"].asInteger());
    EXPECT_EQ(2, value["o"]["2"].asInteger());
    EXPECT_EQ(3, value["o"]["3"].asInteger());
}

TEST_F(ReaderTest, ParseObjectMissKey) {
    EXPECT_PARSE_ERROR(ParseResult::MissKey, "{:1,");
    EXPECT_PARSE_ERROR(ParseResult::MissKey, "{1:1,");
    EXPECT_PARSE_ERROR(ParseResult::MissKey, "{true:1,");
    EXPECT_PARSE_ERROR(ParseResult::MissKey, "{false:1,");
    EXPECT_PARSE_ERROR(ParseResult::MissKey, "{null:1,");
    EXPECT_PARSE_ERROR(ParseResult::MissKey, "{[]:1,");
    EXPECT_PARSE_ERROR(ParseResult::MissKey, "{{}:1,");
    EXPECT_PARSE_ERROR(ParseResult::MissKey, "{\"a\":1,");
}

TEST_F(ReaderTest, ParseObjectMissColon) {
    EXPECT_PARSE_ERROR(ParseResult::MissColon, R"({"a"})");
    EXPECT_PARSE_ERROR(ParseResult::MissColon, R"({"a","b"})");
}

TEST_F(ReaderTest, ParseObjectMissComma) {
    EXPECT_PARSE_ERROR(ParseResult::MissComma, R"({"a":1])");
    EXPECT_PARSE_ERROR(ParseResult::MissComma, R"({"a":1 "b")");
}

TEST_F(ReaderTest, ParseObjectMissCurlyBracket) {
    EXPECT_PARSE_ERROR(ParseResult::MissCurlyBracket, R"({"a":1)");
    EXPECT_PARSE_ERROR(ParseResult::MissCurlyBracket, R"({"a":{})");
}

TEST_F(ReaderTest, ParseObjectInvalidValue) {
    EXPECT_PARSE_ERROR(ParseResult::InvalidValue, R"({"a": nul})");
    EXPECT_PARSE_ERROR(ParseResult::MissQuotationMark, R"({"abc)");
}

}  // namespace SimpleJson
