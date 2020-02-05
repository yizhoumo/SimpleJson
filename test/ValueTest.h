#ifndef SIMPLEJSON_VALUETEST_H
#define SIMPLEJSON_VALUETEST_H

#define EXPECT_VALUE_INTEGER(number)                 \
    do {                                             \
        auto value = Value(number);                  \
        EXPECT_EQ(ValueType::Integer, value.type()); \
        if (value.isInteger()) {                     \
            EXPECT_EQ(number, value.asInteger());    \
        }                                            \
    } while (false)

#define EXPECT_VALUE_REAL(number)                 \
    do {                                          \
        auto value = Value(number);               \
        EXPECT_EQ(ValueType::Real, value.type()); \
        if (value.isReal()) {                     \
            EXPECT_EQ(number, value.asReal());    \
        }                                         \
    } while (false)

#endif  // SIMPLEJSON_VALUETEST_H
