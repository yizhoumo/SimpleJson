#include "ValueTest.h"

#include <cfloat>
#include <climits>

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

    val = Value(Null());
    EXPECT_EQ(ValueType::Null, val.type());
}

TEST(ValueTest, TypeBool) {
    Value val(ValueType::Bool);
    EXPECT_TRUE(val.isBool());
    EXPECT_EQ(ValueType::Bool, val.type());
    EXPECT_EQ(false, val.asBool());

    val = Value(true);
    EXPECT_TRUE(val.isBool());
    EXPECT_EQ(ValueType::Bool, val.type());
    EXPECT_EQ(true, val.asBool());
}

TEST(ValueTest, TypeInteger) {
    Value val(ValueType::Integer);
    EXPECT_TRUE(val.isInteger());
    EXPECT_EQ(ValueType::Integer, val.type());
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
    EXPECT_EQ(ValueType::Real, val.type());
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

    EXPECT_VALUE_REAL(0.0f);
    EXPECT_VALUE_REAL(1.0f);
    EXPECT_VALUE_REAL(-1.0f);
    EXPECT_VALUE_REAL(1.5f);
    EXPECT_VALUE_REAL(-1.5f);
    EXPECT_VALUE_REAL(FLT_MAX);
    EXPECT_VALUE_REAL(-FLT_MAX);
    EXPECT_VALUE_REAL(FLT_MIN);
    EXPECT_VALUE_REAL(-FLT_MIN);
}

TEST(ValueTest, TypeString) {
    Value val(ValueType::String);
    EXPECT_TRUE(val.isString());
    EXPECT_EQ(ValueType::String, val.type());
    EXPECT_EQ("", val.asString());

    std::string str = "hello";
    val = Value(str);
    EXPECT_TRUE(val.isString());
    EXPECT_EQ(ValueType::String, val.type());
    EXPECT_EQ(str, val.asString());

    str = "world";
    val = Value(str.data());
    EXPECT_TRUE(val.isString());
    EXPECT_EQ(ValueType::String, val.type());
    EXPECT_EQ(str, val.asString());
}

}  // namespace SimpleJson
