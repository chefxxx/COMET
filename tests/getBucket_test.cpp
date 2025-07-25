//
// Created by mshamrai on 7/22/25.
//

#include <gtest/gtest.h>

#include "BucketPolicy.h"

namespace GetBucketTest
{
struct TestElem {
    double x;
    double y;
};

struct TestElemB {
    double x;
    double y;
    double z;
    double w;
    double v;
};

auto lambda1 = [](auto const &arg) {
    return arg.x;
};
auto lambda2 = [](auto const &arg) {
    return arg.y;
};
auto lambda3 = [](auto const &arg) {
    return arg.z;
};
auto lambda4 = [](auto const &arg) {
    return arg.w;
};
auto lambda5 = [](auto const &arg) {
    return arg.v;
};

const auto callableShort = std::make_tuple(lambda1, lambda2);
const auto callablesLong = std::make_tuple(lambda1, lambda2, lambda3, lambda4, lambda5);
}  // namespace GetBucketTest

TEST(GetBucketTest, sevenAndFiveRngesWithOverflow)
{
    const auto bin1 = std::vector<double>{1.0, 2.0, 3.0, 4.0, 5.0, 6.0};
    const auto bin2 = std::vector<double>{0.0, 1.0, 2.0, 3.0};
    auto bp         = BucketPolicy(GetBucketTest::callableShort, {bin1, bin2});
    auto arg        = GetBucketTest::TestElem{-1.0, 2.0};

    const auto test    = bp.getBucket(&arg);
    constexpr auto res = -1;
    ASSERT_EQ(res, test);
}

TEST(GetBucketTest, manyDimensionsWithOverflow)
{
    const auto bin1 = std::vector<double>{-3.0, -2.0, -1.0, 0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0};
    const auto bin2 = std::vector<double>{0.25, 1.0, 2.222222, 3.2223};
    const auto bin3 = std::vector<double>{-0.7, -0.6, -0.5, 3.0, 4.0, 10.0};
    const auto bin4 = std::vector<double>{0.0, 1.0, 2.0, 3.0};
    const auto bin5 = std::vector<double>{0.0, 1.0, 2.0, 3.0};
    auto arg        = GetBucketTest::TestElemB{-0.5, 0.44, -0.66, 2.45, 10.0};
    auto bp         = BucketPolicy(GetBucketTest::callablesLong, {bin1, bin2, bin3, bin4, bin5});

    const auto test    = bp.getBucket(&arg);
    constexpr auto res = -1;
    ASSERT_EQ(res, test);
}
