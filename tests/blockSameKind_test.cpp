//
// Created by mshamrai on 1/11/26.
//

#include <gtest/gtest.h>
#include <string>
#include <tuple>
#include <vector>

#include "include/BucketPolicy.h"
#include "include/Combinations.h"
#include "include/Producers.h"

class BlockSameTypeCombinationsTest : public ::testing::Test
{
    public:
    std::vector<double> buckets = {0.0, 10.0, 20.0, 30.0};

    struct IdentityCallable {
        auto operator()(int const& a) const { return static_cast<double>(a); }
    };
    [[no_unique_address]] IdentityCallable callable;
};

TEST_F(BlockSameTypeCombinationsTest, StrictlyUpper_WindowRespectsBucketBoundaries)
{
    std::vector<int> data = {1, 2, 3, 11, 12, 13};

    const auto bp = BucketPolicy(false, callable, buckets);

    int window     = 1;
    int minCatSize = 0;

    auto producer = makeSameKindBlockCombinations<SameTypeStrictlyUpperCombinationsPolicy>(
        bp, minCatSize, window, data, data
    );

    std::vector<std::tuple<int, int>> expected = {
        { 1,  2},
        { 2,  3}, // Bucket 0
        {11, 12},
        {12, 13}  // Bucket 1
    };

    int count = 0;
    for (const auto& [a, b] : producer) {
        ASSERT_LT(count, expected.size());
        EXPECT_EQ(std::tie(a, b), expected[count]);
        count++;
    }
    EXPECT_EQ(count, expected.size());
}

TEST_F(BlockSameTypeCombinationsTest, StrictlyUpper_WindowLargerThanBucket)
{
    std::vector<int> data = {1, 2, 3};  // All in Bucket 0

    const auto bp = BucketPolicy(false, callable, buckets);
    int window    = 10;

    auto producer = makeSameKindBlockCombinations<SameTypeStrictlyUpperCombinationsPolicy>(
        bp, 1, window, data, data
    );

    std::vector<std::tuple<int, int>> expected = {
        {1, 2},
        {1, 3},
        {2, 3}
    };

    int count = 0;
    for (const auto& [a, b] : producer) {
        ASSERT_LT(count, expected.size());
        EXPECT_EQ(std::tie(a, b), expected[count]);
        count++;
    }
    EXPECT_EQ(count, expected.size());
}

TEST_F(BlockSameTypeCombinationsTest, StrictlyUpper_Triplets3D)
{
    std::vector<int> data = {1, 2, 3, 4};
    const auto bp         = BucketPolicy(false, callable, buckets);
    int window            = 2;

    auto producer = makeSameKindBlockCombinations<SameTypeStrictlyUpperCombinationsPolicy>(
        bp, 1, window, data, data, data
    );

    std::vector<std::tuple<int, int, int>> expected = {
        {1, 2, 3},
        {2, 3, 4}
    };

    int count = 0;
    for (const auto& [a, b, c] : producer) {
        ASSERT_LT(count, expected.size());
        EXPECT_EQ(std::tie(a, b, c), expected[count]);
        count++;
    }
    EXPECT_EQ(count, expected.size());
}

TEST_F(BlockSameTypeCombinationsTest, Full_WindowDiagonalBand)
{
    std::vector<int> data = {1, 2, 3};
    const auto bp         = BucketPolicy(false, callable, buckets);
    int window            = 1;

    auto producer =
        makeSameKindBlockCombinations<SameTypeFullCombinationsPolicy>(bp, 1, window, data, data);

    std::vector<std::tuple<int, int>> expected = {
        {1, 1},
        {1, 2},
        {2, 2},
        {2, 3},
        {3, 3}
    };

    int count = 0;
    for (const auto& [a, b] : producer) {
        ASSERT_LT(count, expected.size());
        EXPECT_EQ(std::tie(a, b), expected[count]);
        count++;
    }
    EXPECT_EQ(count, expected.size());
}

TEST_F(BlockSameTypeCombinationsTest, Full_MultiBucketSkip)
{
    std::vector<int> data = {1, 11, 12};
    const auto bp         = BucketPolicy(false, callable, buckets);

    auto producer =
        makeSameKindBlockCombinations<SameTypeFullCombinationsPolicy>(bp, 1, 0, data, data);

    std::vector<std::tuple<int, int>> expected = {
        { 1,  1}, // Bucket 0
        {11, 11}, // Bucket 1
        {12, 12}
    };

    int count = 0;
    for (const auto& [a, b] : producer) {
        ASSERT_LT(count, expected.size());
        EXPECT_EQ(std::tie(a, b), expected[count]);
        count++;
    }
    EXPECT_EQ(count, expected.size());
}

TEST_F(BlockSameTypeCombinationsTest, SkipsBucketsSmallerThanMinSize)
{
    std::vector<int> data = {1, 11, 12, 13};
    const auto bp         = BucketPolicy(false, callable, buckets);

    // Require at least 2 elements in a bucket to process it
    int minCatSize = 2;
    int window     = 1;

    auto producer = makeSameKindBlockCombinations<SameTypeStrictlyUpperCombinationsPolicy>(
        bp, minCatSize, window, data, data
    );

    // Bucket 0 is skipped.
    // Bucket 1 processed: (11,12), (12,13)
    std::vector<std::tuple<int, int>> expected = {
        {11, 12},
        {12, 13}
    };

    int count = 0;
    for (const auto& [a, b] : producer) {
        ASSERT_LT(count, expected.size());
        EXPECT_EQ(std::tie(a, b), expected[count]);
        count++;
    }
    EXPECT_EQ(count, expected.size());
}
