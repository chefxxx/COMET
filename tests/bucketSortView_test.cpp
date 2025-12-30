//
// Created by mshamrai on 12/30/25.
//

#include "BucketSortView.h"
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
    // Data
    std::vector<GroupingStruct> groupingData = {{1}, {2}, {3}};

    // Associated data:
    std::vector<AssociatedStruct> associatedData = {
        { 1, 1.0,     "A"},
        { 2, 2.0,     "B"},
        { 1, 1.5,     "C"},
        {99, 0.0, "Stray"}
    };
};
