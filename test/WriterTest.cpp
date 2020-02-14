#include "WriterTest.h"

#include "TestHelper.h"
#include "gtest/gtest.h"
#include "simplejson/Reader.h"
#include "simplejson/Writer.h"

namespace SimpleJson {

class WriterTest : public testing::Test {
protected:
    Reader reader;
    Writer writer;
};

TEST_F(WriterTest, Roundtrip) {
    ROUNDTRIP_TEST("null");
    ROUNDTRIP_TEST("true");
    ROUNDTRIP_TEST("false");
}

TEST_F(WriterTest, RoundtripInteger) {
    ROUNDTRIP_TEST("0");
    ROUNDTRIP_TEST("1");
    ROUNDTRIP_TEST("-1");
    ROUNDTRIP_TEST("2147483647");
    ROUNDTRIP_TEST("-2147483648");
    ROUNDTRIP_TEST("9223372036854775807");
    ROUNDTRIP_TEST("-9223372036854775808");
}

TEST_F(WriterTest, RoundtripRealNumber) {
    ROUNDTRIP_TEST("1.5");
    ROUNDTRIP_TEST("-1.5");
    ROUNDTRIP_TEST("3.25");
    ROUNDTRIP_TEST("-3.25");
    ROUNDTRIP_TEST("1e+20");
    ROUNDTRIP_TEST("-1e+20");
    ROUNDTRIP_TEST("1e-20");
    ROUNDTRIP_TEST("-1e-20");
    ROUNDTRIP_TEST("1.234e+20");
    ROUNDTRIP_TEST("1.234e-20");

    // Smallest number > 1
    ROUNDTRIP_TEST("1.0000000000000002");
    // Min. subnormal double
    ROUNDTRIP_TEST("4.9406564584124654e-324");
    ROUNDTRIP_TEST("-4.9406564584124654e-324");
    // Max. subnormal double
    ROUNDTRIP_TEST("2.2250738585072009e-308");
    ROUNDTRIP_TEST("-2.2250738585072009e-308");
    // Min. normal double
    ROUNDTRIP_TEST("2.2250738585072014e-308");
    ROUNDTRIP_TEST("-2.2250738585072014e-308");
    // Max. Double
    ROUNDTRIP_TEST("1.7976931348623157e+308");
    ROUNDTRIP_TEST("-1.7976931348623157e+308");
}

TEST_F(WriterTest, RoundtripString) {
    ROUNDTRIP_TEST(R"("")");
    ROUNDTRIP_TEST(R"("Hello")");
    ROUNDTRIP_TEST(R"("Hello\nWorld")");
    ROUNDTRIP_TEST(R"("\" \\ \/ \b \f \n \r \t")");
    ROUNDTRIP_TEST(R"("Hello\u0000World")");
}

TEST_F(WriterTest, RoundtripArray) {
    ROUNDTRIP_TEST("[]");
    ROUNDTRIP_TEST("[null,false,true,123,1.5,\"abc\",[1,2,3]]");
}

TEST_F(WriterTest, RoundtripObject) {
    ROUNDTRIP_TEST("{}");

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
    ROUNDTRIP_TEST(doc);
}

}  // namespace SimpleJson
