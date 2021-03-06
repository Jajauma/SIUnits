#include "SIUnits.hpp"

#include <gtest/gtest.h>

struct XClass;
template <typename Rep, typename Period = std::ratio<1>>
using X = SI::Units<Rep, Period, XClass>;

TEST(Units, ConstructFromRep2)
{
    EXPECT_EQ(X<int>{10}.count(), 10);
    EXPECT_FLOAT_EQ(X<float>{9}.count(), 9.0);
    EXPECT_FLOAT_EQ(X<float>{-2.1}.count(), -2.1);
}

TEST(Units, ConstructFromOther)
{
    X<int> f{1999};
    X<int> g{f};
    EXPECT_EQ(g.count(), 1999);

    X<float> h{f};
    EXPECT_FLOAT_EQ(h.count(), 1999.0);

    X<double> i{h};
    EXPECT_FLOAT_EQ(i.count(), 1999.0);
}

TEST(Units, AssigmentOperator)
{
    X<int> f{1};
    X<int> g{5};
    g = f;
    EXPECT_EQ(g.count(), 1);
}

TEST(Units, PlusOperator)
{
    X<int> f{1000};
    EXPECT_EQ((+f).count(), f.count());
}

TEST(Units, MinusOperator)
{
    X<int> f{1000};
    EXPECT_EQ((-f).count(), -f.count());
}

TEST(Units, IncrementOperator)
{
    X<int> f{999};
    EXPECT_EQ((++f).count(), 1000);
    EXPECT_EQ((f++).count(), 1000);
    EXPECT_EQ(f.count(), 1001);
}

TEST(Units, DecrementOperator)
{
    X<int> f{0};
    EXPECT_EQ((--f).count(), -1);
    EXPECT_EQ((f--).count(), -1);
    EXPECT_EQ(f.count(), -2);
}

TEST(Units, AssigmentPlusOperator)
{
    X<int> f{1};
    X<int> g{5};
    g += f;
    EXPECT_EQ(g.count(), 6);
}

TEST(Units, AssigmentMinusOperator)
{
    X<int> f{1};
    X<int> g{5};
    f -= g;
    EXPECT_EQ(f.count(), -4);
}

TEST(Units, AssigmentMultiplyOperator)
{
    X<float> f{0.1};
    f *= 7;
    EXPECT_FLOAT_EQ(f.count(), 0.7);
    f *= 0.5;
    EXPECT_FLOAT_EQ(f.count(), 0.35);
}

TEST(Units, AssigmentDivideOperator)
{
    X<int> f{49};
    f /= 7.0;
    EXPECT_EQ(f.count(), 7);
    f /= 10;
    EXPECT_EQ(f.count(), 0);
}

TEST(Units, AssignmentModuloOperator)
{
    X<int> f{33};
    f %= 32;
    EXPECT_EQ(f.count(), 1);
}

TEST(Units, BinaryPlusOperator)
{
    X<float, std::kilo> f{3};
    X<int> g{-700};
    X<float, std::kilo> res = f + g;
    EXPECT_FLOAT_EQ(res.count(), 2.3);
}

TEST(Units, BinaryMinusOperator)
{
    X<int> f{1000000};
    X<float, std::mega> g{1};
    X<float, std::mega> res = f - g;
    EXPECT_FLOAT_EQ(res.count(), 0.);
}

TEST(Units, BinaryMultiplyOperator)
{
    X<float, std::kilo> f{5};
    X<float, std::kilo> res = f * 2;
    EXPECT_FLOAT_EQ(res.count(), 10.);
    res = 0.1 * f;
    EXPECT_FLOAT_EQ(res.count(), 0.5);
}

TEST(Units, BinaryDivideOperator)
{
    X<int, std::kilo> f{25};
    X<int, std::kilo> res = f / 5;
    EXPECT_EQ(res.count(), 5);

    X<int> g{25};
    EXPECT_FLOAT_EQ(f / g, 1000);
}

TEST(Units, BinaryModuloOperator)
{
    X<int, std::kilo> f{42};
    X<int, std::kilo> g{40};
    EXPECT_EQ((f % g).count(), 2);
    EXPECT_EQ((f % 40).count(), 2);
}

TEST(Units, ComparisonOperator)
{
    X<float, std::kilo> f{15};
    X<float> g{15000};
    EXPECT_EQ(f, g);
}

TEST(Units, LessOperator)
{
    X<float, std::kilo> f{15};
    X<int> g{20000};
    EXPECT_LE(f, g);
}

TEST(Units, NotEqualOperator)
{
    X<int, std::kilo> f{1};
    X<float> g{500};
    EXPECT_NE(f, g);
}

TEST(Units, LessEqualOperator)
{
    X<int, std::kilo> f{5};
    X<float> g{5000};
    EXPECT_LE(f, g);
    X<float> h{5001};
    EXPECT_LE(f, h);
}

TEST(Units, GreaterOperator)
{
    X<int> f{115000};
    X<float, std::kilo> g{125};
    EXPECT_GT(g, f);
}

TEST(Units, GreaterEqualOperator)
{
    X<int, std::kilo> f{300};
    X<float> g{300000};
    X<float, std::mega> h{0.1};
    EXPECT_GE(f, g);
    EXPECT_GE(f, h);
}

TEST(Units, ConstexprSwitch)
{
    constexpr X<int> x1{100}, x2{200}, x3{300};

    switch (300)
    {
    case x1.count():
        FAIL();
        break;

    case x2.count():
        FAIL();
        break;

    case x3.count():
        SUCCEED();
        break;

    default:
        FAIL();
        break;
    }
}
