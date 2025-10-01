//
// Created by Mateusz Mikiciuk on 17/09/2025.
//

#include <gtest/gtest.h>

#include <random>

#include "BlockCombinations.h"

class BlockFullCombinationsTest : public ::testing::Test
{
    using dataType = std::vector<double>;

    protected:
    const dataType buckets{0.0, 0.25, 0.5, 0.75, 1.0};
    const dataType buckets2{0.0, 1.0};
    const dataType bucketsBenchmark{-1.0, 1.0};

    struct testCallable {
        auto operator()(double const& a) const { return a; }
    };
    testCallable callable;

    dataType v1{-0.05, 0.05, 0.15, 0.25, 0.35, 0.45, 0.55, 0.65, 0.75, 0.85, 0.95, 1.05};
    dataType v2{0.15, 0.30, 0.45, 0.6, 0.75, 0.90};
    dataType v3{0.33, 0.44, 0.55, 0.66, 0.77, 0.88, 0.99};

    dataType v4{0.33, 0.66, 0.99};
    dataType v5{0.25, 0.5, 0.75};

    BucketPolicy<testCallable> bp  = BucketPolicy(std::make_tuple(callable), {buckets}, false);
    BucketPolicy<testCallable> bp2 = BucketPolicy(std::make_tuple(callable), {buckets2}, false);
    BucketPolicy<testCallable> benchmarkBp =
        BucketPolicy(std::make_tuple(callable), {bucketsBenchmark}, false);

    Ranges<dataType::iterator> r1{v1.begin(), v1.end()};
    Ranges<dataType::iterator> r2{v2.begin(), v2.end()};
    Ranges<dataType::iterator> r3{v3.begin(), v3.end()};
    Ranges<dataType::iterator> r4{v4.begin(), v4.end()};
    Ranges<dataType::iterator> r5{v5.begin(), v5.end()};

    std::tuple<decltype(r1), decltype(r2), decltype(r3)> tuple = std::make_tuple(r1, r2, r3);
    std::tuple<decltype(r4), decltype(r5)> tuple2              = std::make_tuple(r4, r5);

    std::vector<int> expectedBucketsNums{2, 3, 4};
};

TEST_F(BlockFullCombinationsTest, groupedDataSizesAreCorrect)
{
    auto blockFull = makeBlockCombinations<FullCombinationsPolicy>(bp, tuple);
    auto data0     = std::get<0>(blockFull.data());
    auto data1     = std::get<1>(blockFull.data());
    auto data2     = std::get<2>(blockFull.data());

    ASSERT_EQ(data0.size(), expectedBucketsNums.size());
    ASSERT_EQ(data1.size(), expectedBucketsNums.size());
    ASSERT_EQ(data2.size(), expectedBucketsNums.size());
}

TEST_F(BlockFullCombinationsTest, groupedDataBucketsNumbersAreCorrect)
{
    auto blockFull = makeBlockCombinations<FullCombinationsPolicy>(bp, tuple);
    auto data0     = std::get<0>(blockFull.data());
    auto data1     = std::get<1>(blockFull.data());
    auto data2     = std::get<2>(blockFull.data());

    for (const auto& bn : expectedBucketsNums) {
        ASSERT_EQ(data0.contains(bn), true);
        ASSERT_EQ(data1.contains(bn), true);
        ASSERT_EQ(data2.contains(bn), true);
    }
}

TEST_F(BlockFullCombinationsTest, bucketsContentsAreCorrect)
{
    auto blockFull = makeBlockCombinations<FullCombinationsPolicy>(bp, tuple);
    auto data0     = std::get<0>(blockFull.data());
    auto data1     = std::get<1>(blockFull.data());
    auto data2     = std::get<2>(blockFull.data());

    const std::vector expectedB02{0.25, 0.35, 0.45};
    const std::vector expectedB03{0.55, 0.65};
    const std::vector expectedB04{0.75, 0.85, 0.95};

    const std::vector expectedB12{0.30, 0.45};
    const std::vector expectedB13{0.6};
    const std::vector expectedB14{0.75, 0.90};

    const std::vector expectedB22{0.33, 0.44};
    const std::vector expectedB23{0.55, 0.66};
    const std::vector expectedB24{0.77, 0.88, 0.99};

    std::array<std::array<std::vector<double>, 3>, 3> expectedBuckets{
        {{expectedB02, expectedB12, expectedB22},
         {expectedB03, expectedB13, expectedB23},
         {expectedB04, expectedB14, expectedB24}}
    };

    int k = 0;
    for (const auto& id : expectedBucketsNums) {
        const auto& bucket0 = data0.at(id);
        const auto& bucket1 = data1.at(id);
        const auto& bucket2 = data2.at(id);
        const auto& arr     = expectedBuckets[k++];
        auto exp0           = arr[0];
        auto exp1           = arr[1];
        auto exp2           = arr[2];
        for (size_t i = 0; i < bucket0.size(); ++i) {
            ASSERT_EQ(exp0[i], *bucket0[i]);
        }
        for (size_t i = 0; i < bucket1.size(); ++i) {
            ASSERT_EQ(exp1[i], *bucket1[i]);
        }
        for (size_t i = 0; i < bucket2.size(); ++i) {
            ASSERT_EQ(exp2[i], *bucket2[i]);
        }
    }
}

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
    auto blockFull = makeBlockCombinations<FullCombinationsPolicy>(bp2, tuple2);
    for (auto& [elem0, elem1] : blockFull) {
        auto& expected = arr[k++];
        ASSERT_EQ(**elem0, std::get<0>(expected));
        ASSERT_EQ(**elem1, std::get<1>(expected));
    }
}

TEST_F(BlockFullCombinationsTest, complexIterationElementsAreCorrect)
{
    auto blockFull = makeBlockCombinations<FullCombinationsPolicy>(bp, tuple);
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
    for (auto& [elem0, elem1, elem2] : blockFull) {
        auto& expected            = arr[k++];
        const std::string message = "At iteration " + std::to_string(k) + "\n";
        ASSERT_EQ(**elem0, std::get<0>(expected)) << message;
        ASSERT_EQ(**elem1, std::get<1>(expected)) << message;
        ASSERT_EQ(**elem2, std::get<2>(expected)) << message;
    }
}
#if 0
TEST_F(BlockFullCombinationsTest, benchmark)
{
    double lowerBound = -1;
    double upperBound = 1;
    std::uniform_real_distribution<double> unif(lowerBound, upperBound);
    std::default_random_engine re;
    std::array<double, 10009> arr1 = {};
    std::array<double, 10000> arr2 = {};
    for (int i = 0; i < 10000; ++i) {
        arr1[i] = unif(re);
        arr2[i] = unif(re);
    }
    Ranges<std::array<double, 100>::iterator> ranges1{arr1.begin(), arr1.end()};
    Ranges<std::array<double, 100>::iterator> ranges2{arr2.begin(), arr2.end()};
    auto tupleArg = std::make_tuple(ranges1, ranges2);
    auto blockFull = makeBlockCombinations<FullCombinationsPolicy>(benchmarkBp, tupleArg);
    auto start     = std::chrono::high_resolution_clock::now();
    for (auto& [elem0, elem1] : blockFull) {
        auto el0 = **elem0;
        auto el1 = **elem1;
    }
    auto stop     = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << duration.count() << " microseconds" << std::endl;
}
#endif
