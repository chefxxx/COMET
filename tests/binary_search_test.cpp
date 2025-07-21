//
// Created by Mykhailo Shamrai on 18/07/2025.
//

#include <gtest/gtest.h>
#include <Helpers.h>

TEST(BinarySearchTest, oneElementVector_indexOne)
{
    std::vector<double> v{0.0};

    double value = 1.0;

    int res = 1;
    int test = findUpperIndex(v, value);
    ASSERT_EQ(res, test);
}

TEST(BinarySearchTest, emptyVector) {
    std::vector<double> v;

    double value = 1.0;

    int res = -1;
    int test = findUpperIndex(v, value);
    ASSERT_EQ(res, test);
}

TEST(BinarySearchTest, testCase0_4) {
    std::vector<double> v{0.0, 1.0, 2.0, 3.0, 4.0};

    double value = 3.6;
    int res = 4;
    int test = findUpperIndex(v, value);

    ASSERT_EQ(res, test);
}

TEST(BinarySearchTest, testCaseUnderflow_0) {
    std::vector<double> v{-1.0, 0.0, 1.0, 2.0, 3.0, 4.0};

    double value = -313.0;
    int res = 0;
    int test = findUpperIndex(v, value);

    ASSERT_EQ(res, test);
}

struct TestElem {
    double x;
    double y;
};


TEST(BinarySearchTest, testOfGetBucket_ReturnsAtupleOfIndices) {
    auto l1 = [](TestElem const& i){return i.x;};
    auto l2 = [](TestElem const& i){return i.y;};

    auto v1 = std::vector<double>{1.0, 2.0, 3.0, 4.0};
    auto v2 = std::vector<double>{0.0, 1.0, 2.0, 3.0};

    auto bp = BucketPolicy(std::make_tuple(l1, l2), {v1, v2});


    auto test = bp.getBucket(TestElem{1.5, 2.5});
    auto res = std::make_tuple(1, 3);
    ASSERT_EQ(res, test);
}