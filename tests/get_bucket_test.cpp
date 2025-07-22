//
// Created by mshamrai on 7/22/25.
//


#include <gtest/gtest.h>

#include "BucketPolicy.h"
TEST(GetBucketTest, testOfGetBucket_underflow_returns_minus_one) {
    struct TestElem {
        double x;
        double y;
    };

    auto l1 = [](TestElem const& i){return i.x;};
    auto l2 = [](TestElem const& i){return i.y;};

    auto v1 = std::vector<double>{1.0, 2.0, 3.0, 4.0, 5.0, 6.0};
    auto v2 = std::vector<double>{0.0, 1.0, 2.0, 3.0};

    auto bp = BucketPolicy(std::make_tuple(l1, l2), {v1, v2});




    auto test = bp.getBucket(TestElem{-1.0, 2.0});
    auto res = -1;
    ASSERT_EQ(res, test);
}

TEST(GetBucketTest, testOfGetBucket_overflow_with_many_dimensions_returns_minus_one) {
    struct TestElem {
        double x;
        double y;
        double z;
        double w;
        double v;
    };

    auto l1 = [](TestElem const& i){return i.x;};
    auto l2 = [](TestElem const& i){return i.y;};
    auto l3 = [](TestElem const& i){return i.z;};
    auto l4 = [](TestElem const& i){return i.w;};
    auto l5 = [](TestElem const& i){return i.v;};

    auto v1 = std::vector<double>{-3.0, -2.0, -1.0, 0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0};
    auto v2 = std::vector<double>{0.25, 1.0, 2.222222, 3.2223};
    auto v3 = std::vector<double>{-0.7, -0.6, -0.5, 3.0, 4.0, 10.0};
    auto v4 = std::vector<double>{0.0, 1.0, 2.0, 3.0};
    auto v5 = std::vector<double>{0.0, 1.0, 2.0, 3.0};
    auto bp = BucketPolicy(std::make_tuple(l1, l2, l3, l4, l5), {v1, v2, v3, v4, v5});

    auto test = bp.getBucket(TestElem{-0.5, 0.44, -0.66, 2.45, 10.0});
    auto res = -1;
    ASSERT_EQ(res, test);
}