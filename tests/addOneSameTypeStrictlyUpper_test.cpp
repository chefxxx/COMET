//
// Created by mshamrai on 1/11/26.
//

#include <gtest/gtest.h>
#include <string>
#include <tuple>
#include <vector>

#include "include/Combinations.h"
#include "include/Producers.h"

class SameTypeStrictlyUpperCombinationsTest : public ::testing::Test
{
    public:
    std::vector<int> v_small = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::vector<int> v_empty = {};
};

TEST_F(SameTypeStrictlyUpperCombinationsTest, WindowSizeZero_ReturnsDiagonalOnly)
{
    int window = 0;
    auto producer =
        makeSameKindCombinations<SameTypeStrictlyUpperCombinationsPolicy>(window, v_small, v_small);

    std::vector<std::tuple<int, int>> expected = {};

    int count = 0;
    for (const auto& [a, b] : producer) {
        ASSERT_LT(count, expected.size());
        EXPECT_EQ(a, std::get<0>(expected[count]));
        EXPECT_EQ(b, std::get<1>(expected[count]));
        count++;
    }
    EXPECT_EQ(count, expected.size());
}

TEST_F(SameTypeStrictlyUpperCombinationsTest, WindowSizeHuge_ReturnsAllUpperCombinations)
{
    int window = 100;
    auto producer =
        makeSameKindCombinations<SameTypeStrictlyUpperCombinationsPolicy>(window, v_small, v_small);

    std::vector<std::tuple<int, int>> expected = {
        {1, 2},
        {1, 3},
        {1, 4},
        {1, 5},
        {1, 6},
        {1, 7},
        {1, 8},
        {1, 9},
        {2, 3},
        {2, 4},
        {2, 5},
        {2, 6},
        {2, 7},
        {2, 8},
        {2, 9},
        {3, 4},
        {3, 5},
        {3, 6},
        {3, 7},
        {3, 8},
        {3, 9},
        {4, 5},
        {4, 6},
        {4, 7},
        {4, 8},
        {4, 9},
        {5, 6},
        {5, 7},
        {5, 8},
        {5, 9},
        {6, 7},
        {6, 8},
        {6, 9},
        {7, 8},
        {7, 9},
        {8, 9},
    };

    int count = 0;
    for (const auto& [a, b] : producer) {
        ASSERT_LT(count, expected.size());
        EXPECT_EQ(a, std::get<0>(expected[count]));
        EXPECT_EQ(b, std::get<1>(expected[count]));
        count++;
    }
    EXPECT_EQ(count, expected.size());
}

TEST_F(SameTypeStrictlyUpperCombinationsTest, TripletsWithWindow)
{
    int window    = 3;
    auto producer = makeSameKindCombinations<SameTypeStrictlyUpperCombinationsPolicy>(
        window, v_small, v_small, v_small
    );

    std::vector<std::tuple<int, int, int>> expected = {
        {1, 2, 3},
        {1, 2, 4},
        {1, 3, 4},

        {2, 3, 4},
        {2, 3, 5},
        {2, 4, 5},

        {3, 4, 5},
        {3, 4, 6},
        {3, 5, 6},

        {4, 5, 6},
        {4, 5, 7},
        {4, 6, 7},

        {5, 6, 7},
        {5, 6, 8},
        {5, 7, 8},

        {6, 7, 8},
        {6, 7, 9},
        {6, 8, 9},

        {7, 8, 9},
    };

    int count = 0;
    for (const auto& [a, b, c] : producer) {
        ASSERT_LT(count, expected.size());
        EXPECT_EQ(a, std::get<0>(expected[count]));
        EXPECT_EQ(b, std::get<1>(expected[count]));
        EXPECT_EQ(c, std::get<2>(expected[count]));
        count++;
    }
    EXPECT_EQ(count, expected.size());
}

TEST_F(SameTypeStrictlyUpperCombinationsTest, StringTypes_WorksCorrectly)
{
    std::vector<std::string> v = {"A", "B", "C", "D", "E"};
    int window                 = 1;

    auto producer = makeSameKindCombinations<SameTypeStrictlyUpperCombinationsPolicy>(window, v, v);

    std::vector<std::tuple<std::string, std::string>> expected = {
        {"A", "B"},
        {"B", "C"},
        {"C", "D"},
        {"D", "E"},
    };

    int count = 0;
    for (const auto& [a, b] : producer) {
        ASSERT_LT(count, expected.size());
        EXPECT_EQ(std::tie(a, b), expected[count]);
        count++;
    }
    EXPECT_EQ(count, expected.size());
}
