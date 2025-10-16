#include "gtest/gtest.h"

#include "nth_power.hh"
#include "matrix2x2.hh"

TEST(NthPowerTest, GenericFunctionBehavesCorrectly) {
    EXPECT_EQ(nth_power(2u, 11u), (1u << 11));
    EXPECT_EQ(nth_power(123, 0), 1);
    EXPECT_DOUBLE_EQ(nth_power(2.0, 11), 2048.0);

    Matrix2x2 m{1, 0, 0, 1};
    EXPECT_EQ(nth_power(m, 5), m);

    Matrix2x2 m1{1, 1, 1, 0};
    Matrix2x2 m1_exp{13, 8, 8, 5};
    EXPECT_EQ(nth_power(m1, 6), m1_exp);
}
