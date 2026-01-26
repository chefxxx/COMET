//
// Created by mshamrai on 1/3/26.
//

#include "include/BucketPolicy.h"
#include "include/Combinations.h"
#include "include/Producers.h"
#include "gtest/gtest.h"

struct GroupingElement {
    int Id;
    double value;
};

auto GroupingCallable = [](const GroupingElement& element) {
    return element.Id;
};

struct AssociatedElement1 {
    int groupId;
    double value;
};

auto AssociatedCallable1 = [](const AssociatedElement1& element) {
    return element.groupId;
};

struct AssociatedElement2 {
    int id;
    std::string value;
};

auto AssociatedCallable2 = [](const AssociatedElement2& element) {
    return element.id;
};

struct AssociatedElement3 {
    int parentId;
    std::string stringValue;
    int intValue;
};

auto AssociatedCallable3 = [](const AssociatedElement3& element) {
    return element.parentId;
};

class GroupedCombinationsTest : public ::testing::Test
{
    public:
    std::vector<GroupingElement> groupingSource = {
        {10, 0.20},
        {20, 0.22}
    };
    std::vector<AssociatedElement1> associatedSource1 = {
        {10, 1.1},
        {10, 1.2},
        {30, 3.1}
    };
    std::vector<AssociatedElement2> associatedSource2 = {
        {10, "Assoc2_G10_1"},
        {20,   "Assoc2_G20"},
        {10, "Assoc2_G10_2"}
    };
    std::vector<AssociatedElement3> associatedSource3 = {
        {10, "Assoc3_G10_1", 0},
        {20, "Assoc3_G20_1", 2},
        {20, "Assoc3_G20_2", 5},
        {30, "Assoc3_G30_1", 7},
        {10, "Assoc3_G10_2", 1},
    };

    SourceWithCallable<std::vector<GroupingElement>, decltype(GroupingCallable)> groupingWrapper{
        groupingSource, GroupingCallable
    };
    SourceWithCallable<std::vector<AssociatedElement1>, decltype(AssociatedCallable1)>
        assocWrapper1{associatedSource1, AssociatedCallable1};
    SourceWithCallable<std::vector<AssociatedElement2>, decltype(AssociatedCallable2)>
        assocWrapper2{associatedSource2, AssociatedCallable2};

    const std::vector<double> buckets{0.0, 0.25, 0.5, 0.75, 1.0};

    struct BucketCallable {
        auto operator()(const GroupingElement& element) const { return element.value; }
    };
    [[no_unique_address]] BucketCallable bucketCallable;
};

TEST_F(GroupedCombinationsTest, CorrectDataAlignment)
{
    const auto bucketPolicy = BucketPolicy(false, bucketCallable, buckets);

    auto grouped = makeGroupedCombinations<FullCombinationsPolicy>(
        bucketPolicy, 1, groupingWrapper, assocWrapper1, assocWrapper2
    );

    int count = 0;
    for (const auto& [el0, span1, el1, span2] : grouped) {
        count++;

        if (el0.Id == 10) {
            EXPECT_EQ(span1.size(), 2);
            EXPECT_DOUBLE_EQ(span1[0].value, 1.1);
        } else if (el0.Id == 20) {
            EXPECT_EQ(span1.size(), 0);
        }

        if (el1.Id == 10) {
            EXPECT_EQ(span2.size(), 2);  // Assoc2_G10_1, Assoc2_G10_2
        } else if (el1.Id == 20) {
            EXPECT_EQ(span2.size(), 1);  // Assoc2_G20
            EXPECT_EQ(span2[0].value, "Assoc2_G20");
        }
    }
    EXPECT_EQ(count, 4);
}

TEST_F(GroupedCombinationsTest, FiltersOutUnknownIds)
{
    const auto bucketPolicy = BucketPolicy(false, bucketCallable, buckets);

    auto grouped = makeGroupedCombinations<FullCombinationsPolicy>(
        bucketPolicy, 1, groupingWrapper, assocWrapper1, assocWrapper2
    );

    for (auto [el0, span1, el1, span2] : grouped) {
        for (auto it : span1) {
            EXPECT_NE(it.groupId, 30);
        }
    }
}

TEST_F(GroupedCombinationsTest, ThreeAssociatedSourcesWork)
{
    const auto assocWrapper3 = SourceWithCallable{associatedSource3, AssociatedCallable3};
    const auto bucketPolicy  = BucketPolicy(false, bucketCallable, buckets);

    auto grouped = makeGroupedCombinations<FullCombinationsPolicy>(
        bucketPolicy, 1, groupingWrapper, assocWrapper1, assocWrapper2, assocWrapper3
    );

    bool found20 = false;
    for (const auto& [el0, s1, el1, s2, el2, s3] : grouped) {
        if (el0.Id == 20) {
            found20 = true;
            EXPECT_EQ(s3.size(), 2);
        }
    }
    EXPECT_TRUE(found20);
}

TEST_F(GroupedCombinationsTest, PreservesOriginalOrderInSpans)
{
    const auto bucketPolicy = BucketPolicy(false, bucketCallable, buckets);

    auto grouped = makeGroupedCombinations<FullCombinationsPolicy>(
        bucketPolicy, 1, groupingWrapper, assocWrapper1, assocWrapper2
    );

    for (const auto& [el0, s1, el1, s2] : grouped) {
        if (el0.Id == 10) {
            ASSERT_GE(s1.size(), 2);
            EXPECT_DOUBLE_EQ(s1[0].value, 1.1);
            EXPECT_DOUBLE_EQ(s1[1].value, 1.2);
        }
    }
}

TEST_F(GroupedCombinationsTest, TestUseCase) {}
