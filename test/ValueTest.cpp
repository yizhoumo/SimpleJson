#include "ValueTest.h"

#include <cfloat>
#include <climits>
#include <string>

#include "TestHelper.h"
#include "gtest/gtest.h"
#include "simplejson/Value.h"

namespace SimpleJson {

TEST(ValueTest, TypeNull) {
    Value val(ValueType::Null);
    EXPECT_TRUE(val.isNull());
    EXPECT_EQ(ValueType::Null, val.type());

    val = Value();
    EXPECT_EQ(ValueType::Null, val.type());
}

TEST(ValueTest, TypeBool) {
    Value val(ValueType::Bool);
    EXPECT_TRUE(val.isBool());
    ASSERT_EQ(ValueType::Bool, val.type());
    EXPECT_EQ(false, val.asBool());

    val = true;
    EXPECT_TRUE(val.isBool());
    ASSERT_EQ(ValueType::Bool, val.type());
    EXPECT_EQ(true, val.asBool());
}

TEST(ValueTest, TypeInteger) {
    Value val(ValueType::Integer);
    EXPECT_TRUE(val.isInteger());
    ASSERT_EQ(ValueType::Integer, val.type());
    EXPECT_EQ(0, val.asInteger());
}

TEST(ValueTest, TypeIntegerSigned) {
    // signed int
    EXPECT_VALUE_INTEGER(0);
    EXPECT_VALUE_INTEGER(1);
    EXPECT_VALUE_INTEGER(-1);
    EXPECT_VALUE_INTEGER(INT_MAX);
    EXPECT_VALUE_INTEGER(INT_MIN);

    // signed long
    EXPECT_VALUE_INTEGER(0L);
    EXPECT_VALUE_INTEGER(1L);
    EXPECT_VALUE_INTEGER(-1L);
    EXPECT_VALUE_INTEGER(LONG_MAX);
    EXPECT_VALUE_INTEGER(LONG_MIN);

    // signed long long
    EXPECT_VALUE_INTEGER(0LL);
    EXPECT_VALUE_INTEGER(1LL);
    EXPECT_VALUE_INTEGER(-1LL);
    EXPECT_VALUE_INTEGER(LLONG_MAX);
    EXPECT_VALUE_INTEGER(LLONG_MIN);
}

// TODO: uint support
#if 0
TEST(ValueTest, TypeIntegerUnsigned) {
    // unsigned int
    EXPECT_VALUE_INTEGER(0U);
    EXPECT_VALUE_INTEGER(1U);
    EXPECT_VALUE_INTEGER(UINT_MAX);

    // unsigned long
    EXPECT_VALUE_INTEGER(0UL);
    EXPECT_VALUE_INTEGER(1UL);
    EXPECT_VALUE_INTEGER(ULONG_MAX);

    // unsigned long long
    EXPECT_VALUE_INTEGER(0ULL);
    EXPECT_VALUE_INTEGER(1ULL);
    EXPECT_VALUE_INTEGER(ULLONG_MAX);
}
#endif

TEST(ValueTest, TypeReal) {
    Value val(ValueType::Real);
    EXPECT_TRUE(val.isReal());
    ASSERT_EQ(ValueType::Real, val.type());
    EXPECT_EQ(0.0, val.asReal());

    EXPECT_VALUE_REAL(0.0);
    EXPECT_VALUE_REAL(1.0);
    EXPECT_VALUE_REAL(-1.0);
    EXPECT_VALUE_REAL(1.5);
    EXPECT_VALUE_REAL(-1.5);
    EXPECT_VALUE_REAL(DBL_MAX);
    EXPECT_VALUE_REAL(-DBL_MAX);
    EXPECT_VALUE_REAL(DBL_MIN);
    EXPECT_VALUE_REAL(-DBL_MIN);

    EXPECT_VALUE_REAL(0.0F);
    EXPECT_VALUE_REAL(1.0F);
    EXPECT_VALUE_REAL(-1.0F);
    EXPECT_VALUE_REAL(1.5F);
    EXPECT_VALUE_REAL(-1.5F);
    EXPECT_VALUE_REAL(FLT_MAX);
    EXPECT_VALUE_REAL(-FLT_MAX);
    EXPECT_VALUE_REAL(FLT_MIN);
    EXPECT_VALUE_REAL(-FLT_MIN);
}

TEST(ValueTest, TypeString) {
    Value val(ValueType::String);
    EXPECT_TRUE(val.isString());
    ASSERT_EQ(ValueType::String, val.type());
    EXPECT_TRUE(val.asString().empty());
    EXPECT_EQ(nullptr, val.asCString());

    val = "";
    ASSERT_EQ(ValueType::String, val.type());
    EXPECT_TRUE(val.asString().empty());
    EXPECT_EQ(nullptr, val.asCString());

    std::string str = "hello";
    val = str;
    ASSERT_EQ(ValueType::String, val.type());
    EXPECT_EQ(str, val.asString());
    EXPECT_STREQ(str.data(), val.asCString());

    str = "world";
    val = str.data();
    ASSERT_EQ(ValueType::String, val.type());
    EXPECT_EQ(str, val.asString());
    EXPECT_STREQ(str.data(), val.asCString());

    using namespace std::string_literals;
    str = "hello\0world!"s;
    val = str;
    ASSERT_EQ(ValueType::String, val.type());
    EXPECT_EQ(str, val.asString());
    EXPECT_STREQ(str.data(), val.asCString());
}

TEST(ValueTest, TypeArray) {
    Value val(ValueType::Array);
    EXPECT_TRUE(val.isArray());
    ASSERT_EQ(ValueType::Array, val.type());
    EXPECT_TRUE(val.empty());
    EXPECT_EQ(0, val.size());

    val.append(1);
    val.append(2.0);
    val.append("hello");
    val.append(val);

    ASSERT_EQ(4, val.size());
    EXPECT_EQ(1, val[0].asInteger());
    EXPECT_EQ(2.0, val[1].asReal());
    EXPECT_EQ("hello", val[2].asStringView());
    ASSERT_EQ(ValueType::Array, val[3].type());
    ASSERT_EQ(3, val[3].size());
    EXPECT_EQ(1, val[3][0].asInteger());
    EXPECT_EQ(2.0, val[3][1].asReal());
    EXPECT_EQ("hello", val[3][2].asStringView());

    val.resize(5);
    ASSERT_EQ(5, val.size());
    EXPECT_EQ(1, val[0].asInteger());
    EXPECT_EQ(2.0, val[1].asReal());
    EXPECT_EQ("hello", val[2].asStringView());
    ASSERT_EQ(ValueType::Array, val[3].type());
    ASSERT_EQ(3, val[3].size());
    EXPECT_EQ(1, val[3][0].asInteger());
    EXPECT_EQ(2.0, val[3][1].asReal());
    EXPECT_EQ("hello", val[3][2].asStringView());
    EXPECT_EQ(ValueType::Null, val[4].type());

    val.clear();
    EXPECT_TRUE(val.empty());
    EXPECT_EQ(0, val.size());
}

TEST(ValueTest, TypeObject) {
    Value val(ValueType::Object);
    EXPECT_TRUE(val.isObject());
    ASSERT_EQ(ValueType::Object, val.type());
    EXPECT_TRUE(val.empty());
    EXPECT_EQ(0, val.size());

    val["one"] = 1;
    val["two"] = 2.0;
    val["hello"] = "world";
    val["this"] = val;
    EXPECT_EQ(ValueType::Null, val["null"].type());

    const auto keys = val.getMemberNames();
    EXPECT_EQ(5, keys.size());

    EXPECT_EQ(5, val.size());
    EXPECT_EQ(1, val["one"].asInteger());
    EXPECT_EQ(2.0, val["two"].asReal());
    EXPECT_EQ("world", val["hello"].asStringView());

    ASSERT_EQ(ValueType::Object, val["this"].type());
    EXPECT_EQ(3, val["this"].size());
    EXPECT_EQ(1, val["this"]["one"].asInteger());
    EXPECT_EQ(2.0, val["this"]["two"].asReal());
    EXPECT_EQ("world", val["this"]["hello"].asStringView());

    val.clear();
    EXPECT_TRUE(val.empty());
    EXPECT_EQ(0, val.size());
}

}  // namespace SimpleJson
