#include "SIUnits.hpp"

#include <gtest/gtest.h>

struct XClass;
template <typename Rep, typename Period = std::ratio<1>>
using X = SI::Units<Rep, Period, XClass>;

TEST(UnitsTest, ConstructFromRep2)
{
    EXPECT_EQ(X<int>{10}.Count(), 10);
    EXPECT_FLOAT_EQ(X<float>{9}.Count(), 9.0);
    EXPECT_FLOAT_EQ(X<float>{-2.1}.Count(), -2.1);
}

TEST(UnitsTest, ConstructFromOther)
{
    X<int> f{1999};
    X<int> g{f};
    EXPECT_EQ(g.Count(), 1999);

    X<float> h{f};
    EXPECT_FLOAT_EQ(h.Count(), 1999.0);

    X<double> i{h};
    EXPECT_FLOAT_EQ(i.Count(), 1999.0);
}

TEST(UnitsTest, AssigmentOperator)
{
    X<int> f{1};
    X<int> g{5};
    g = f;
    EXPECT_EQ(g.Count(), 1);
}

TEST(UnitsTest, PlusOperator)
{
    X<int> f{1000};
    EXPECT_EQ((+f).Count(), f.Count());
}

TEST(UnitsTest, MinusOperator)
{
    X<int> f{1000};
    EXPECT_EQ((-f).Count(), -f.Count());
}

TEST(UnitsTest, IncrementOperator)
{
    X<int> f{999};
    EXPECT_EQ((++f).Count(), 1000);
    EXPECT_EQ((f++).Count(), 1000);
    EXPECT_EQ(f.Count(), 1001);
}

TEST(UnitsTest, DecrementOperator)
{
    X<int> f{0};
    EXPECT_EQ((--f).Count(), -1);
    EXPECT_EQ((f--).Count(), -1);
    EXPECT_EQ(f.Count(), -2);
}

TEST(UnitsTest, AssigmentPlusOperator)
{
    X<int> f{1};
    X<int> g{5};
    g += f;
    EXPECT_EQ(g.Count(), 6);
}

TEST(UnitsTest, AssigmentMinusOperator)
{
    X<int> f{1};
    X<int> g{5};
    f -= g;
    EXPECT_EQ(f.Count(), -4);
}

TEST(UnitsTest, AssigmentMultiplyOperator)
{
    X<float> f{0.1};
    f *= 7;
    EXPECT_FLOAT_EQ(f.Count(), 0.7);
    f *= 0.5;
    EXPECT_FLOAT_EQ(f.Count(), 0.35);
}

TEST(UnitsTest, AssigmentDivideOperator)
{
    X<int> f{49};
    f /= 7.0;
    EXPECT_EQ(f.Count(), 7);
    f /= 10;
    EXPECT_EQ(f.Count(), 0);
}

TEST(UnitsTest, AssignmentModuloOperator)
{
    X<int> f{33};
    f %= 32;
    EXPECT_EQ(f.Count(), 1);
}

TEST(UnitsTest, BinaryPlusOperator)
{
    X<float, std::kilo> f{3};
    X<int> g{-700};
    X<float, std::kilo> res = f + g;
    EXPECT_FLOAT_EQ(res.Count(), 2.3);
}

TEST(UnitsTest, BinaryMinusOperator)
{
    X<int> f{1000000};
    X<float, std::mega> g{1};
    X<float, std::mega> res = f - g;
    EXPECT_FLOAT_EQ(res.Count(), 0.);
}

TEST(UnitsTest, BinaryMultiplyOperator)
{
    X<float, std::kilo> f{5};
    X<float, std::kilo> res = f * 2;
    EXPECT_FLOAT_EQ(res.Count(), 10.);
    res = 0.1 * f;
    EXPECT_FLOAT_EQ(res.Count(), 0.5);
}

TEST(UnitsTest, BinaryDivideOperator)
{
    X<int, std::kilo> f{25};
    X<int, std::kilo> res = f / 5;
    EXPECT_EQ(res.Count(), 5);

    X<int> g{25};
    EXPECT_FLOAT_EQ(f / g, 1000);
}

TEST(UnitsTest, BinaryModuloOperator)
{
    X<int, std::kilo> f{42};
    X<int, std::kilo> g{40};
    EXPECT_EQ((f % g).Count(), 2);
    EXPECT_EQ((f % 40).Count(), 2);
}

TEST(UnitsTest, ComparisonOperator)
{
    X<float, std::kilo> f{15};
    X<float> g{15000};
    EXPECT_EQ(f, g);
}

TEST(UnitsTest, LessOperator)
{
    X<float, std::kilo> f{15};
    X<int> g{20000};
    EXPECT_LE(f, g);
}

TEST(UnitsTest, NotEqualOperator)
{
    X<int, std::kilo> f{1};
    X<float> g{500};
    EXPECT_NE(f, g);
}

TEST(UnitsTest, LessEqualOperator)
{
    X<int, std::kilo> f{5};
    X<float> g{5000};
    EXPECT_LE(f, g);
    X<float> h{5001};
    EXPECT_LE(f, h);
}

TEST(UnitsTest, GreaterOperator)
{
    X<int> f{115000};
    X<float, std::kilo> g{125};
    EXPECT_GT(g, f);
}

TEST(UnitsTest, GreaterEqualOperator)
{
    X<int, std::kilo> f{300};
    X<float> g{300000};
    X<float, std::mega> h{0.1};
    EXPECT_GE(f, g);
    EXPECT_GE(f, h);
}
