//
// Created by mshamrai on 7/25/25.
//

#include <gtest/gtest.h>
#include "BucketPolicy.h"

namespace test
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

}  // namespace getUpperIndicesForTupleTest

TEST(GetUpperIndicesForTupleTest, fiveRangesInEachDimensionWithoutOverflows)
{
    auto bin1     = std::vector{1.0, 2.0, 3.0, 4.0};
    auto bin2     = std::vector{0.0, 1.0, 2.0, 3.0};
    const auto bp = BucketPolicy(true, test::lambda1, test::lambda2, bin1, bin2);
    const auto arg  = std::make_tuple(1.5, 2.5);
    const auto res  = std::make_tuple(1, 3);
    const auto test = bp.getUpperIndicesForTuple(arg);
    ASSERT_EQ(res, test);
}

TEST(GetUpperIndicesForTupleTest, fiveRangesInEachDimensionWithOverflows)
{
    auto bin1 = std::vector{1.0, 2.0, 3.0, 4.0};
    auto bin2 = std::vector{0.0, 1.0, 2.0, 3.0};
    const auto bp = BucketPolicy(true, test::lambda1, test::lambda2, bin1, bin2);
    const auto arg = std::make_tuple(1.5, 3.5);
    const auto res  = std::make_tuple(1, -1);
    const auto test = bp.getUpperIndicesForTuple(arg);
    ASSERT_EQ(res, test);
}

TEST(GetUpperIndicesForTupleTest, sevenAndFiveRangesWithOverflows)
{
    auto bin1 = std::vector{1.0, 2.0, 3.0, 4.0, 5.0, 6.0};
    auto bin2 = std::vector{0.0, 1.0, 2.0, 3.0};
    const auto bp   = BucketPolicy(true, test::lambda1, test::lambda2, bin1, bin2);
    const auto arg  = std::make_tuple(5.5, -0.5);
    const auto res = std::make_tuple(5, -1);
    const auto test = bp.getUpperIndicesForTuple(arg);
    ASSERT_EQ(res, test);
}

TEST(GetUpperIndicesForTupleTest, manyDimensionsWithOverflows)
{
    auto bin1 = std::vector{-3.0, -2.0, -1.0, 0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0};
    auto bin2 = std::vector{0.25, 1.0, 2.222222, 3.2223};
    auto bin3 = std::vector{-0.7, -0.6, -0.5, 3.0, 4.0, 10.0};
    auto bin4 = std::vector{0.0, 1.0, 2.0, 3.0};
    auto bin5 = std::vector{0.0, 1.0, 2.0, 3.0};
    const auto bp = BucketPolicy(true, test::lambda1, test::lambda2, test::lambda3, test::lambda4, test::lambda5, bin1, bin2, bin3, bin4, bin5);
    const auto arg = std::make_tuple(-0.5, 0.44, -0.66, 2.45, 10.0);
    const auto test = bp.getUpperIndicesForTuple(arg);
    const auto res  = std::make_tuple(3, 1, 1, 3, -1);
    ASSERT_EQ(res, test);
}
