//
// Created by Mateusz Mikiciuk on 18/07/2025.
//

#include <gtest/gtest.h>
#include <array>
#include <vector>
#include "BucketPolicy.h"


TEST(LambdasTest, test1) {
    struct MyPoint {
        int x;
        int y;
    };

    auto lambda1 = [](MyPoint a){};
    auto lambda2 = [](MyPoint b){};

    std::vector<double> vec1{0.1, 0.2, 0.3, 0.4};
    std::vector<double> vec2{0.5, 0.6, 0.7, 0.8};

    auto callables = std::make_tuple(lambda1, lambda2);

    auto bp = BucketPolicy(callables, {vec1, vec2});
}