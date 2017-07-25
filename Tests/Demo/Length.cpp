#include "SIUnits.hpp"

#include <gtest/gtest.h>

using namespace SI::Literals;

TEST(Length, nm)
{
    SI::Length<int, std::nano> f = -99_nm;
    EXPECT_EQ(f.Count(), -99);
    SI::Length<float, std::nano> g = 10e9_nm;
    EXPECT_FLOAT_EQ(g.Count(), 10e9);
}

TEST(Length, um)
{
    SI::Length<int, std::micro> f = 650_um;
    EXPECT_EQ(f.Count(), 650);
    SI::Length<float, std::micro> g = 2.99_um;
    EXPECT_FLOAT_EQ(g.Count(), 2.99);
}

TEST(Length, mm)
{
    SI::Length<int, std::milli> f = 3_mm;
    EXPECT_EQ(f.Count(), 3);
    SI::Length<float, std::milli> g = 0.1_mm;
    EXPECT_FLOAT_EQ(g.Count(), 0.1);
}

TEST(Length, cm)
{
    SI::Length<int, std::centi> f = 5_cm;
    EXPECT_EQ(f.Count(), 5);
    SI::Length<float, std::centi> g = 19_cm;
    EXPECT_FLOAT_EQ(g.Count(), 19);
}

TEST(Length, m)
{
    SI::Length<int> f = 1_m;
    EXPECT_EQ(f.Count(), 1);
    SI::Length<float> g = 1.1_m;
    EXPECT_FLOAT_EQ(g.Count(), 1.1);
}

TEST(Length, km)
{
    SI::Length<int, std::kilo> f = 72_km;
    EXPECT_EQ(f.Count(), 72);
    SI::Length<float, std::kilo> g = 40e3_km;
    EXPECT_FLOAT_EQ(g.Count(), 40000.0);
}
