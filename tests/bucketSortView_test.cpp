//
// Created by mshamrai on 12/30/25.
//

#include "include/BucketSortView.h"
#include <gtest/gtest.h>

struct GroupingStruct {
    int id;
};

struct AssociatedStruct {
    int groupingStructId;
    double pt;
    std::string name;
};

auto getGroupingStructId = [](const GroupingStruct& c) {
    return c.id;
};

auto getAssociatedStructParent = [](const AssociatedStruct& t) {
    return t.groupingStructId;
};

class BucketSortViewTest : public ::testing::Test
{
    public:
    std::vector<GroupingStruct> groupingData = {{1}, {2}, {3}};

    std::vector<AssociatedStruct> associatedData = {
        { 1, 1.0,     "A"},
        { 2, 2.0,     "B"},
        { 1, 1.5,     "C"},
        {99, 0.0, "Stray"}
    };
};

TEST_F(BucketSortViewTest, CorrectGroupingAndAccess)
{
    auto view = BucketSortView(
        groupingData, getGroupingStructId, associatedData, getAssociatedStructParent
    );

    auto span1 = view.getSpanForBucket(1);
    ASSERT_EQ(span1.size(), 2);
    EXPECT_EQ(span1[0].name, "A");
    EXPECT_EQ(span1[1].name, "C");

    auto span2 = view.getSpanForBucket(2);
    ASSERT_EQ(span2.size(), 1);
    EXPECT_EQ(span2[0].name, "B");

    auto span3 = view.getSpanForBucket(3);
    EXPECT_TRUE(span3.empty());
}

TEST_F(BucketSortViewTest, StabilityIsPreserved)
{
    std::vector<AssociatedStruct> associatedDataSameGroup = {
        {1, 10.0,  "First"},
        {1, 20.0, "Second"},
        {1, 30.0,  "Third"}
    };
    auto view = BucketSortView(
        groupingData, getGroupingStructId, associatedDataSameGroup, getAssociatedStructParent
    );
    auto span = view.getSpanForBucket(1);
    ASSERT_EQ(span.size(), 3);
    EXPECT_EQ(span[0].name, "First");
    EXPECT_EQ(span[1].name, "Second");
    EXPECT_EQ(span[2].name, "Third");
}

TEST_F(BucketSortViewTest, CheckNonExistingGroupingId)
{
    auto view = BucketSortView(
        groupingData, getGroupingStructId, associatedData, getAssociatedStructParent
    );

    auto span = view.getSpanForBucket(11);
    EXPECT_TRUE(span.empty());
}

TEST_F(BucketSortViewTest, CheckIfGroupingIsEmpty)
{
    std::vector<GroupingStruct> groupingDataEmpty;
    auto view = BucketSortView(
        groupingDataEmpty, getGroupingStructId, associatedData, getAssociatedStructParent
    );

    auto span = view.getSpanForBucket(1);
    EXPECT_TRUE(span.empty());
}

TEST_F(BucketSortViewTest, CheckIfAssociatedIsEmpty)
{
    std::vector<AssociatedStruct> associatedDataEmpty;
    auto view = BucketSortView(
        groupingData, getGroupingStructId, associatedDataEmpty, getAssociatedStructParent
    );

    auto span = view.getSpanForBucket(1);
    EXPECT_TRUE(span.empty());
    span = view.getSpanForBucket(2);
    EXPECT_TRUE(span.empty());
    span = view.getSpanForBucket(3);
    EXPECT_TRUE(span.empty());
}

TEST_F(BucketSortViewTest, CheckIfStringBucketTypeIsDeduced)
{
    struct StringId {
        std::string val;
    };
    struct StringData {
        std::string sid;
    };

    std::vector<StringId> ids    = {{"alpha"}, {"beta"}};
    std::vector<StringData> data = {{"beta"}, {"alpha"}, {"alpha"}};

    auto idCall = [](const StringId& s) {
        return s.val;
    };
    auto dataCall = [](const StringData& d) {
        return d.sid;
    };

    auto view = BucketSortView(ids, idCall, data, dataCall);

    auto spanAlpha = view.getSpanForBucket("alpha");
    EXPECT_EQ(spanAlpha.size(), 2);
    auto spanBeta = view.getSpanForBucket("beta");
    EXPECT_EQ(spanBeta.size(), 1);
}
