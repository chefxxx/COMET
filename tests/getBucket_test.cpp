//
// Created by mshamrai on 7/22/25.
//

#include <gtest/gtest.h>

#include "BucketPolicy.h"

class GetBucketTest : public ::testing::Test
{
    public:
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

    struct callable1 {
        auto operator()(const auto &arg) const { return arg->x; }
    };
    struct callable2 {
        auto operator()(const auto &arg) const { return arg->y; }
    };
    struct callable3 {
        auto operator()(const auto &arg) const { return arg->z; }
    };
    struct callable4 {
        auto operator()(const auto &arg) const { return arg->w; }
    };
    struct callable5 {
        auto operator()(const auto &arg) const { return arg->v; }
    };
};

TEST_F(GetBucketTest, sevenAndFiveRngesWithOverflow)
{
    constexpr auto bin1 = std::array{1.0, 2.0, 3.0, 4.0, 5.0, 6.0};
    constexpr auto bin2 = std::array{0.0, 1.0, 2.0, 3.0};
    const auto bp       = BucketPolicy(true, callable1{}, callable2{}, bin1, bin2);
    auto arg            = TestElem{-1.0, 2.0};

    const auto test    = bp.getBucket(&arg);
    constexpr auto res = -1;
    ASSERT_EQ(res, test);
}

TEST_F(GetBucketTest, manyDimensionsWithOverflow)
{
    constexpr auto bin1 = std::array{-3.0, -2.0, -1.0, 0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0};
    constexpr auto bin2 = std::array{0.25, 1.0, 2.222222, 3.2223};
    constexpr auto bin3 = std::array{-0.7, -0.6, -0.5, 3.0, 4.0, 10.0};
    constexpr auto bin4 = std::array{0.0, 1.0, 2.0, 3.0};
    constexpr auto bin5 = std::array{0.0, 1.0, 2.0, 3.0};
    const auto bp       = BucketPolicy(
        true, callable1{}, callable2{}, callable3{}, callable4{}, callable5{}, bin1, bin2, bin3,
        bin4, bin5
    );
    auto arg = TestElemB{-0.5, 0.44, -0.66, 2.45, 10.0};

    const auto test    = bp.getBucket(&arg);
    constexpr auto res = -1;
    ASSERT_EQ(res, test);
}
