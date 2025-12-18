#include <gtest/gtest.h>
#include <type_traits>
#include "physical_quantities.hh"


using Dimensionless = Unit<0, 0, 0>;
using Meter         = Unit<1, 0, 0>;
using Second        = Unit<0, 0, 1>;
using Kg            = Unit<0, 1, 0>;

using Speed         = Unit<1, 0, -1>;
using Acceleration  = Unit<1, 0, -2>;
using Force         = Unit<1, 1, -2>;
using Energy        = Unit<2, 1, -2>;


TEST(PhysicalQuantitiesTest, ValMultiplication) {
    Value<Meter> length{10.0};
    Value<Dimensionless> factor{2.0};

    auto result = length * factor;
    EXPECT_DOUBLE_EQ(static_cast<double>(result), 20.0);
}

TEST(PhysicalQuantitiesTest, DimAnalysis_SpeedTime) {
    Value<Speed> v{20.0};
    Value<Second> t{5.0};

    auto distance = v * t;
    EXPECT_DOUBLE_EQ(static_cast<double>(distance), 100.0);

    constexpr bool is_meter = std::is_same_v<decltype(distance), Value<Meter>>;
    EXPECT_TRUE(is_meter);
}

TEST(PhysicalQuantitiesTest, DimAnalysis_ForceDistance) {
    Value<Force> f{10.0};
    Value<Meter> d{5.0};

    auto work = f * d;
    EXPECT_DOUBLE_EQ(static_cast<double>(work), 50.0);

    constexpr bool is_energy = std::is_same_v<decltype(work), Value<Energy>>;
    EXPECT_TRUE(is_energy);
}

TEST(PhysicalQuantitiesTest, Newton2ndLaw) {
    Value<Kg> mass{50.0};
    Value<Acceleration> acc{2.0};

    auto force = mass * acc;
    EXPECT_DOUBLE_EQ(static_cast<double>(force), 100.0);

    constexpr bool is_force = std::is_same_v<decltype(force), Value<Force>>;
    EXPECT_TRUE(is_force);
}
