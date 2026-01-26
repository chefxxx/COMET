//
// Created by mshamrai on 9/14/25.
//

#include <gtest/gtest.h>

#include "include/Combinations.h"
#include "include/Producers.h"

TEST(AddOneStrictlyUpper, twoVectorsSameSize)
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
        auto [ex0, ex1] = expected[i];
        ++i;
        ASSERT_EQ(elem0, ex0);
        ASSERT_EQ(elem1, ex1);
    }

    /* this loop does not even execute, because we have input iterator */
    for (const auto &[elem0, elem1] : combinationsProducer) {
        std::cout << "I do not even execute :( ...\n";
        auto [ex0, ex1] = expected[i];
        ++i;
        ASSERT_EQ(elem0, ex0);
        ASSERT_EQ(elem1, ex1);
    }
}

TEST(AddOneStrictlyUpper, threeVectorsDifferentSize)
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
        auto [ex0, ex1, ex2] = expected[i];
        ++i;
        ASSERT_EQ(elem0, ex0);
        ASSERT_EQ(elem1, ex1);
        ASSERT_EQ(elem2, ex2);
    }
}

TEST(AddOneStrictlyUpper, oneRangeIsLessThanItCanBe)
{
    const std::vector v1 = {1, 2, 3};
    const std::vector v2 = {'#'};
    const std::vector v3 = {'a', 'b', 'c', 'd'};

    const auto combinationsProducer = makeCombinations<StrictlyUpperCombinationsPolicy>(v1, v2, v3);

    ASSERT_TRUE(combinationsProducer.isEnd());
}

TEST(AddOneStrictlyUpper, classicExampleTest)
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
        auto [ex0, ex1, ex2] = expected[i];
        ++i;
        ASSERT_EQ(elem0, ex0);
        ASSERT_EQ(elem1, ex1);
        ASSERT_EQ(elem2, ex2);
    }
}
