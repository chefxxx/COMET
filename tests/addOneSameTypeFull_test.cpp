//
// Created by mshamrai on 01/06/26.
//

#include <gtest/gtest.h>
#include <string>
#include <tuple>
#include <vector>

#include "Combinations.h"
#include "Producers.h"

class SameTypeFullCombinationsTest : public ::testing::Test
{
    public:
    std::vector<int> v_small = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::vector<int> v_empty = {};
};

TEST_F(SameTypeFullCombinationsTest, WindowSizeZero_ReturnsDiagonalOnly)
{
    int window = 0;
    auto producer =
        makeSameTypeCombinations<SameTypeFullCombinationsPolicy>(window, v_small, v_small);

    std::vector<std::tuple<int, int>> expected = {
        {1, 1},
        {2, 2},
        {3, 3},
        {4, 4},
        {5, 5},
        {6, 6},
        {7, 7},
        {8, 8},
        {9, 9}
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

TEST_F(SameTypeFullCombinationsTest, WindowSizeHuge_ReturnsAllUpperCombinations)
{
    int window = 100;
    auto producer =
        makeSameTypeCombinations<SameTypeFullCombinationsPolicy>(window, v_small, v_small);

    std::vector<std::tuple<int, int>> expected = {
        {1, 1},
        {1, 2},
        {1, 3},
        {1, 4},
        {1, 5},
        {1, 6},
        {1, 7},
        {1, 8},
        {1, 9},
        {2, 2},
        {2, 3},
        {2, 4},
        {2, 5},
        {2, 6},
        {2, 7},
        {2, 8},
        {2, 9},
        {3, 3},
        {3, 4},
        {3, 5},
        {3, 6},
        {3, 7},
        {3, 8},
        {3, 9},
        {4, 4},
        {4, 5},
        {4, 6},
        {4, 7},
        {4, 8},
        {4, 9},
        {5, 5},
        {5, 6},
        {5, 7},
        {5, 8},
        {5, 9},
        {6, 6},
        {6, 7},
        {6, 8},
        {6, 9},
        {7, 7},
        {7, 8},
        {7, 9},
        {8, 8},
        {8, 9},
        {9, 9},
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

TEST_F(SameTypeFullCombinationsTest, EmptyVectors_NoIterations)
{
    int window = 5;
    auto producer =
        makeSameTypeCombinations<SameTypeFullCombinationsPolicy>(window, v_empty, v_empty);

    int count = 0;
    for (const auto& val : producer) {
        count++;
    }
    EXPECT_EQ(count, 0);
    EXPECT_TRUE(producer.isEnd());
}

TEST_F(SameTypeFullCombinationsTest, TripletsWithWindow_GeneratesCubesOnDiagonal)
{
    int window = 1;
    auto producer =
        makeSameTypeCombinations<SameTypeFullCombinationsPolicy>(window, v_small, v_small, v_small);

    std::vector<std::tuple<int, int, int>> expected = {
        {1, 1, 1},
        {1, 1, 2},
        {1, 2, 1},
        {1, 2, 2},

        {2, 2, 2},
        {2, 2, 3},
        {2, 3, 2},
        {2, 3, 3},

        {3, 3, 3},
        {3, 3, 4},
        {3, 4, 3},
        {3, 4, 4},

        {4, 4, 4},
        {4, 4, 5},
        {4, 5, 4},
        {4, 5, 5},

        {5, 5, 5},
        {5, 5, 6},
        {5, 6, 5},
        {5, 6, 6},

        {6, 6, 6},
        {6, 6, 7},
        {6, 7, 6},
        {6, 7, 7},

        {7, 7, 7},
        {7, 7, 8},
        {7, 8, 7},
        {7, 8, 8},

        {8, 8, 8},
        {8, 8, 9},
        {8, 9, 8},
        {8, 9, 9},

        {9, 9, 9},
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

// TEST_F(SameTypeFullCombinationsTest, DifferentVectorsSameSize_IndicesAligned)
// {
//     std::vector<int> vA = {10, 20};
//     std::vector<int> vB = {100, 200};
//     int window          = 1;
//
//     auto producer = makeSameTypeCombinations<SameTypeFullCombinationsPolicy>(window, vA, vB);
//
//     std::vector<std::tuple<int, int>> expected = {
//         {10, 100},
//         {10, 200},
//         {20, 200}
//     };
//
//     int count = 0;
//     for (const auto& [a, b] : producer) {
//         ASSERT_LT(count, expected.size());
//         EXPECT_EQ(std::tie(a, b), expected[count]);
//         count++;
//     }
//     EXPECT_EQ(count, expected.size());
// }
//
// TEST_F(SameTypeFullCombinationsTest, StringTypes_WorksCorrectly)
// {
//     std::vector<std::string> v = {"A", "B", "C"};
//     int window                 = 1;
//
//     auto producer = makeSameTypeCombinations<SameTypeFullCombinationsPolicy>(window, v, v);
//
//     std::vector<std::tuple<std::string, std::string>> expected = {
//         {"A", "A"},
//         {"A", "B"},
//         {"B", "B"},
//         {"B", "C"},
//         {"C", "C"}
//     };
//
//     int count = 0;
//     for (const auto& [a, b] : producer) {
//         ASSERT_LT(count, expected.size());
//         EXPECT_EQ(std::tie(a, b), expected[count]);
//         count++;
//     }
//     EXPECT_EQ(count, expected.size());
// }
