//
// Created by Mateusz Mikiciuk on 18/07/2025.
//

#include <gtest/gtest.h>
#include <array>
#include <vector>
#include "BucketPolicy.h"
#include "Helpers.h"

namespace testStuff
{
struct MyPoint {
    double x;
    double y;
};

std::ostream &operator<<(std::ostream &os, const MyPoint &p)
{
    os << "(" << p.x << ", " << p.y << ")";
    return os;
}

auto lambda1 = [](const MyPoint &a) {
    return a.x;
};
auto lambda2 = [](const MyPoint &b) {
    return b.y;
};

const auto callables = std::make_tuple(lambda1, lambda2);

const std::vector vec1{0.0, 0.5, 1.0};
const std::vector vec2{0.0, 0.5, 1.0};

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

TEST(LambdasTest, doesBpCompile)
{
    auto bp = BucketPolicy(testStuff::callables, {testStuff::vec1, testStuff::vec2});
}

TEST(GroupTableTest, doesGroupDataGroups)
{
    const auto bp = BucketPolicy(testStuff::callables, {testStuff::vec1, testStuff::vec2}, false);
    const auto bucketCount = bp.getInitialBucketCount();
    ASSERT_EQ(bucketCount, 16);
    std::cout << "Initial bucket count: " << bucketCount << std::endl;
    const auto res = groupData(testStuff::points.begin(), testStuff::points.end(), bp);
    ASSERT_EQ(res.size(), 16);
    for (const auto &p : res) {
        std::cout << "{ bucket: " << p.mBucketIdx << ", data: " << p.mDataIdx << ", " << *p.mIter
                  << "} \n";
    }
}
