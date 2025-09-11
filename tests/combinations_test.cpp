//
// Created by Mateusz Mikiciuk on 11/09/2025.
//

#include "Combinations.h"
#include <gtest/gtest.h>

class CombinationsPolicyBaseTest : public ::testing::Test
{
    protected:
    std::vector<int> v1    = {1, 2, 3, 4};
    std::vector<double> v2 = {1.0, 2.0, 3.0, 4.0};
};

TEST_F(CombinationsPolicyBaseTest, IteratorTypeGeneratesCorrectCombinationsType)
{
    Ranges r1{v1.begin(), v1.end()};
    Ranges r2{v2.begin(), v2.end()};
    auto t = std::make_tuple(r1, r2);
    CombinationsPolicyBase combinations{t};
}
