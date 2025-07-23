//
// Created by mshamrai on 7/22/25.
//

#include <gtest/gtest.h>
#include <array>
#include <vector>
#include "BucketPolicy.h"

TEST(GetBucketAtTest, test)
{
    struct TestElem {
        double x;
        double y;
    };

    auto l1 = [](TestElem const& i) {
        return i.x;
    };
    auto l2 = [](TestElem const& i) {
        return i.y;
    };

    auto v1 = std::vector<double>{1.0, 2.0, 3.0, 4.0};
    auto v2 = std::vector<double>{0.0, 1.0, 2.0, 3.0};

    auto bp = BucketPolicy(std::make_tuple(l1, l2), {v1, v2});

    auto test = bp.calculateBucketAtIndices(bp.getUpperIndicesForTuple(std::make_tuple(1.5, 2.5)));
    auto res  = 16;
    ASSERT_EQ(res, test);
}
