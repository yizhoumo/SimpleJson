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

    val = Null();
    EXPECT_EQ(ValueType::Null, val.type());
}

TEST(ValueTest, TypeBool) {
    Value val(ValueType::Bool);
    EXPECT_TRUE(val.isBool());
    EXPECT_EQ(ValueType::Bool, val.type());
    EXPECT_EQ(false, val.asBool());

    val = true;
    EXPECT_TRUE(val.isBool());
    EXPECT_EQ(ValueType::Bool, val.type());
    EXPECT_EQ(true, val.asBool());
}

TEST(ValueTest, TypeNumberInt) {
    Value val(ValueType::Number);
    EXPECT_TRUE(val.isNumber());
    EXPECT_EQ(ValueType::Number, val.type());
    EXPECT_EQ(0, val.asNumber());

    auto x = 12345;
    val = (int)++x;
    EXPECT_EQ(ValueType::Number, val.type());
    EXPECT_EQ(x, val.asNumber());

    val = (unsigned)++x;
    EXPECT_EQ(ValueType::Number, val.type());
    EXPECT_EQ(x, val.asNumber());

    val = (intmax_t)++x;
    EXPECT_EQ(ValueType::Number, val.type());
    EXPECT_EQ(x, val.asNumber());

    val = (uintmax_t)++x;
    EXPECT_EQ(ValueType::Number, val.type());
    EXPECT_EQ(x, val.asNumber());
}

TEST(ValueTest, TypeNumberReal) {
    Value val = 0.0;
    EXPECT_EQ(ValueType::Number, val.type());
    EXPECT_EQ(0.0, val.asNumber());

    val = 1.0;
    EXPECT_EQ(ValueType::Number, val.type());
    EXPECT_EQ(1.0, val.asNumber());

    val = 1.0f;
    EXPECT_EQ(ValueType::Number, val.type());
    EXPECT_EQ(1.0f, val.asNumber());

    val = DBL_MAX;
    EXPECT_EQ(ValueType::Number, val.type());
    EXPECT_EQ(DBL_MAX, val.asNumber());

    val = DBL_MIN;
    EXPECT_EQ(ValueType::Number, val.type());
    EXPECT_EQ(DBL_MIN, val.asNumber());
}

}  // namespace SimpleJson
