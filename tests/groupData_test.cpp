//
// Created by Mateusz Mikiciuk on 18/07/2025.
//

#include <gtest/gtest.h>
#include <array>
#include <vector>
#include "BucketPolicy.h"
#include "Helpers.h"

namespace groupDataTest
{
struct MyPoint {
    double x;
    double y;
};

/* lambdas defined for testing */
auto lambda1 = [](const MyPoint &a) {
    return a.x;
};
auto lambda2 = [](const MyPoint &b) {
    return b.y;
};

const auto callables = std::make_tuple(lambda1, lambda2);

/* vectors of bins */
const std::vector vec1{0.0, 0.5, 1.0};
const std::vector vec2{0.0, 0.5, 1.0};

const std::vector vec3{0.0, 0.5, 1.0, 1.5, 2.0};
const std::vector vec4{0.0, 0.5, 1.0, 1.5, 2.0};

/* Elements that are subject of binning */
MyPoint p1{-0.1, 0.1};
MyPoint p2{-0.1, -0.1};
MyPoint p3{-0.1, 0.6};
MyPoint p4{-0.1, 1.1};
MyPoint p5{0.1, 0.1};
MyPoint p6{0.1, -0.1};
MyPoint p7{0.1, 0.6};
MyPoint p8{0.1, 1.1};
MyPoint p9{0.6, 0.1};
MyPoint p10{0.6, -0.1};
MyPoint p11{0.6, 0.6};
MyPoint p12{0.6, 1.1};
MyPoint p13{1.6, 0.1};
MyPoint p14{1.6, -0.1};
MyPoint p15{1.6, 0.6};
MyPoint p16{1.6, 1.1};

const std::vector points{p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16};

}  // namespace testStuff


TEST(GroupDataTest, initialBucketCount)
{
    const auto bp16 = BucketPolicy(groupDataTest::callables, {groupDataTest::vec1, groupDataTest::vec2}, false);
    ASSERT_EQ(bp16.getInitialBucketCount(), 16);
    const auto bp36 = BucketPolicy(groupDataTest::callables, {groupDataTest::vec3, groupDataTest::vec4}, false);
    ASSERT_EQ(bp36.getInitialBucketCount(), 36);
    const auto bp24 = BucketPolicy(groupDataTest::callables, {groupDataTest::vec1, groupDataTest::vec3}, false);
    ASSERT_EQ(bp24.getInitialBucketCount(), 24);
}
