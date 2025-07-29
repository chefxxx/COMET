//
// Created by mshamrai on 7/17/25.
//

#ifndef BUCKETPOLICY_H
#define BUCKETPOLICY_H

#include <gtest/gtest_prod.h>
#include <algorithm>
#include <array>
#include <iterator>
#include <tuple>
#include <unordered_map>
#include <vector>

template <typename T>
concept is_less_comparable = requires(T t, T u) {
    { t < u } -> std::same_as<bool>;
};

template <std::forward_iterator TIter, typename... TCallables>
concept is_function_callable_on_element = (std::invocable<TCallables, TIter> && ...);

template <std::forward_iterator TIter, typename... TCallables>
concept is_function_binable_on_doubles =
    is_function_callable_on_element<TIter, TCallables...> &&
    (std::is_convertible_v<std::invoke_result_t<TCallables, TIter>, double> && ...);

template <typename T>
    requires is_less_comparable<T>
int findIndex(std::vector<T> const &data, T const &value, const bool ignoreOverflows)
{
    const auto tmp =
        static_cast<int>(distance(data.begin(), std::upper_bound(data.begin(), data.end(), value)));
    return ignoreOverflows && (tmp == 0 || tmp == static_cast<int>(data.size())) ? -1 : tmp;
}

template <typename... TCallables>
struct BucketPolicy final {
    BucketPolicy(
        std::tuple<TCallables...> const &callables,
        std::array<std::vector<double>, sizeof...(TCallables)> const &bucketsRanges,
        const bool ignoreOverflows = true
    )
        : mCallables(callables), mBucketsRanges(bucketsRanges), ignoreOverflows(ignoreOverflows)
    {
    }

    template <std::forward_iterator TIter>
        requires is_function_callable_on_element<TIter, TCallables...>
    [[nodiscard]] int getBucket(TIter const &it)
    {
        auto values  = getValues(it);
        auto indices = getUpperIndicesForTuple(values);
        auto bucket  = calculateBucketAtIndices(indices);
        return bucket;
    }

    [[nodiscard]] int getMaximalBucketCount() const
    {
        return [&]<std::size_t... I>(std::index_sequence<I...>) {
            return (1 * ... * (std::get<I>(mBucketsRanges).size() + 1));
        }(std::make_index_sequence<sizeof...(TCallables)>{});
    }

    template <std::forward_iterator TIter>
    [[nodiscard]] auto getValues(TIter const &it)
    {
        return std::make_tuple(std::get<TCallables>(mCallables)(it)...);
    }

    template <typename... Types>
    [[nodiscard]] auto getUpperIndicesForTuple(std::tuple<Types...> const &values)
    {
        return [&]<std::size_t... I>(std::index_sequence<I...>) {
            return std::make_tuple(
                findIndex(std::get<I>(mBucketsRanges), std::get<I>(values), ignoreOverflows)...
            );
        }(std::make_index_sequence<sizeof...(TCallables)>{});
    }

    template <typename... TIndices>
    [[nodiscard]] int calculateBucketAtIndices(std::tuple<TIndices...> const &indices)
    {
        constexpr auto N = sizeof...(TIndices);
        auto indexSeq    = std::make_index_sequence<N - 1>();
        return ignoreOverflows && checkUnderOverflows(indices)
                   ? -1
                   : [&]<size_t... I>(std::index_sequence<I...>) {
                         return (
                             std::get<0>(indices) + ... +
                             (std::get<I + 1>(indices) *
                              (1 * ... * (std::get<I>(mBucketsRanges).size() + 1)))
                         );
                     }(indexSeq);
    }

    private:
    /*                  --- TEST SECTION ---                    */
    FRIEND_TEST(BinarySearchTest, oneElementVector_indexOne);
    FRIEND_TEST(GetUpperIndicesForTupleTest, fiveRangesInEachDimensionWithOverflows);
    FRIEND_TEST(GetUpperIndicesForTupleTest, fiveRangesInEachDimensionWithoutOverflows);
    FRIEND_TEST(GetUpperIndicesForTupleTest, sevenAndFiveRangesWithOverflows);
    FRIEND_TEST(GetUpperIndicesForTupleTest, manyDimensionsWithOverflows);
    FRIEND_TEST(CalculateBucketAtIndicesTest, fiveRangesInTwoDimensionsWithout);
    FRIEND_TEST(GetBucketTest, manyDimensionsWithOverflow);
    /*                  ---              ---                    */

    template <typename... Types>
    bool checkUnderOverflows(std::tuple<Types...> arg) const
    {
        return [&arg]<size_t... I>(std::index_sequence<I...>) {
            return ((std::get<I>(arg) == -1) || ...);
        }(std::make_index_sequence<sizeof...(Types)>{});
    }

    std::tuple<TCallables...> mCallables;
    std::array<std::vector<double>, sizeof...(TCallables)> mBucketsRanges;
    bool ignoreOverflows;
};

#endif  // BUCKETPOLICY_H
