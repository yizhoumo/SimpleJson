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
    EXPECT_ROUNDTRIP("null");
    EXPECT_ROUNDTRIP("true");
    EXPECT_ROUNDTRIP("false");
}

TEST_F(WriterTest, RoundtripInteger) {
    EXPECT_ROUNDTRIP("0");
    EXPECT_ROUNDTRIP("1");
    EXPECT_ROUNDTRIP("-1");
    EXPECT_ROUNDTRIP("2147483647");
    EXPECT_ROUNDTRIP("-2147483648");
    EXPECT_ROUNDTRIP("9223372036854775807");
    EXPECT_ROUNDTRIP("-9223372036854775808");
}

TEST_F(WriterTest, RoundtripRealNumber) {
    EXPECT_ROUNDTRIP("1.5");
    EXPECT_ROUNDTRIP("-1.5");
    EXPECT_ROUNDTRIP("3.25");
    EXPECT_ROUNDTRIP("-3.25");
    EXPECT_ROUNDTRIP("1e+20");
    EXPECT_ROUNDTRIP("-1e+20");
    EXPECT_ROUNDTRIP("1.234e+20");
    EXPECT_ROUNDTRIP("1.234e-20");

    // Smallest number > 1
    EXPECT_ROUNDTRIP("1.0000000000000002");
    // Min. subnormal double
    EXPECT_ROUNDTRIP("4.9406564584124654e-324");
    EXPECT_ROUNDTRIP("-4.9406564584124654e-324");
    // Max. subnormal double
    EXPECT_ROUNDTRIP("2.2250738585072009e-308");
    EXPECT_ROUNDTRIP("-2.2250738585072009e-308");
    // Min. normal double
    EXPECT_ROUNDTRIP("2.2250738585072014e-308");
    EXPECT_ROUNDTRIP("-2.2250738585072014e-308");
    // Max. Double
    EXPECT_ROUNDTRIP("1.7976931348623157e+308");
    EXPECT_ROUNDTRIP("-1.7976931348623157e+308");
}

TEST_F(WriterTest, RoundtripString) {
    EXPECT_ROUNDTRIP(R"("")");
    EXPECT_ROUNDTRIP(R"("Hello")");
    EXPECT_ROUNDTRIP(R"("Hello\nWorld")");
    EXPECT_ROUNDTRIP(R"("\" \\ \/ \b \f \n \r \t")");
    EXPECT_ROUNDTRIP(R"("Hello\u0000World")");
}

TEST_F(WriterTest, RoundtripArray) {
    EXPECT_ROUNDTRIP("[]");
    EXPECT_ROUNDTRIP("[null,false,true,123,1.5,\"abc\",[1,2,3]]");
}

TEST_F(WriterTest, RoundtripObject) {
    EXPECT_ROUNDTRIP("{}");

    const auto doc = R"({"1n":null,"2f":false,"3t":true,"4i":123,"5d":1.5,)"
                     R"("6s":"abc","7a":[1,2,3],"8o":{"1":1,"2":2,"3":3}})";
    EXPECT_ROUNDTRIP(doc);
}

}  // namespace SimpleJson
