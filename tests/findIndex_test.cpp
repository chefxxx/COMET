//
// Created by Mykhailo Shamrai on 18/07/2025.
//

#include <gtest/gtest.h>
#include "BucketPolicy.h"

TEST(BinarySearchTest, oneElementVector_indexOne)
{
    constexpr std::array v{0.0};
    constexpr double value = 1.0;
    constexpr int res      = -1;
    const int test         = findIndex(v, value, true);
    ASSERT_EQ(res, test);
}

TEST(FindIndexTest, emptyVectorWithoutOverflow)
{
    constexpr std::array<double, 0> v{};
    constexpr double value = 1.0;
    constexpr int res      = 0;
    const int test         = findIndex(v, value, false);
    ASSERT_EQ(res, test);
}

TEST(FindIndexTest, emptyVectorWithOverflows)
{
    constexpr std::array<double, 0> v{};
    constexpr double value = 1.0;
    constexpr int res      = -1;
    const int test         = findIndex(v, value, true);
    ASSERT_EQ(res, test);
}

TEST(FindIndexTest, sixRangesWithoutOverflows)
{
    constexpr std::array v{0.0, 1.0, 2.0, 3.0, 4.0};
    constexpr double value = 3.6;

    constexpr int res = 4;
    const int test    = findIndex(v, value, false);

    ASSERT_EQ(res, test);
}

TEST(FindIndexTest, fourRangesWithOverflows)
{
    constexpr std::array v{0.0, 1.0, 2.0, 3.0, 4.0};
    constexpr double value = -0.00001;

    constexpr int res = -1;
    const int test    = findIndex(v, value, true);

    ASSERT_EQ(res, test);
}

TEST(FindIndexTest, fourRangesWithOverflowsVectorOfDoubleAndFloatArg)
{
    constexpr std::array v{0.0, 1.0, 2.0, 3.0, 4.0};
    constexpr float value = -0.00001f;

    constexpr int res = -1;
    const int test    = findIndex(v, value, true);

    ASSERT_EQ(res, test);
}

TEST(FindIndexTest, fourRangesWithOverflowsVectorOfIntsAndlongArg)
{
    constexpr std::array v{0, 2, 4, 6, 8};
    constexpr long value = 1;

    constexpr int res = 1;
    const int test    = findIndex(v, value, true);

    ASSERT_EQ(res, test);
}
