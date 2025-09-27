// //
// // Created by mshamrai on 9/14/25.
// //
//
// #include <gtest/gtest.h>
// #include "Combinations.h"
//
// TEST(AddOneStrictlyFirst, twoVectorsSameSize)
// {
//     std::vector v1 = {1, 2, 3, 4, 5, 6, 7, 8};
//     std::vector v2 = {'a', 'b', 'c', 'd', 'e'};
//
//     Ranges r1 = {v1.begin(), v1.end()};
//     Ranges r2 = {v2.begin(), v2.end()};
//
//     auto tuple = std::make_tuple(r1, r2);
//
//     using combinationsType =
//         StrictlyUpperCombinationsPolicy<std::vector<int>::iterator, std::vector<char>::iterator>;
//     combinationsType combinationPolicy{};
//     combinationPolicy.setData(tuple);
//
//     std::vector<std::tuple<int, char>> expected = {
//         {1, 'b'},
//         {1, 'c'},
//         {1, 'd'},
//         {1, 'e'},
//         {2, 'c'},
//         {2, 'd'},
//         {2, 'e'},
//         {3, 'd'},
//         {3, 'e'},
//         {4, 'e'},
//     };
//     for (int i = 0; !combinationPolicy.mBase.isEnd; combinationPolicy.addOne(), i++) {
//         const auto current_val1 = *std::get<0>(combinationPolicy.mBase.mCurrentState);
//         const auto current_val2 = *std::get<1>(combinationPolicy.mBase.mCurrentState);
//
//         std::cout << current_val1 << " " << current_val2 << std::endl;
//         ASSERT_TRUE(current_val1 == std::get<0>(expected[i]));
//         ASSERT_TRUE(current_val2 == std::get<1>(expected[i]));
//     }
// }
//
// TEST(AddOneStrictlyFirst, threeVectorsDifferentSize)
// {
//     std::vector v1 = {1, 2, 3};
//     std::vector v2 = {'#', '@', '$', '%', '*'};
//     std::vector v3 = {'a', 'b', 'c', 'd'};
//
//     Ranges r1 = {v1.begin(), v1.end()};
//     Ranges r2 = {v2.begin(), v2.end()};
//     Ranges r3 = {v3.begin(), v3.end()};
//
//     auto tuple = std::make_tuple(r1, r2, r3);
//
//     using combinationsType = StrictlyUpperCombinationsPolicy<
//         std::vector<int>::iterator, std::vector<char>::iterator, std::vector<char>::iterator>;
//     combinationsType combinationPolicy{};
//     combinationPolicy.setData(tuple);
//
//     const std::vector<std::tuple<int, char, char>> expected = {
//         {1, '@', 'c'},
//         {1, '@', 'd'},
//         {1, '$', 'd'},
//         {2, '$', 'd'}
//     };
//
//     for (int i = 0; !combinationPolicy.mBase.isEnd; combinationPolicy.addOne(), i++) {
//         auto current_val1 = *std::get<0>(combinationPolicy.mBase.mCurrentState);
//         auto current_val2 = *std::get<1>(combinationPolicy.mBase.mCurrentState);
//         auto current_val3 = *std::get<2>(combinationPolicy.mBase.mCurrentState);
//
//         std::cout << current_val1 << " " << current_val2 << " " << current_val3 << std::endl;
//         ASSERT_EQ(current_val1, std::get<0>(expected[i]));
//         ASSERT_EQ(current_val2, std::get<1>(expected[i]));
//         ASSERT_EQ(current_val3, std::get<2>(expected[i]));
//     }
// }
//
// TEST(AddOneStrictlyUpper, OneRangeIsLessThanItCanBe)
// {
//     std::vector v1 = {1, 2, 3};
//     std::vector v2 = {'#'};
//     std::vector v3 = {'a', 'b', 'c', 'd'};
//
//     Ranges r1 = {v1.begin(), v1.end()};
//     Ranges r2 = {v2.begin(), v2.end()};
//     Ranges r3 = {v3.begin(), v3.end()};
//
//     auto tuple = std::make_tuple(r1, r2, r3);
//
//     using combinationsType = StrictlyUpperCombinationsPolicy<
//         std::vector<int>::iterator, std::vector<char>::iterator, std::vector<char>::iterator>;
//     combinationsType combinationPolicy{};
//     combinationPolicy.setData(tuple);
//
//     ASSERT_TRUE(combinationPolicy.mBase.isEnd);
// }
//
// TEST(AddOneStrictlyUpper, ClassicExampleTest)
// {
//     std::vector v1 = {0, 1, 2, 3, 4};
//     std::vector v2 = {0, 1, 2, 3, 4};
//     std::vector v3 = {0, 1, 2, 3, 4};
//
//     Ranges r1 = {v1.begin(), v1.end()};
//     Ranges r2 = {v2.begin(), v2.end()};
//     Ranges r3 = {v3.begin(), v3.end()};
//
//     auto tuple = std::make_tuple(r1, r2, r3);
//
//     using combinationsType = StrictlyUpperCombinationsPolicy<
//         std::vector<int>::iterator, std::vector<int>::iterator, std::vector<int>::iterator>;
//     combinationsType combinationPolicy{};
//     combinationPolicy.setData(tuple);
//
//     const std::vector<std::tuple<int, int, int>> expected = {
//         {0, 1, 2},
//         {0, 1, 3},
//         {0, 1, 4},
//         {0, 2, 3},
//         {0, 2, 4},
//         {0, 3, 4},
//         {1, 2, 3},
//         {1, 2, 4},
//         {1, 3, 4},
//         {2, 3, 4}
//     };
//
//     for (int i = 0; !combinationPolicy.mBase.isEnd; combinationPolicy.addOne(), i++) {
//         const auto current_val1 = *std::get<0>(combinationPolicy.mBase.mCurrentState);
//         const auto current_val2 = *std::get<1>(combinationPolicy.mBase.mCurrentState);
//         const auto current_val3 = *std::get<2>(combinationPolicy.mBase.mCurrentState);
//
//         ASSERT_EQ(current_val1, std::get<0>(expected[i]));
//         ASSERT_EQ(current_val2, std::get<1>(expected[i]));
//         ASSERT_EQ(current_val3, std::get<2>(expected[i]));
//     }
// }
