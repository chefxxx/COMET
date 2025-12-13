//
// Created by mshamrai on 9/14/25.
//

#include <gtest/gtest.h>
#include "Combinations.h"

TEST(AddOneStrictlyFirst, twoVectorsSameSize)
{
    const std::vector v1 = {1, 2, 3, 4, 5, 6, 7, 8};
    const std::vector v2 = {'a', 'b', 'c', 'd', 'e'};

    auto combinationsProducer = makeCombinations<StrictlyUpperCombinationsPolicy>(v1, v2);

    const std::vector<std::tuple<int, char>> expected = {
        {1, 'b'},
        {1, 'c'},
        {1, 'd'},
        {1, 'e'},
        {2, 'c'},
        {2, 'd'},
        {2, 'e'},
        {3, 'd'},
        {3, 'e'},
        {4, 'e'},
    };

    int i = 0;
    for (const auto &[elem0, elem1] : combinationsProducer) {
        auto currValue = expected[i++];
        ASSERT_EQ(*elem0, std::get<0>(currValue));
        ASSERT_EQ(*elem1, std::get<1>(currValue));
    }

    /* this loop does not even execute, because we have input iterator */
    for (const auto &[elem0, elem1] : combinationsProducer) {
        auto currValue = expected[i++];
        ASSERT_EQ(*elem0, std::get<0>(currValue));
        ASSERT_EQ(*elem1, std::get<1>(currValue));
    }
}

TEST(AddOneStrictlyFirst, threeVectorsDifferentSize)
{
    const std::vector v1 = {1, 2, 3};
    const std::vector v2 = {'#', '@', '$', '%', '*'};
    const std::vector v3 = {'a', 'b', 'c', 'd'};

    auto combinationsProducer = makeCombinations<StrictlyUpperCombinationsPolicy>(v1, v2, v3);

    const std::vector<std::tuple<int, char, char>> expected = {
        {1, '@', 'c'},
        {1, '@', 'd'},
        {1, '$', 'd'},
        {2, '$', 'd'}
    };

    int i = 0;
    for (const auto &[elem0, elem1, elem2] : combinationsProducer) {
        auto currValues = expected[i++];
        ASSERT_EQ(*elem0, std::get<0>(currValues));
        ASSERT_EQ(*elem1, std::get<1>(currValues));
        ASSERT_EQ(*elem2, std::get<2>(currValues));
    }
}

TEST(AddOneStrictlyUpper, OneRangeIsLessThanItCanBe)
{
    const std::vector v1 = {1, 2, 3};
    const std::vector v2 = {'#'};
    const std::vector v3 = {'a', 'b', 'c', 'd'};

    const auto combinationsProducer = makeCombinations<StrictlyUpperCombinationsPolicy>(v1, v2, v3);

    ASSERT_TRUE(combinationsProducer.isEnd());
}

TEST(AddOneStrictlyUpper, ClassicExampleTest)
{
    const std::vector v1 = {0, 1, 2, 3, 4};
    const std::vector v2 = {0, 1, 2, 3, 4};
    const std::vector v3 = {0, 1, 2, 3, 4};

    auto combinationsProducer = makeCombinations<StrictlyUpperCombinationsPolicy>(v1, v2, v3);

    const std::vector<std::tuple<int, int, int>> expected = {
        {0, 1, 2},
        {0, 1, 3},
        {0, 1, 4},
        {0, 2, 3},
        {0, 2, 4},
        {0, 3, 4},
        {1, 2, 3},
        {1, 2, 4},
        {1, 3, 4},
        {2, 3, 4}
    };

    int i = 0;
    for (const auto &[elem0, elem1, elem2] : combinationsProducer) {
        auto currValues = expected[i++];

        ASSERT_EQ(*elem0, std::get<0>(currValues));
        ASSERT_EQ(*elem1, std::get<1>(currValues));
        ASSERT_EQ(*elem2, std::get<2>(currValues));
    }
}
