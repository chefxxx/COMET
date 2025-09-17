//
// Created by Mateusz Mikiciuk on 17/09/2025.
//

#include "BlockCombinations.h"
#include <gtest/gtest.h>

class BlockCombinationsTest : public ::testing::Test
{
    protected:
    const std::vector<double> buckets{0.0, 0.5, 1.0};

    struct testCallable {
        auto operator()(double const& a) const { return a; }
    };
    testCallable callable;

    std::vector<double> v1{0.0, 0.25, 0.5, 0.75, 1.0};
    std::vector<double> v2{0.15, 0.30, 0.45, 0.6, 0.75, 0.90};
};

TEST_F(BlockCombinationsTest, doesGroupDataGroups)
{
    const auto bp = BucketPolicy(std::make_tuple(callable), {buckets});
    Ranges r1{v1.begin(), v1.end()};
    Ranges r2{v2.begin(), v2.end()};
    auto tuple        = std::make_tuple(r1, r2);
    const auto cp     = FullCombinationsPolicy{tuple};
    const auto blockP = BlockCombinations{bp, cp, tuple};
}
