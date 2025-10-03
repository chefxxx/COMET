//
// Created by mshamrai on 9/14/25.
//

#include <gtest/gtest.h>
#include "Combinations.h"

TEST(AddOneStrictlyFirst, twoVectorsSameSize)
{
    std::vector v1 = {1, 2, 3, 4, 5, 6, 7, 8};
    std::vector v2 = {'a', 'b', 'c', 'd', 'e'};

    Ranges r1 = {v1.begin(), v1.end()};
    Ranges r2 = {v2.begin(), v2.end()};

    auto tuple = std::make_tuple(r1, r2);

    auto combinationsProducer = makeCombinations<StrictlyUpperCombinationsPolicy>(tuple);

    std::vector<std::tuple<int, char>> expected = {
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
    for (const auto& [elem0, elem1] : combinationsProducer) {
        auto currValue = expected[i++];
        ASSERT_EQ(*elem0, std::get<0>(currValue));
        ASSERT_EQ(*elem1, std::get<1>(currValue));
    }
    /* this loop does not even execute, because we have input iterator */
    for (const auto& [elem0, elem1] : combinationsProducer) {
        auto currValue = expected[i++];
        ASSERT_EQ(*elem0, std::get<0>(currValue));
        ASSERT_EQ(*elem1, std::get<1>(currValue));
    }
}

TEST(AddOneStrictlyFirst, threeVectorsDifferentSize)
{
    std::vector v1 = {1, 2, 3};
    std::vector v2 = {'#', '@', '$', '%', '*'};
    std::vector v3 = {'a', 'b', 'c', 'd'};

    Ranges r1 = {v1.begin(), v1.end()};
    Ranges r2 = {v2.begin(), v2.end()};
    Ranges r3 = {v3.begin(), v3.end()};

    auto tuple = std::make_tuple(r1, r2, r3);

    auto combinationsProducer = makeCombinations<StrictlyUpperCombinationsPolicy>(tuple);

    const std::vector<std::tuple<int, char, char>> expected = {
        {1, '@', 'c'},
        {1, '@', 'd'},
        {1, '$', 'd'},
        {2, '$', 'd'}
    };

    int i = 0;
    for (const auto& [elem0, elem1, elem2] : combinationsProducer) {
        auto currValues = expected[i++];
        ASSERT_EQ(*elem0, std::get<0>(currValues));
        ASSERT_EQ(*elem1, std::get<1>(currValues));
        ASSERT_EQ(*elem2, std::get<2>(currValues));
    }
}

TEST(AddOneStrictlyUpper, OneRangeIsLessThanItCanBe)
{
    std::vector v1 = {1, 2, 3};
    std::vector v2 = {'#'};
    std::vector v3 = {'a', 'b', 'c', 'd'};

    Ranges r1 = {v1.begin(), v1.end()};
    Ranges r2 = {v2.begin(), v2.end()};
    Ranges r3 = {v3.begin(), v3.end()};

    auto tuple                      = std::make_tuple(r1, r2, r3);
    const auto combinationsProducer = makeCombinations<StrictlyUpperCombinationsPolicy>(tuple);

    ASSERT_TRUE(combinationsProducer.isEnd());
}

TEST(AddOneStrictlyUpper, ClassicExampleTest)
{
    std::vector v1 = {0, 1, 2, 3, 4};
    std::vector v2 = {0, 1, 2, 3, 4};
    std::vector v3 = {0, 1, 2, 3, 4};

    Ranges r1 = {v1.begin(), v1.end()};
    Ranges r2 = {v2.begin(), v2.end()};
    Ranges r3 = {v3.begin(), v3.end()};

    auto tuple                = std::make_tuple(r1, r2, r3);
    auto combinationsProducer = makeCombinations<StrictlyUpperCombinationsPolicy>(tuple);

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
    for (const auto& [elem0, elem1, elem2] : combinationsProducer) {
        auto currValues = expected[i++];

        ASSERT_EQ(*elem0, std::get<0>(currValues));
        ASSERT_EQ(*elem1, std::get<1>(currValues));
        ASSERT_EQ(*elem2, std::get<2>(currValues));
    }
}
