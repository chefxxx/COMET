//
// Created by mshamrai on 12/28/25.
//

#include <gtest/gtest.h>

#include "BucketSortHelpers.h"

auto testLambda = [](auto const &arg) {
    return arg.id;
};

struct TestElement {
    int id;
    double val;
};

class BucketSortHelpersTest : public ::testing::Test
{
    protected:
    std::vector<TestElement> data = {
        {100, 1.1},
        { 10, 2.2},
        { 50, 3.3},
        {100, 4.4},
        { 10, 5.5},
        {100, 6.6}
    };
};

TEST_F(BucketSortHelpersTest, GetAvailableBucketsUniqueAndSorted)
{
    auto buckets = getAvailableBuckets<int>(data, testLambda);

    ASSERT_EQ(buckets.size(), 3);
    EXPECT_EQ(buckets[0], 10);
    EXPECT_EQ(buckets[1], 50);
    EXPECT_EQ(buckets[2], 100);
}

TEST_F(BucketSortHelpersTest, GetIndexFromBucketLogic)
{
    std::vector<int> buckets = {10, 50, 100};

    EXPECT_EQ(getIndexFromBucket(buckets, 10), 0);
    EXPECT_EQ(getIndexFromBucket(buckets, 50), 1);
    EXPECT_EQ(getIndexFromBucket(buckets, 100), 2);
    EXPECT_EQ(getIndexFromBucket(buckets, 999), -1);  // Not found
}

TEST_F(BucketSortHelpersTest, GetAmountsIsCorrect)
{
    auto buckets = getAvailableBuckets<int>(data, testLambda);
    auto amounts = getAmountsInsideAvailableBuckets(data, testLambda, buckets);

    ASSERT_EQ(amounts.size(), 3);
    EXPECT_EQ(amounts[0], 2);  // ID 10
    EXPECT_EQ(amounts[1], 1);  // ID 50
    EXPECT_EQ(amounts[2], 3);  // ID 100
}

TEST_F(BucketSortHelpersTest, GetOffsetsFollowsCSR)
{
    std::vector<size_t> amounts = {2, 1, 3};
    auto offsets                = getOffsetsFromAmounts(amounts);

    ASSERT_EQ(offsets.size(), 4);
    EXPECT_EQ(offsets[0], 0);
    EXPECT_EQ(offsets[1], 2);
    EXPECT_EQ(offsets[2], 3);
    EXPECT_EQ(offsets[3], 6);
}

TEST_F(BucketSortHelpersTest, FullPipelineIntegration)
{
    auto buckets = getAvailableBuckets<int>(data, testLambda);
    auto amounts = getAmountsInsideAvailableBuckets(data, testLambda, buckets);
    auto offsets = getOffsetsFromAmounts(amounts);

    EXPECT_EQ(offsets.back(), data.size());

    for (size_t i = 0; i < buckets.size(); ++i) {
        size_t start = offsets[i];
        size_t end   = offsets[i + 1];
        EXPECT_EQ(end - start, amounts[i]);
    }
}
