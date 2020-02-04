#ifndef SIMPLEJSON_VALUETEST_H
#define SIMPLEJSON_VALUETEST_H

#define EXPECT_VALUE_NUMBER(number)                 \
    do {                                            \
        auto value = Value(number);                 \
        EXPECT_EQ(ValueType::Number, value.type()); \
        if (value.isNumber()) {                     \
            EXPECT_EQ(number, value.asNumber());    \
        }                                           \
    } while (false)

#endif  // SIMPLEJSON_VALUETEST_H
