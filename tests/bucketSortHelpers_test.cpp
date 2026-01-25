//
// Created by mshamrai on 12/28/25.
//

#include <gtest/gtest.h>

#include <list>

#include "BucketSortHelpers.h"

auto testLambda = [](auto const &arg) {
    return arg.id;
};

auto groupingTestLambda = [](auto const &arg) {
    return arg.primaryKey;
};

struct TestElement {
    int id;
    double val;
};

struct GroupingData {
    int primaryKey;
    std::string name;
};

class BucketSortHelpersTest : public ::testing::Test
{
    public:
    std::vector<TestElement> data = {
        {100, 1.1},
        { 10, 2.2},
        { 50, 3.3},
        {100, 4.4},
        { 10, 5.5},
        {100, 6.6}
    };

    std::vector<GroupingData> dataGrouping = {
        {10,    "Ten"},
        {20, "Twenty"},
        {30, "Thirty"},
        {40,  "Forty"},
        {50,  "Fifty"}
    };

    std::vector<TestElement> dataAssociated = {
        {10, 0.0},
        {20, 1.0},
        {10, 2.0},
        {30, 3.0},
        {20, 4.0},
        {10, 5.0}
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
    std::vector<std::size_t> amounts = {2, 1, 3};
    auto offsets                     = getOffsetsFromAmounts(amounts);

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

    for (std::size_t i = 0; i < buckets.size(); ++i) {
        std::size_t start = offsets[i];
        std::size_t end   = offsets[i + 1];
        EXPECT_EQ(end - start, amounts[i]);
    }
}

TEST_F(BucketSortHelpersTest, GetIteratorsSortedTestBasicGroupingAndStability)
{
    auto buckets = getAvailableBuckets<int>(dataGrouping, groupingTestLambda);
    auto amounts = getAmountsInsideAvailableBuckets(dataAssociated, testLambda, buckets);
    auto offsets = getOffsetsFromAmounts(amounts);

    auto sortedIterators =
        getIteratorsSorted(dataAssociated, testLambda, offsets, buckets, amounts);

    EXPECT_EQ(sortedIterators[0]->id, 10);
    EXPECT_EQ(sortedIterators[0]->val, 0.0);
    EXPECT_EQ(sortedIterators[1]->id, 10);
    EXPECT_EQ(sortedIterators[1]->val, 2.0);
    EXPECT_EQ(sortedIterators[2]->id, 10);
    EXPECT_EQ(sortedIterators[2]->val, 5.0);

    EXPECT_EQ(sortedIterators[3]->id, 20);
    EXPECT_EQ(sortedIterators[3]->val, 1.0);
    EXPECT_EQ(sortedIterators[4]->id, 20);
    EXPECT_EQ(sortedIterators[4]->val, 4.0);

    EXPECT_EQ(sortedIterators[5]->id, 30);
    EXPECT_EQ(sortedIterators[5]->val, 3.0);
}

TEST_F(BucketSortHelpersTest, GetIteratorsSortedHandlesEmptyContainer)
{
    std::vector<GroupingData> emptyGroupingSource;
    std::vector<TestElement> emptyAssociatedSource;
    auto buckets = getAvailableBuckets<int>(emptyGroupingSource, groupingTestLambda);
    auto amounts = getAmountsInsideAvailableBuckets(emptyAssociatedSource, testLambda, buckets);
    auto offsets = getOffsetsFromAmounts(amounts);

    auto sortedIts =
        getIteratorsSorted(emptyAssociatedSource, testLambda, offsets, buckets, amounts);

    EXPECT_TRUE(sortedIts.empty());
}

TEST_F(BucketSortHelpersTest, GetIteratorsSortedHandlesEmptyGroupingSource)
{
    std::vector<GroupingData> emptyGroupingSource;
    auto buckets = getAvailableBuckets<int>(emptyGroupingSource, groupingTestLambda);
    auto amounts = getAmountsInsideAvailableBuckets(dataAssociated, testLambda, buckets);
    EXPECT_TRUE(amounts.empty());
    auto offsets          = getOffsetsFromAmounts(amounts);
    auto sizeOfTheOffsets = offsets.size();
    EXPECT_TRUE(sizeOfTheOffsets == 1);
    EXPECT_TRUE(offsets[0] == 0);

    auto sortedIts = getIteratorsSorted(dataAssociated, testLambda, offsets, buckets, amounts);
    EXPECT_TRUE(sortedIts.empty());
}

TEST_F(BucketSortHelpersTest, GetIteratorsSortedHandlesEmptyAssociatedSource)
{
    std::vector<TestElement> emptyAssociatedSource;
    auto buckets = getAvailableBuckets<int>(dataGrouping, groupingTestLambda);
    auto amounts = getAmountsInsideAvailableBuckets(emptyAssociatedSource, testLambda, buckets);
    EXPECT_TRUE(amounts.size() == buckets.size());
    for (const unsigned long amount : amounts) {
        EXPECT_TRUE(amount == 0);
    }

    auto offsets = getOffsetsFromAmounts(amounts);
    EXPECT_TRUE(offsets.size() == amounts.size() + 1);
    EXPECT_TRUE(offsets.back() == 0);

    auto sortedIts =
        getIteratorsSorted(emptyAssociatedSource, testLambda, offsets, buckets, amounts);
    EXPECT_TRUE(sortedIts.empty());
}

TEST_F(BucketSortHelpersTest, GetIteratorsSortedHandlesNoSuchBucketSituation)
{
    std::vector<TestElement> singleBucket = {
        {5,  1.0},
        {5,  2.0},
        {5, 33.0}
    };

    auto buckets = getAvailableBuckets<int>(dataGrouping, groupingTestLambda);
    auto amounts = getAmountsInsideAvailableBuckets(singleBucket, testLambda, buckets);

    EXPECT_TRUE(amounts.size() == buckets.size());
    for (const unsigned long amount : amounts) {
        EXPECT_TRUE(amount == 0);
    }

    auto offsets = getOffsetsFromAmounts(amounts);
    EXPECT_TRUE(offsets.size() == amounts.size() + 1);
    EXPECT_TRUE(offsets.back() == 0);

    auto sortedIts = getIteratorsSorted(singleBucket, testLambda, offsets, buckets, amounts);
    EXPECT_TRUE(sortedIts.empty());
}

TEST_F(BucketSortHelpersTest, GetIteratorsSortedOneBucket)
{
    std::vector<GroupingData> newGroupingData = {
        {1,   "One"},
        {3, "Three"},
        {5,  "Five"}
    };
    std::vector<TestElement> singleBucket = {
        {5,  1.0},
        {5,  2.0},
        {5, 33.0}
    };

    auto buckets = getAvailableBuckets<int>(newGroupingData, groupingTestLambda);
    auto amounts = getAmountsInsideAvailableBuckets(singleBucket, testLambda, buckets);
    auto offsets = getOffsetsFromAmounts(amounts);

    auto sortedIts = getIteratorsSorted(singleBucket, testLambda, offsets, buckets, amounts);
    ASSERT_EQ(sortedIts.size(), 3);
    EXPECT_EQ(sortedIts[0]->val, 1.0);
    EXPECT_EQ(sortedIts[1]->val, 2.0);
    EXPECT_EQ(sortedIts[2]->val, 33.0);
}

TEST(BucketSortHelpersManualTest, GetIteratorsSortedNonRandomAccessContainer)
{
    std::unordered_map<int, int> groupingMap = {
        {1, 1},
        {2, 3},
        {3, 2}
    };
    std::list<int> l = {2, 1, 2};
    auto call        = [](int i) {
        return i;
    };
    auto callOnMap = [](const auto &pair) {
        return pair.second;
    };

    auto buckets = getAvailableBuckets<int>(groupingMap, callOnMap);
    auto amounts = getAmountsInsideAvailableBuckets(l, call, buckets);
    auto offsets = getOffsetsFromAmounts(amounts);

    auto sortedIts = getIteratorsSorted(l, call, offsets, buckets, amounts);

    ASSERT_EQ(sortedIts.size(), 3);
    EXPECT_EQ(*sortedIts[0], 1);
    EXPECT_EQ(*sortedIts[1], 2);
    EXPECT_EQ(*sortedIts[2], 2);
}
