//
// Created by Mateusz Mikiciuk on 18/07/2025.
//

#include <array>
#include <vector>
#include <gtest/gtest.h>
#include "BucketPolicy.h"


TEST(LambdasTest, test1) {
    struct MyPoint {
        int x;
        std::string y;
    };

    auto lambda1 = [](MyPoint a){ return 1.0; };
    auto lambda2 = [](MyPoint b){ return 3.0; };

    std::vector<double> vec1{0.1, 0.2, 0.3, 0.4};
    std::vector<double> vec2{0.5, 0.6, 0.7, 0.8};
    std::vector<double> vec3{1.5, 1.6, 1.7, 1.8};

    auto callables = std::make_tuple(lambda1, lambda2);

    auto bp = BucketPolicy(callables, {vec1, vec2});

    MyPoint p1{1,"aaa"};
    auto res = bp.getBucket(p1);
}