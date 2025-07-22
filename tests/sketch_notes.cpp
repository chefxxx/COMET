//
// Created by Mateusz Mikiciuk on 18/07/2025.
//

#include <array>
#include <vector>
#include <gtest/gtest.h>
#include "BucketPolicy.h"
#include "Helpers.h"

namespace testStuff {
    struct MyPoint {
        double x;
        double y;
    };

    std::ostream& operator<<(std::ostream& os, const MyPoint& p) {
        os << "(" << p.x << ", " << p.y << ")";
        return os;
    }

    auto lambda1 = [](const MyPoint& a) { return a.x; };
    auto lambda2 = [](const MyPoint& b) { return b.y; };

    const auto callables = std::make_tuple(lambda1, lambda2);

    const std::vector vec1{0.0, 0.5, 1.0};
    const std::vector vec2{0.0, 0.5, 1.0};

    MyPoint p1{0.1, -0.1};
    MyPoint p2{-0.1, 0.1};
    MyPoint p3{0.6, 0.6};
    MyPoint p4{0.1, 0.6};

    const std::vector points{p1, p2, p3, p4};
}

TEST(LambdasTest, doesBpCompile) {
    auto bp = BucketPolicy(testStuff::callables, {testStuff::vec1, testStuff::vec2});
}

TEST(GroupTableTest, doesGroupDataGroups) {
    const auto bp = BucketPolicy(testStuff::callables, {testStuff::vec1, testStuff::vec2});
    auto bucketCount = bp.getInitialBucketCount();
    std::cout << "Initial bucket count: " << bucketCount << std::endl;
    const auto res = groupData(testStuff::points.begin(), testStuff::points.end(), bp);

    for (const auto& p : res) {
        std::cout << "Bucket " << p.first << ":\n";
        for (const auto& v : p.second) {
            std::cout << *v.mIter << "\n";
        }
    }
}