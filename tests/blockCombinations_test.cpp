//
// Created by Mateusz Mikiciuk on 17/09/2025.
//

#include "BlockCombinations.h"
#include <gtest/gtest.h>

class BlockCombinationsTest : public ::testing::Test
{
    protected:
    const std::vector<double> buckets{0.0, 0.25, 0.5, 0.75, 1.0};

    struct testCallable {
        auto operator()(double const& a) const { return a; }
    };
    testCallable callable;

    std::vector<double> v1{-0.05, 0.05, 0.15, 0.25, 0.35, 0.45, 0.55, 0.65, 0.75, 0.85, 0.95, 1.05};
    std::vector<double> v2{0.15, 0.30, 0.45, 0.6, 0.75, 0.90};
    std::vector<double> v3{0.33, 0.44, 0.55, 0.66, 0.77, 0.88, 0.99};

    BucketPolicy<testCallable> bp = BucketPolicy(std::make_tuple(callable), {buckets}, false);

    Ranges<std::vector<double>::iterator> r1{v1.begin(), v1.end()};
    Ranges<std::vector<double>::iterator> r2{v2.begin(), v2.end()};
    Ranges<std::vector<double>::iterator> r3{v3.begin(), v3.end()};

    std::vector<int> expectedBN0{0, 1, 2, 3, 4, 5};
    std::vector<int> expectedBN1{1, 2, 3, 4};
    std::vector<int> expectedBN2{2, 3, 4};

    std::tuple<decltype(r1), decltype(r2), decltype(r3)> tuple = std::make_tuple(r1, r2, r3);
    FullCombinationsPolicy<
        std::vector<double>::iterator, std::vector<double>::iterator, std::vector<double>::iterator>
        cp = FullCombinationsPolicy{tuple};
    BlockCombinations<
        decltype(bp), decltype(cp), std::vector<double>::iterator, std::vector<double>::iterator,
        std::vector<double>::iterator>
        blockP = BlockCombinations{bp, cp, tuple};
};

TEST_F(BlockCombinationsTest, groupDataSizesAreCorrect)
{
    auto data0 = std::get<0>(blockP.mGroupedData);
    auto data1 = std::get<1>(blockP.mGroupedData);
    auto data2 = std::get<2>(blockP.mGroupedData);

    ASSERT_EQ(expectedBN0.size(), data0.size());
    ASSERT_EQ(expectedBN1.size(), data1.size());
    ASSERT_EQ(expectedBN2.size(), data2.size());
}

TEST_F(BlockCombinationsTest, groupDataBucketsAreCorrect)
{
    const auto [buckets0, bucketsNumbers0] = std::get<0>(blockP.mGroupedData);
    const auto [buckets1, bucketsNumbers1] = std::get<1>(blockP.mGroupedData);
    const auto [buckets2, bucketsNumbers2] = std::get<2>(blockP.mGroupedData);

    for (const auto& num : expectedBN0) {
        ASSERT_EQ(true, bucketsNumbers0.contains(num));
    }

    for (const auto& num : expectedBN1) {
        ASSERT_EQ(true, bucketsNumbers1.contains(num));
    }

    for (const auto& num : expectedBN2) {
        ASSERT_EQ(true, bucketsNumbers2.contains(num));
    }
}

TEST_F(BlockCombinationsTest, groupDataElementsInBucketsAreCorrect)
{
    const auto [buckets0, bucketsNumbers0] = std::get<0>(blockP.mGroupedData);

    const std::vector expectedB00{-0.05};
    const std::vector expectedB01{0.05, 0.15};
    const std::vector expectedB02{0.25, 0.35, 0.45};
    const std::vector expectedB03{0.55, 0.65};
    const std::vector expectedB04{0.75, 0.85, 0.95};
    const std::vector expectedB05{1.05};

    const std::array expectedBuckets{expectedB00, expectedB01, expectedB02,
                                     expectedB03, expectedB04, expectedB05};

    for (size_t i = 0; i < buckets0.size(); ++i) {
        auto bucket          = buckets0.at(i);
        const auto& expected = expectedBuckets.at(i);
        ASSERT_EQ(expected.size(), bucket.size());
        for (size_t j = 0; j < bucket.size(); ++j) {
            ASSERT_EQ(expected.at(j), *bucket.at(j));
        }
    }
}

TEST_F(BlockCombinationsTest, basicSyncData)
{
    const std::vector<double> expectedCommon{2, 3, 4};
    syncBuckets(blockP.mGroupedData);
    const auto data = std::get<0>(blockP.mGroupedData);
}
