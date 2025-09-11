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

    auto operator<=>(const MyPoint &) const = default;
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
MyPoint p2{-0.1, -0.1};  // 0
MyPoint p1{-0.1, 0.1};   // 1
MyPoint p3{-0.1, 0.6};   // 2
MyPoint p4{-0.1, 1.1};   // 3
MyPoint p6{0.1, -0.1};   // 4
MyPoint p5{0.1, 0.1};    // 5 non overflow in any dimension for vec1 and vec2 buckets
MyPoint p7{0.1, 0.6};    // 6 non overflow in any dimension for vec1 and vec2 buckets
MyPoint p8{0.1, 1.1};    // 7
MyPoint p10{0.6, -0.1};  // 8
MyPoint p9{0.6, 0.1};    // 9  non overflow in any dimension for vec1 and vec2 buckets
MyPoint p11{0.6, 0.6};   // 10 non overflow in any dimension for vec1 and vec2 buckets
MyPoint p12{0.6, 1.1};   // 11
MyPoint p14{1.6, -0.1};  // 12
MyPoint p13{1.6, 0.1};   // 13
MyPoint p15{1.6, 0.6};   // 14
MyPoint p16{1.6, 1.1};   // 15

const std::vector points1{p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16};

const auto bp16overflows       = BucketPolicy(callables, {vec1, vec2}, false);
const auto bp16ignoreOverflows = BucketPolicy(callables, {vec1, vec2});
const auto bp36                = BucketPolicy(callables, {vec3, vec4});
const auto bp24                = BucketPolicy(callables, {vec1, vec3});

}  // namespace groupDataTest

TEST(GroupDataTest, initialBucketCount)
{
    ASSERT_EQ(groupDataTest::bp16overflows.getMaximalBucketCount(), 16);
    ASSERT_EQ(groupDataTest::bp16ignoreOverflows.getMaximalBucketCount(), 16);
    ASSERT_EQ(groupDataTest::bp36.getMaximalBucketCount(), 36);
    ASSERT_EQ(groupDataTest::bp24.getMaximalBucketCount(), 24);
}

TEST(GroupDataTest, bucketPartitionWithOverflows)
{
    const auto [buckets, bucketsNumbers] = groupData(
        groupDataTest::points1.begin(), groupDataTest::points1.end(), groupDataTest::bp16overflows
    );
    for (const int &key : bucketsNumbers) {
        auto bucket = buckets.at(key);
        ASSERT_EQ(bucket.size(), 1);
        ASSERT_EQ(bucket[0].mBucketIdx, key);
    }
}

TEST(GroupDataTest, bucketPartitionIgnoreOverflows)
{
    const auto [buckets, bucketsNumbers] = groupData(
        groupDataTest::points1.begin(), groupDataTest::points1.end(),
        groupDataTest::bp16ignoreOverflows
    );
    ASSERT_EQ(bucketsNumbers.size(), 5);
    ASSERT_EQ(buckets.size(), 5);
    const std::vector checkBuckets = {-1, 5, 6, 9, 10};
    for (const int &bucket : checkBuckets) {
        ASSERT_EQ(true, bucketsNumbers.contains(bucket));
    }
}

TEST(GroupDataTest, minCategorySize)
{
    constexpr int minCatSize   = 2;
    const auto indexesMinSize2 = groupData(
        groupDataTest::points1.begin(), groupDataTest::points1.end(), groupDataTest::bp16overflows,
        minCatSize
    );
    ASSERT_EQ(indexesMinSize2.buckets.size(), 0);
    ASSERT_EQ(indexesMinSize2.bucketsNumbers.size(), 0);
}
