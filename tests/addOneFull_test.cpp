//
// Created by mshamrai on 9/13/25.
//

#include <gtest/gtest.h>
#include "Combinations.h"

TEST(AddOneFullTest, twoVectorsSameSize)
{
    std::vector v1 = {1, 2, 3, 4, 5, 6, 7, 8};
    std::vector v2 = {'a', 'b', 'c', 'd', 'e'};

    Ranges r1 = {v1.begin(), v1.end()};
    Ranges r2 = {v2.begin(), v2.end()};

    auto tuple = std::make_tuple(r1, r2);

    // Why does it compile with tuple created before and not without
    // auto combinationPolicy = FullCombinationsPolicy({r1, r2}); ??? It is interesting
    // Answer: to make it work we should add forwarding

    auto combinationsProducer = makeCombinations<FullCombinationsPolicy>(tuple);

    const std::vector<std::tuple<int, char>> expectedValues = {
        {1, 'a'},
        {1, 'b'},
        {1, 'c'},
        {1, 'd'},
        {1, 'e'},
        {2, 'a'},
        {2, 'b'},
        {2, 'c'},
        {2, 'd'},
        {2, 'e'},
        {3, 'a'},
        {3, 'b'},
        {3, 'c'},
        {3, 'd'},
        {3, 'e'},
        {4, 'a'},
        {4, 'b'},
        {4, 'c'},
        {4, 'd'},
        {4, 'e'},
        {5, 'a'},
        {5, 'b'},
        {5, 'c'},
        {5, 'd'},
        {5, 'e'},
        {6, 'a'},
        {6, 'b'},
        {6, 'c'},
        {6, 'd'},
        {6, 'e'},
        {7, 'a'},
        {7, 'b'},
        {7, 'c'},
        {7, 'd'},
        {7, 'e'},
        {8, 'a'},
        {8, 'b'},
        {8, 'c'},
        {8, 'd'},
        {8, 'e'}
    };

    int i = 0;
    for (const auto& [elem0, elem1] : combinationsProducer) {
        auto expectedCombination = expectedValues[i++];
        ASSERT_EQ(*elem0, std::get<0>(expectedCombination));
        ASSERT_EQ(*elem1, std::get<1>(expectedCombination));
    }
}

TEST(AddOneFullTest, fourVectorsDifferentSizes)
{
    std::vector v1              = {1, 2, 3};
    std::vector v2              = {'a', 'b', 'c', 'd', 'e'};
    std::vector v3              = {'=', '#', '$', '!'};
    std::vector<std::string> v4 = {"Michał", "Mati"};

    Ranges r1 = {v1.begin(), v1.end()};
    Ranges r2 = {v2.begin(), v2.end()};
    Ranges r3 = {v3.begin(), v3.end()};
    Ranges r4 = {v4.begin(), v4.end()};

    auto tuple = std::make_tuple(r1, r2, r3, r4);

    auto combinationsProducer = makeCombinations<FullCombinationsPolicy>(tuple);

    std::vector<std::tuple<int, char, char, std::string>> expected = {
  // Combinations starting with 1
        {1, 'a', '=', "Michał"},
        {1, 'a', '=',    "Mati"},
        {1, 'a', '#', "Michał"},
        {1, 'a', '#',    "Mati"},
        {1, 'a', '$', "Michał"},
        {1, 'a', '$',    "Mati"},
        {1, 'a', '!', "Michał"},
        {1, 'a', '!',    "Mati"},
        {1, 'b', '=', "Michał"},
        {1, 'b', '=',    "Mati"},
        {1, 'b', '#', "Michał"},
        {1, 'b', '#',    "Mati"},
        {1, 'b', '$', "Michał"},
        {1, 'b', '$',    "Mati"},
        {1, 'b', '!', "Michał"},
        {1, 'b', '!',    "Mati"},
        {1, 'c', '=', "Michał"},
        {1, 'c', '=',    "Mati"},
        {1, 'c', '#', "Michał"},
        {1, 'c', '#',    "Mati"},
        {1, 'c', '$', "Michał"},
        {1, 'c', '$',    "Mati"},
        {1, 'c', '!', "Michał"},
        {1, 'c', '!',    "Mati"},
        {1, 'd', '=', "Michał"},
        {1, 'd', '=',    "Mati"},
        {1, 'd', '#', "Michał"},
        {1, 'd', '#',    "Mati"},
        {1, 'd', '$', "Michał"},
        {1, 'd', '$',    "Mati"},
        {1, 'd', '!', "Michał"},
        {1, 'd', '!',    "Mati"},
        {1, 'e', '=', "Michał"},
        {1, 'e', '=',    "Mati"},
        {1, 'e', '#', "Michał"},
        {1, 'e', '#',    "Mati"},
        {1, 'e', '$', "Michał"},
        {1, 'e', '$',    "Mati"},
        {1, 'e', '!', "Michał"},
        {1, 'e', '!',    "Mati"},

 // Combinations starting with 2
        {2, 'a', '=', "Michał"},
        {2, 'a', '=',    "Mati"},
        {2, 'a', '#', "Michał"},
        {2, 'a', '#',    "Mati"},
        {2, 'a', '$', "Michał"},
        {2, 'a', '$',    "Mati"},
        {2, 'a', '!', "Michał"},
        {2, 'a', '!',    "Mati"},
        {2, 'b', '=', "Michał"},
        {2, 'b', '=',    "Mati"},
        {2, 'b', '#', "Michał"},
        {2, 'b', '#',    "Mati"},
        {2, 'b', '$', "Michał"},
        {2, 'b', '$',    "Mati"},
        {2, 'b', '!', "Michał"},
        {2, 'b', '!',    "Mati"},
        {2, 'c', '=', "Michał"},
        {2, 'c', '=',    "Mati"},
        {2, 'c', '#', "Michał"},
        {2, 'c', '#',    "Mati"},
        {2, 'c', '$', "Michał"},
        {2, 'c', '$',    "Mati"},
        {2, 'c', '!', "Michał"},
        {2, 'c', '!',    "Mati"},
        {2, 'd', '=', "Michał"},
        {2, 'd', '=',    "Mati"},
        {2, 'd', '#', "Michał"},
        {2, 'd', '#',    "Mati"},
        {2, 'd', '$', "Michał"},
        {2, 'd', '$',    "Mati"},
        {2, 'd', '!', "Michał"},
        {2, 'd', '!',    "Mati"},
        {2, 'e', '=', "Michał"},
        {2, 'e', '=',    "Mati"},
        {2, 'e', '#', "Michał"},
        {2, 'e', '#',    "Mati"},
        {2, 'e', '$', "Michał"},
        {2, 'e', '$',    "Mati"},
        {2, 'e', '!', "Michał"},
        {2, 'e', '!',    "Mati"},

 // Combinations starting with 3
        {3, 'a', '=', "Michał"},
        {3, 'a', '=',    "Mati"},
        {3, 'a', '#', "Michał"},
        {3, 'a', '#',    "Mati"},
        {3, 'a', '$', "Michał"},
        {3, 'a', '$',    "Mati"},
        {3, 'a', '!', "Michał"},
        {3, 'a', '!',    "Mati"},
        {3, 'b', '=', "Michał"},
        {3, 'b', '=',    "Mati"},
        {3, 'b', '#', "Michał"},
        {3, 'b', '#',    "Mati"},
        {3, 'b', '$', "Michał"},
        {3, 'b', '$',    "Mati"},
        {3, 'b', '!', "Michał"},
        {3, 'b', '!',    "Mati"},
        {3, 'c', '=', "Michał"},
        {3, 'c', '=',    "Mati"},
        {3, 'c', '#', "Michał"},
        {3, 'c', '#',    "Mati"},
        {3, 'c', '$', "Michał"},
        {3, 'c', '$',    "Mati"},
        {3, 'c', '!', "Michał"},
        {3, 'c', '!',    "Mati"},
        {3, 'd', '=', "Michał"},
        {3, 'd', '=',    "Mati"},
        {3, 'd', '#', "Michał"},
        {3, 'd', '#',    "Mati"},
        {3, 'd', '$', "Michał"},
        {3, 'd', '$',    "Mati"},
        {3, 'd', '!', "Michał"},
        {3, 'd', '!',    "Mati"},
        {3, 'e', '=', "Michał"},
        {3, 'e', '=',    "Mati"},
        {3, 'e', '#', "Michał"},
        {3, 'e', '#',    "Mati"},
        {3, 'e', '$', "Michał"},
        {3, 'e', '$',    "Mati"},
        {3, 'e', '!', "Michał"},
        {3, 'e', '!',    "Mati"}
    };

    int i = 0;
    for (const auto& [elem0, elem1, elem2, elem3] : combinationsProducer)
    {
        auto currentValues = expected[i++];
        ASSERT_TRUE(*elem0 == std::get<0>(currentValues));
        ASSERT_TRUE(*elem1 == std::get<1>(currentValues));
        ASSERT_TRUE(*elem2 == std::get<2>(currentValues));
        ASSERT_TRUE(*elem3 == std::get<3>(currentValues));
    }
}


TEST(AddOneFullTest, OneRangeIsEmpty)
{
    std::vector v1       = {1, 2, 3};
    std::vector<char> v2 = {};
    std::vector v3       = {'a', 'b', 'c', 'd'};

    Ranges r1 = {v1.begin(), v1.end()};
    Ranges r2 = {v2.begin(), v2.end()};
    Ranges r3 = {v3.begin(), v3.end()};

    auto tuple = std::make_tuple(r1, r2, r3);
    const auto combinationsProducer = makeCombinations<FullCombinationsPolicy>(tuple);
    ASSERT_TRUE(combinationsProducer.finished());
}