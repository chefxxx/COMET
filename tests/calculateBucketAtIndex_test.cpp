//
// Created by mshamrai on 7/22/25.
//

#include <gtest/gtest.h>
#include <array>
#include <vector>
#include "BucketPolicy.h"

namespace calculateBucketAtIndicesTest
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
}  // namespace calculateBucketAtIndicesTest

TEST(CalculateBucketAtIndicesTest, fiveRangesInTwoDimensionsWithout)
{
    auto bin1 = std::vector<double>{1.0, 2.0, 3.0, 4.0};
    auto bin2 = std::vector<double>{0.0, 1.0, 2.0, 3.0};
    auto arg  = std::make_tuple(1.5, 2.5);
    auto bp   = BucketPolicy(calculateBucketAtIndicesTest::callableShort, {bin1, bin2});

    auto test = bp.calculateBucketAtIndices(bp.getUpperIndicesForTuple(arg));
    auto res  = 16;

    ASSERT_EQ(res, test);
}
