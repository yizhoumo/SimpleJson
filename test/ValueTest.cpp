#include "gtest/gtest.h"
#include "simplejson/Value.h"

namespace SimpleJson {

TEST(ValueTest, TypeNull) {
    Value val(ValueType::Null);
    EXPECT_TRUE(val.isNull());
    EXPECT_EQ(ValueType::Null, val.type());

    val = Value();
    EXPECT_TRUE(val.isNull());

    val = Null();
    EXPECT_TRUE(val.isNull());
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

}
