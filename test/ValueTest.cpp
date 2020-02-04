#include "ValueTest.h"

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

TEST(ValueTest, TypeNumber) {
    Value val(ValueType::Number);
    EXPECT_TRUE(val.isNumber());
    EXPECT_EQ(ValueType::Number, val.type());
    EXPECT_EQ(0, val.asNumber());
}

TEST(ValueTest, TypeNumberInt) {
    // signed
    EXPECT_VALUE_NUMBER(0);
    EXPECT_VALUE_NUMBER(1);
    EXPECT_VALUE_NUMBER(-1);
    EXPECT_VALUE_NUMBER(0L);
    EXPECT_VALUE_NUMBER(1L);
    EXPECT_VALUE_NUMBER(-1L);
    EXPECT_VALUE_NUMBER(0LL);
    EXPECT_VALUE_NUMBER(1LL);
    EXPECT_VALUE_NUMBER(-1LL);

    // unsigned
    EXPECT_VALUE_NUMBER(0U);
    EXPECT_VALUE_NUMBER(1U);
    EXPECT_VALUE_NUMBER(0UL);
    EXPECT_VALUE_NUMBER(1UL);
    EXPECT_VALUE_NUMBER(0ULL);
    EXPECT_VALUE_NUMBER(1ULL);
}

TEST(ValueTest, TypeNumberReal) {
    EXPECT_VALUE_NUMBER(0.0);
    EXPECT_VALUE_NUMBER(1.0);
    EXPECT_VALUE_NUMBER(-1.0);
    EXPECT_VALUE_NUMBER(1.5);
    EXPECT_VALUE_NUMBER(-1.5);

    EXPECT_VALUE_NUMBER(0.0f);
    EXPECT_VALUE_NUMBER(1.0f);
    EXPECT_VALUE_NUMBER(-1.0f);
    EXPECT_VALUE_NUMBER(1.5f);
    EXPECT_VALUE_NUMBER(-1.5f);

    EXPECT_VALUE_NUMBER(DBL_MAX);
    EXPECT_VALUE_NUMBER(-DBL_MAX);
    EXPECT_VALUE_NUMBER(DBL_MIN);
    EXPECT_VALUE_NUMBER(-DBL_MIN);
}

}  // namespace SimpleJson
