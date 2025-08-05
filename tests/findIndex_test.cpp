//
// Created by Mykhailo Shamrai on 18/07/2025.
//

#include <Helpers.h>
#include <gtest/gtest.h>

TEST(BinarySearchTest, oneElementVector_indexOne)
{
    std::vector<double> v{0.0};

    double value = 1.0;

    int res  = -1;
    int test = findIndex(v, value, true);
    ASSERT_EQ(res, test);
}

TEST(FindIndexTest, emptyVectorWithoutOverflow)
{
    std::vector<double> v{};
    double value = 1.0;

    int res  = 0;
    int test = findIndex(v, value, false);

    ASSERT_EQ(res, test);
}

TEST(FindIndexTest, emptyVectorWithOverflows)
{
    std::vector<double> v{};
    double value = 1.0;

    int res  = -1;
    int test = findIndex(v, value, true);

    ASSERT_EQ(res, test);
}

TEST(FindIndexTest, sixRangesWithoutOverflows)
{
    std::vector<double> v{0.0, 1.0, 2.0, 3.0, 4.0};
    double value = 3.6;

    int res  = 4;
    int test = findIndex(v, value, false);

    ASSERT_EQ(res, test);
}

TEST(FindIndexTest, fourRangesWithOverflows)
{
    std::vector<double> v{0.0, 1.0, 2.0, 3.0, 4.0};
    double value = -0.00001;

    int res  = -1;
    int test = findIndex(v, value, true);

    ASSERT_EQ(res, test);
}

TEST(FindIndexTest, fourRangesWithOverflowsVectorOfDoubleAndFloatArg)
{
    std::vector<double> v{0.0, 1.0, 2.0, 3.0, 4.0};
    float value = -0.00001;

    int res  = -1;
    int test = findIndex(v, value, true);

    ASSERT_EQ(res, test);
}

TEST(FindIndexTest, fourRangesWithOverflowsVectorOfIntsAndlongArg)
{
    std::vector<int> v{0, 2, 4, 6, 8};
    long value = 1;

    int res  = 1;
    int test = findIndex(v, value, true);

    ASSERT_EQ(res, test);
}
