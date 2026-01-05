//
// Created by mshamrai on 1/3/26.
//

#include "BucketPolicy.h"
#include "Combinations.h"
#include "GroupedCombinations.h"
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

    const std::vector<double> buckets{0.0, 0.25, 0.5, 0.75, 1.0};

    struct BucketCallable {
        auto operator()(const GroupingElement& element) const { return element.value; }
    };
    [[no_unique_address]] BucketCallable bucketCallable;
};

TEST_F(GroupedCombinationsTest, ConstructionWithTwoAssociatedSources)
{
    // Wrap sources with callables as expected by GroupedCombinations
    auto groupingWrapper    = SourceWithCallable{groupingSource, GroupingCallable};
    auto associatedWrapper1 = SourceWithCallable{associatedSource1, AssociatedCallable1};
    auto associatedWrapper2 = SourceWithCallable{associatedSource2, AssociatedCallable2};
    auto associatedWrapper3 = SourceWithCallable{associatedSource3, AssociatedCallable3};

    // Define Bucket Policy and Min Cat Size
    const auto bucketPolicy = BucketPolicy(false, bucketCallable, buckets);
    int minCatSize          = 1;

    auto groupedCombinations = makeGroupedCombinations<FullCombinationsPolicy>(
        bucketPolicy, minCatSize, groupingWrapper, associatedWrapper1, associatedWrapper2
    );

    auto blockFull = makeBlockCombinations<FullCombinationsPolicy>(bucketPolicy, minCatSize, groupingSource, groupingSource);
    for (auto [elem0, elem1] : blockFull) {
        std::cout << "elo" << std::endl;
    }

    for (auto [elem0, elem1] : groupedCombinations) {
        std::cout << "elo" << std::endl;
    }
}
