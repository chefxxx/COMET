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

    std::vector<double> v1{-0.05, 0.05, 0.25, 0.5, 0.75, 0.95, 1.01};
    std::vector<double> v2{0.15, 0.30, 0.45, 0.6, 0.75, 0.90};
};

TEST_F(BlockCombinationsTest, doesGroupDataGroups)
{
    const auto bp = BucketPolicy(std::make_tuple(callable), {buckets}, false);
    Ranges r1{v1.begin(), v1.end()};
    Ranges r2{v2.begin(), v2.end()};
    auto tuple    = std::make_tuple(r1, r2);
    const auto cp = FullCombinationsPolicy{tuple};
    auto blockP   = BlockCombinations{bp, cp, tuple};

    syncBuckets(blockP.mGroupedData);
    auto [buckets1, bucketsNumbers1] = std::get<0>(blockP.mGroupedData);
    auto [buckets2, bucketsNumbers2] = std::get<1>(blockP.mGroupedData);

    for (const auto& bucketNum : bucketsNumbers1) {
        std::cout << bucketNum << ":\n";
        const auto bucket = buckets1[bucketNum];
        for (const auto& elem : bucket) {
            std::cout << *elem.mIter << " ";
        }
        std::cout << '\n';
    }
    std::cout << '\n';
    for (const auto& bucketNum : bucketsNumbers2) {
        std::cout << bucketNum << " ";
    }
}
