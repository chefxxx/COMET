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

TEST(BinarySearchTest, emptyVector)
{
    std::vector<double> v;

    double value = 1.0;

    int res  = 0;
    int test = findIndex(v, value, false);
    ASSERT_EQ(res, test);
}

TEST(BinarySearchTest, testCase0_4)
{
    std::vector<double> v{0.0, 1.0, 2.0, 3.0, 4.0};

    double value = 3.6;
    int res      = 4;
    int test     = findIndex(v, value, true);

    ASSERT_EQ(res, test);
}

TEST(BinarySearchTest, testCaseUnderflow_0)
{
    std::vector<double> v{-1.0, 0.0, 1.0, 2.0, 3.0, 4.0};

    double value = -313.0;
    int res      = -1;
    int test     = findIndex(v, value, true);

    ASSERT_EQ(res, test);
}

struct TestElem {
    double x;
    double y;
};

TEST(BinarySearchTest, testOfGetBucket_ReturnsAtupleOfIndices)
{
    auto l1 = [](TestElem const& i) {
        return i.x;
    };
    auto l2 = [](TestElem const& i) {
        return i.y;
    };

    auto v1 = std::vector<double>{1.0, 2.0, 3.0, 4.0};
    auto v2 = std::vector<double>{0.0, 1.0, 2.0, 3.0};

    auto bp = BucketPolicy(std::make_tuple(l1, l2), {v1, v2});

    auto test = bp.getUpperIndicesForTuple(std::make_tuple(1.5, 2.5));
    auto res  = std::make_tuple(1, 3);
    ASSERT_EQ(res, test);
}

TEST(BinarySearchTest, testOfGetUpperIndices_underflow_returns_minus_one)
{
    auto l1 = [](TestElem const& i) {
        return i.x;
    };
    auto l2 = [](TestElem const& i) {
        return i.y;
    };

    auto v1 = std::vector<double>{1.0, 2.0, 3.0, 4.0, 5.0, 6.0};
    auto v2 = std::vector<double>{0.0, 1.0, 2.0, 3.0};

    auto bp = BucketPolicy(std::make_tuple(l1, l2), {v1, v2});

    auto test = bp.getUpperIndicesForTuple(std::make_tuple(5.5, -0.5));
    auto res  = std::make_tuple(5, -1);
    ASSERT_EQ(res, test);
}

TEST(BinarySearchTest, testOfGetUpperIndices_overflow_with_many_dimensions)
{
    struct TestElem {
        double x;
        double y;
        double z;
        double w;
        double v;
    };

    auto l1 = [](TestElem const& i) {
        return i.x;
    };
    auto l2 = [](TestElem const& i) {
        return i.y;
    };
    auto l3 = [](TestElem const& i) {
        return i.z;
    };
    auto l4 = [](TestElem const& i) {
        return i.w;
    };
    auto l5 = [](TestElem const& i) {
        return i.v;
    };

    auto v1 = std::vector<double>{-3.0, -2.0, -1.0, 0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0};
    auto v2 = std::vector<double>{0.25, 1.0, 2.222222, 3.2223};
    auto v3 = std::vector<double>{-0.7, -0.6, -0.5, 3.0, 4.0, 10.0};
    auto v4 = std::vector<double>{0.0, 1.0, 2.0, 3.0};
    auto v5 = std::vector<double>{0.0, 1.0, 2.0, 3.0};
    auto bp = BucketPolicy(std::make_tuple(l1, l2, l3, l4, l5), {v1, v2, v3, v4, v5});

    auto test = bp.getUpperIndicesForTuple(std::make_tuple(-0.5, 0.44, -0.66, 2.45, 10.0));
    auto res  = std::make_tuple(3, 1, 1, 3, -1);
    ASSERT_EQ(res, test);
}
