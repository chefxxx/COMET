//
// Created by mshamrai on 7/17/25.
//

#ifndef BUCKETPOLICY_H
#define BUCKETPOLICY_H

#include <gtest/gtest_prod.h>
#include <algorithm>
#include <array>
#include <tuple>
#include <unordered_map>
#include <vector>

template <typename T>
concept is_less_comparable = requires(T t, T u) {
    { t < u } -> std::same_as<bool>;
};

template <typename TElement, typename... TCallables>
concept is_function_callable_on_element = (std::invocable<TCallables, TElement> && ...);

template <typename TElement, typename... TCallables>
concept is_function_binable_on_doubles =
    is_function_callable_on_element<TElement, TCallables...> &&
    (std::is_convertible_v<std::invoke_result_t<TCallables, TElement>, double> && ...);

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

    template <typename TElement>
        requires is_function_binable_on_doubles<TElement, TCallables...>
    [[nodiscard]] auto getBucket(TElement const &arg)
    {
        return calculateBucketAtIndices(getUpperIndicesForTuple(getValues(arg)));
    }

    [[nodiscard]] int getMaximalBucketCount() const
    {
        return [&]<std::size_t... I>(std::index_sequence<I...>) {
            return (1 * ... * (std::get<I>(mBucketsRanges).size() + 1));
        }(std::make_index_sequence<sizeof...(TCallables)>{});
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

    template <typename... Ts>
    bool checkUnderOverflows(std::tuple<Ts...> arg) const
    {
        return [&arg]<size_t... I>(std::index_sequence<I...>) {
            return ((std::get<I>(arg) == -1) || ...);
        }(std::make_index_sequence<sizeof...(Ts)>{});
    }

    template <typename TElement>
    auto getValues(TElement const &arg)
    {
        return std::make_tuple(std::get<TCallables>(mCallables)(arg)...);
    }

    template <typename... Ts>
    auto getUpperIndicesForTuple(std::tuple<Ts...> const &values)
    {
        return [&]<std::size_t... I>(std::index_sequence<I...>) {
            return std::make_tuple(
                findIndex(std::get<I>(mBucketsRanges), std::get<I>(values), ignoreOverflows)...
            );
        }(std::make_index_sequence<sizeof...(TCallables)>{});
    }

    template <typename... TIndices>
    int calculateBucketAtIndices(std::tuple<TIndices...> const &indices)
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

    std::tuple<TCallables...> mCallables;
    std::array<std::vector<double>, sizeof...(TCallables)> mBucketsRanges;
    bool ignoreOverflows;
};

#endif  // BUCKETPOLICY_H
