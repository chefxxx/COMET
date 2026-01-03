//
// Created by Mateusz Mikiciuk on 17/09/2025.
//

#include <Combinations.h>
#include <gtest/gtest.h>
#include <numeric>

#include "BucketPolicy.h"
#include "Producers.h"

class BlockFullCombinationsTest : public ::testing::Test
{
    using dataType = std::vector<double>;

    public:
    const dataType buckets{0.0, 0.25, 0.5, 0.75, 1.0};
    const dataType buckets2{0.0, 1.0};
    const dataType bucketsBenchmark{-1.0, 1.0};

    struct testCallable {
        auto operator()(double const &a) const { return a; }
    };
    [[no_unique_address]] testCallable callable;

    dataType v1{-0.05, 0.05, 0.15, 0.25, 0.35, 0.45, 0.55, 0.65, 0.75, 0.85, 0.95, 1.05};
    dataType v2{0.15, 0.30, 0.45, 0.6, 0.75, 0.90};
    dataType v3{0.33, 0.44, 0.55, 0.66, 0.77, 0.88, 0.99};

    dataType v4{0.33, 0.66, 0.99};
    dataType v5{0.25, 0.5, 0.75};

    std::vector<int> expectedBucketsNums{2, 3, 4};
};

TEST_F(BlockFullCombinationsTest, simpleIterationElementsAreCorrect)
{
    const std::array<std::tuple<double, double>, 9> arr = {
        {{0.33, 0.25},
         {0.33, 0.5},
         {0.33, 0.75},
         {0.66, 0.25},
         {0.66, 0.5},
         {0.66, 0.75},
         {0.99, 0.25},
         {0.99, 0.5},
         {0.99, 0.75}}
    };

    int k          = 0;
    const auto bp  = BucketPolicy(false, callable, buckets2);
    auto blockFull = makeBlockCombinations<FullCombinationsPolicy>(bp, 1, v4, v5);
    for (auto [elem0, elem1] : blockFull) {
        auto &[ex0, ex1] = arr[k];
        k++;
        ASSERT_EQ(elem0, ex0);
        ASSERT_EQ(elem1, ex1);
    }
}

TEST_F(BlockFullCombinationsTest, complexIterationElementsAreCorrect)
{
    const auto bp  = BucketPolicy(false, callable, buckets);
    auto blockFull = makeBlockCombinations<FullCombinationsPolicy>(bp, 1, v1, v2, v3);
    const std::array<std::tuple<double, double, double>, 34> arr = {
        {
         {0.25, 0.3, 0.33}, {0.25, 0.3, 0.44}, {0.25, 0.45, 0.33}, {0.25, 0.45, 0.44},
         {0.35, 0.3, 0.33}, {0.35, 0.3, 0.44}, {0.35, 0.45, 0.33}, {0.35, 0.45, 0.44},
         {0.45, 0.3, 0.33}, {0.45, 0.3, 0.44}, {0.45, 0.45, 0.33}, {0.45, 0.45, 0.44},
         {0.55, 0.6, 0.55}, {0.55, 0.6, 0.66}, {0.65, 0.6, 0.55}, {0.65, 0.6, 0.66},
         {0.75, 0.75, 0.77}, {0.75, 0.75, 0.88}, {0.75, 0.75, 0.99}, {0.75, 0.90, 0.77},
         {0.75, 0.90, 0.88}, {0.75, 0.90, 0.99}, {0.85, 0.75, 0.77}, {0.85, 0.75, 0.88},
         {0.85, 0.75, 0.99}, {0.85, 0.90, 0.77}, {0.85, 0.90, 0.88}, {0.85, 0.90, 0.99},
         {0.95, 0.75, 0.77}, {0.95, 0.75, 0.88}, {0.95, 0.75, 0.99}, {0.95, 0.90, 0.77},
         {0.95, 0.90, 0.88}, {0.95, 0.90, 0.99},
         }
    };

    int k = 0;
    for (const auto &[elem0, elem1, elem2] : blockFull) {
        auto &[ex0, ex1, ex2] = arr[k];
        ++k;
        ASSERT_EQ(elem0, ex0);
        ASSERT_EQ(elem1, ex1);
        ASSERT_EQ(elem2, ex2);
    }
}

#if 0

using DoubleMicros = std::chrono::duration<double, std::micro>;

TEST(BlockPerformanceTest, bigVectorsOneBucket)
{
    constexpr size_t SIZE = 1e7;
    std::vector<size_t> v1(SIZE);
    std::vector<size_t> v2(SIZE);
    std::iota(v1.begin(), v1.end(), 0);
    std::iota(v2.begin(), v2.end(), 1);
    auto lambda = [](const int &a) { return a; };
    std::vector<double> buckets(SIZE + 1);
    for (size_t i = 0; i <= SIZE / 10; ++i) {
        buckets[i] = i * 10.0;
    }
    const auto bp = BucketPolicy(std::make_tuple(lambda), {buckets}, false);
    const auto start          = std::chrono::high_resolution_clock::now();
    auto bc  = makeBlockCombinations<FullCombinationsPolicy>(bp, 1, v1, v2);
    const auto stop             = std::chrono::high_resolution_clock::now();
    const DoubleMicros duration = stop - start;
    std::cout << std::format("Time taken {} s\n", duration.count() / 1e6);
}

#endif
