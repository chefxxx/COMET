//
// Created by mshamrai on 9/14/25.
//

#include <gtest/gtest.h>
#include "Combinations.h"

namespace addOneStricktlyUpper
{
// TODO: There is a bug, that first couple of iterations have the same behaviour as the full
// version.
// TODO: We should add some function like set start ranges or smth. like that
TEST(AddOneStricktlyFirst, twoVectorsSameSize)
{
    std::vector<int> v1  = {1, 2, 3, 4, 5, 6, 7, 8};
    std::vector<char> v2 = {'a', 'b', 'c', 'd', 'e'};

    Ranges r1 = {v1.begin(), v1.end()};
    Ranges r2 = {v2.begin(), v2.end()};

    auto tuple = std::make_tuple(r1, r2);

    auto combinationPolicy = StrictlyUpperCombinationsPolicy(tuple);
    // That's what I do not like - to obtain isEnd I should use mBase
    std::vector<std::tuple<int, char>> expected = {
        {1, 'a'},
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
    for (int i = 0; !combinationPolicy.mBase.isEnd; combinationPolicy.addOne(), i++) {
        auto current_val1 = *std::get<0>(combinationPolicy.mBase.mCurrentState);
        auto current_val2 = *std::get<1>(combinationPolicy.mBase.mCurrentState);

        std::cout << current_val1 << " " << current_val2 << std::endl;
        ASSERT_TRUE(current_val1 == std::get<0>(expected[i]));
        ASSERT_TRUE(current_val2 == std::get<1>(expected[i]));
    }
}

TEST(AddOneStricktlyFirst, threeVectorsDifferentSize)
{
    std::vector<int> v1  = {1, 2, 3};
    std::vector<char> v2 = {'#', '@', '$', '%', '*'};
    std::vector<char> v3 = {'a', 'b', 'c', 'd'};

    Ranges r1 = {v1.begin(), v1.end()};
    Ranges r2 = {v2.begin(), v2.end()};
    Ranges r3 = {v3.begin(), v3.end()};

    auto tuple = std::make_tuple(r1, r2, r3);

    auto combinationPolicy = StrictlyUpperCombinationsPolicy(tuple);
    // That's what I do not like - to obtain isEnd I should use mBase
    std::vector<std::tuple<int, char, char>> expected = {
        {1, '#', 'a'},
        {1, '#', 'b'},
        {1, '#', 'c'},
        {1, '#', 'd'},
        {1, '@', 'c'},
        {1, '@', 'd'},
        {1, '$', 'd'},
        {2, '$', 'd'}
    };
    for (int i = 0; !combinationPolicy.mBase.isEnd; combinationPolicy.addOne(), i++) {
        auto current_val1 = *std::get<0>(combinationPolicy.mBase.mCurrentState);
        auto current_val2 = *std::get<1>(combinationPolicy.mBase.mCurrentState);
        auto current_val3 = *std::get<2>(combinationPolicy.mBase.mCurrentState);

        std::cout << current_val1 << " " << current_val2 << " " << current_val3 << std::endl;
        ASSERT_EQ(current_val1, std::get<0>(expected[i]));
        ASSERT_EQ(current_val2, std::get<1>(expected[i]));
        ASSERT_EQ(current_val3, std::get<2>(expected[i]));
    }
}

}  // namespace addOneStricktlyUpper
