#include "Demo/Frequency.hpp"

#include <gtest/gtest.h>

using namespace Demo;

TEST(Frequency, UsingCxx11Auto)
{
    auto f = 1_Hz;
    EXPECT_EQ(sizeof(f), sizeof(unsigned long long int));
    EXPECT_EQ(f.count(), 1);
    auto g = 1.0_Hz;
    EXPECT_EQ(sizeof(g), sizeof(long double));
    EXPECT_FLOAT_EQ(g.count(), 1.0);
}

TEST(Frequency, Hz)
{
    Frequency<int> f = 144_Hz;
    EXPECT_EQ(f.count(), 144);
    Frequency<double> g = 300000.0_Hz;
    EXPECT_FLOAT_EQ(g.count(), 300000.0);
}

TEST(Frequency, kHz)
{
    Frequency<int, std::kilo> f = 125_kHz;
    EXPECT_EQ(f.count(), 125);
    Frequency<double, std::kilo> g = 300.0_kHz;
    EXPECT_FLOAT_EQ(g.count(), 300.0);
}

TEST(Frequency, MHz)
{
    Frequency<int, std::mega> f = -1_MHz;
    EXPECT_EQ(f.count(), -1);
    Frequency<double, std::mega> g = 27.5_MHz;
    EXPECT_FLOAT_EQ(g.count(), 27.5);
}

TEST(Frequency, GHz)
{
    Frequency<int, std::giga> f = 0_GHz;
    EXPECT_EQ(f.count(), 0);
    Frequency<double, std::giga> g = 3.3_GHz;
    EXPECT_FLOAT_EQ(g.count(), 3.3);
}

TEST(Frequency, THz)
{
    Frequency<int, std::tera> f = 430_THz;
    EXPECT_EQ(f.count(), 430);
    Frequency<double, std::tera> g = .11_THz;
    EXPECT_FLOAT_EQ(g.count(), 0.11);
}
