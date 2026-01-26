//
// Created by mshamrai on 1/11/26.
//
#include <gtest/gtest.h>
#include <array>
#include <tuple>
#include <vector>

#include "include/BucketPolicy.h"
#include "include/Combinations.h"
#include "include/Producers.h"

class BlockStrictlyUpperCombinationsTest : public ::testing::Test
{
    public:
    std::vector<double> buckets{0.0, 0.5, 1.0};

    struct IdentityCallable {
        auto operator()(double const& a) const { return a; }
    };
    [[no_unique_address]] IdentityCallable callable;
};

TEST_F(BlockStrictlyUpperCombinationsTest, SelfCombinationPairs)
{
    std::vector<double> data = {0.1, 0.2, 0.3, 0.6, 0.7, 0.8, 0.9};

    const auto bp = BucketPolicy(true, callable, buckets);

    auto producer = makeBlockCombinations<StrictlyUpperCombinationsPolicy>(bp, 1, data, data);

    std::vector<std::tuple<double, double>> results;
    for (const auto& val : producer) {
        results.push_back(val);
    }

    std::vector<std::tuple<double, double>> expected = {
        {0.1, 0.2},
        {0.1, 0.3},
        {0.2, 0.3}, // Bucket 0
        {0.6, 0.7},
        {0.6, 0.8},
        {0.6, 0.9}, // Bucket 1
        {0.7, 0.8},
        {0.7, 0.9},
        {0.8, 0.9}
    };

    ASSERT_EQ(results.size(), expected.size());
    for (std::size_t i = 0; i < expected.size(); ++i) {
        EXPECT_EQ(results[i], expected[i]);
    }
}

TEST_F(BlockStrictlyUpperCombinationsTest, SelfCombinationTriplets)
{
    std::vector<double> data = {0.1, 0.2, 0.3, 0.4};

    const auto bp = BucketPolicy(false, callable, buckets);
    auto producer = makeBlockCombinations<StrictlyUpperCombinationsPolicy>(bp, 1, data, data, data);

    std::vector<std::tuple<double, double, double>> expected = {
        {0.1, 0.2, 0.3},
        {0.1, 0.2, 0.4},
        {0.1, 0.3, 0.4},
        {0.2, 0.3, 0.4}
    };

    int count = 0;
    for (const auto& [a, b, c] : producer) {
        ASSERT_LT(count, expected.size());
        EXPECT_EQ(a, std::get<0>(expected[count]));
        EXPECT_EQ(b, std::get<1>(expected[count]));
        EXPECT_EQ(c, std::get<2>(expected[count]));
        count++;
    }
    EXPECT_EQ(count, expected.size());
}

TEST_F(BlockStrictlyUpperCombinationsTest, DifferentContainersOfSameSize)
{
    std::vector<double> v1 = {0.1, 0.2, 0.3};
    std::vector<double> v2 = {0.11, 0.21, 0.31};

    const auto bp = BucketPolicy(false, callable, buckets);
    auto producer = makeBlockCombinations<StrictlyUpperCombinationsPolicy>(bp, 1, v1, v2);

    std::vector<std::tuple<double, double>> expected = {
        {0.1, 0.21},
        {0.1, 0.31},
        {0.2, 0.31}
    };

    int count = 0;
    for (const auto& [a, b] : producer) {
        ASSERT_LT(count, expected.size());
        EXPECT_EQ(std::tie(a, b), expected[count]);
        count++;
    }
    EXPECT_EQ(count, expected.size());
}

TEST_F(BlockStrictlyUpperCombinationsTest, MinCategorySizeRespectsBucketCount)
{
    std::vector<double> data = {0.1, 0.2, 0.6, 0.7, 0.8};

    const auto bp = BucketPolicy(false, callable, buckets);

    auto producer = makeBlockCombinations<StrictlyUpperCombinationsPolicy>(bp, 3, data, data);

    std::vector<std::tuple<double, double>> expected = {
        {0.6, 0.7},
        {0.6, 0.8},
        {0.7, 0.8}
    };

    int count = 0;
    for (const auto& [a, b] : producer) {
        ASSERT_LT(count, expected.size());
        EXPECT_EQ(std::tie(a, b), expected[count]);
        count++;
    }
    EXPECT_EQ(count, expected.size());
}

TEST_F(BlockStrictlyUpperCombinationsTest, DifferentContainersDifferentSizes)
{
    std::vector<double> v1 = {0.1, 0.2};
    std::vector<double> v2 = {0.1, 0.2, 0.3, 0.4};

    const auto bp = BucketPolicy(false, callable, buckets);
    auto producer = makeBlockCombinations<StrictlyUpperCombinationsPolicy>(bp, 1, v1, v2);

    std::vector<std::tuple<double, double>> expected = {
        {0.1, 0.2},
        {0.1, 0.3},
        {0.1, 0.4},
        {0.2, 0.3},
        {0.2, 0.4}
    };

    int count = 0;
    for (const auto& [a, b] : producer) {
        ASSERT_LT(count, expected.size());
        EXPECT_EQ(std::tie(a, b), expected[count]);
        count++;
    }
    EXPECT_EQ(count, expected.size());
}

TEST_F(BlockStrictlyUpperCombinationsTest, CombinationsAppearOnlyInAFewBuckets)
{
    std::vector<double> bucketsLocal{0.0, 0.25, 0.5, 0.75, 1.0, 1.25};
    std::vector<double> v1 = {0.11, 0.33, 0.52, 0.53, 0.76, 1.1, 1.2, 1.3};
    std::vector<double> v2 = {0.1, 0.3, 0.521, 0.522, 0.523, 0.77, 1.11, 1.21, 1.221};

    const auto bp = BucketPolicy(false, callable, bucketsLocal);
    auto producer = makeBlockCombinations<StrictlyUpperCombinationsPolicy>(bp, 1, v1, v2);

    std::vector<std::tuple<double, double>> expected = {
        {0.52, 0.522},
        {0.52, 0.523},
        {0.53, 0.523},
        { 1.1,  1.21},
        { 1.1, 1.221},
        { 1.2, 1.221}
    };

    int count = 0;
    for (const auto& [a, b] : producer) {
        ASSERT_LT(count, expected.size());
        EXPECT_EQ(std::tie(a, b), expected[count]);
        count++;
    }
    EXPECT_EQ(count, expected.size());
}
