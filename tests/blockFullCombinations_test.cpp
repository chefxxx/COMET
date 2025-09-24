//
// Created by Mateusz Mikiciuk on 17/09/2025.
//

#include <gtest/gtest.h>
#include "BlockCombinations.h"

class BlockFullCombinationsTest : public ::testing::Test
{
    using dataType = std::vector<double>;

    protected:
    const dataType buckets{0.0, 0.25, 0.5, 0.75, 1.0};
    const dataType buckets2{0.0, 1.0};

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

TEST_F(BlockFullCombinationsTest, simpleIterationSize)
{
    auto blockFull = makeBlockCombinations<FullCombinationsPolicy>(bp2, tuple2);
    int k          = 0;
    for (const auto& combination : blockFull) {
        std::get<0>(combination);
        k++;
    }
    ASSERT_EQ(k, 9);
}

TEST_F(BlockFullCombinationsTest, simpleIterationElementsAreCorrect)
{
    auto blockFull = makeBlockCombinations<FullCombinationsPolicy>(bp2, tuple2);
    for (auto& combination : blockFull) {
        auto& elem0 = std::get<0>(combination);
        auto& elem1 = std::get<1>(combination);
        std::cout << "(" << **elem0 << ", " << **elem1 << ")" << '\n';
    }
}
