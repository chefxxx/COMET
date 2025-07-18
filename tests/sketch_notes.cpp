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

    auto lambda1 = [](const MyPoint& a) { return a.x; };
    auto lambda2 = [](const MyPoint& b) { return b.y; };

    const auto callables = std::make_tuple(lambda1, lambda2);

    const std::vector<double> vec1{0.1, 0.2, 0.3, 0.4};
    const std::vector<double> vec2{0.5, 0.6, 0.7, 0.8};

    MyPoint p1{0.05, 0.1};
    MyPoint p2{0.51, 0.2};
    MyPoint p3{0.61, 0.3};
    MyPoint p4{0.75, 0.4};
    const std::vector<MyPoint> points{p1, p2, p3, p4};
}

TEST(LambdasTest, doesBpCompile) {
    auto bp = BucketPolicy(testStuff::callables, {testStuff::vec1, testStuff::vec2});
}

TEST(GroupTableTest, doesGroupDataGroups) {
    const auto bp = BucketPolicy(testStuff::callables, {testStuff::vec1, testStuff::vec2});
    auto res = groupData(testStuff::points.begin(), testStuff::points.end(), bp);
}