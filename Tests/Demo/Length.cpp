#include "Demo/Length.hpp"

#include <gtest/gtest.h>

using namespace Demo;

TEST(Length, nm)
{
    Length<int, std::nano> f = -99_nm;
    EXPECT_EQ(f.count(), -99);
    Length<float, std::nano> g = 10e9_nm;
    EXPECT_FLOAT_EQ(g.count(), 10e9);
}

TEST(Length, um)
{
    Length<int, std::micro> f = 650_um;
    EXPECT_EQ(f.count(), 650);
    Length<float, std::micro> g = 2.99_um;
    EXPECT_FLOAT_EQ(g.count(), 2.99);
}

TEST(Length, mm)
{
    Length<int, std::milli> f = 3_mm;
    EXPECT_EQ(f.count(), 3);
    Length<float, std::milli> g = 0.1_mm;
    EXPECT_FLOAT_EQ(g.count(), 0.1);
}

TEST(Length, cm)
{
    Length<int, std::centi> f = 5_cm;
    EXPECT_EQ(f.count(), 5);
    Length<float, std::centi> g = 19_cm;
    EXPECT_FLOAT_EQ(g.count(), 19);
}

TEST(Length, m)
{
    Length<int> f = 1_m;
    EXPECT_EQ(f.count(), 1);
    Length<float> g = 1.1_m;
    EXPECT_FLOAT_EQ(g.count(), 1.1);
}

TEST(Length, km)
{
    Length<int, std::kilo> f = 72_km;
    EXPECT_EQ(f.count(), 72);
    Length<float, std::kilo> g = 40e3_km;
    EXPECT_FLOAT_EQ(g.count(), 40000.0);
}
