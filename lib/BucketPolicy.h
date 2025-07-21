//
// Created by mshamrai on 7/17/25.
//

#ifndef BUCKETPOLICY_H
#define BUCKETPOLICY_H

#include <array>
#include <utility>
#include <tuple>
#include <vector>

template <typename T>
concept type_with_less_operator = requires(T t, T u)
{
    { t < u } -> std::same_as<bool>;
};

template <type_with_less_operator T>
inline int findUpperIndex(std::vector<T> const& data, T const& value) {
    return static_cast<int>(distance(data.begin(), std::upper_bound(data.begin(), data.end(), value)));
}

template<typename TElement, typename... TCallables>
concept is_function_callable_on_element = (std::invocable<TCallables, TElement> && ...);

template<typename TElement, typename... TCallable>
concept is_function_binable_on_doubles =
        is_function_callable_on_element<TElement, TCallable...> &&
        (std::is_convertible_v<std::invoke_result_t<TCallable, TElement>, double> && ...);

template<typename... TCallables>
struct BucketPolicy final {
    BucketPolicy(std::tuple<TCallables...> const &callables,
                 std::array<std::vector<double>, sizeof...(TCallables)> const &bucketsRanges,
                 const bool ignoreOverflows = true) :
        mCallables(callables), mBucketsRanges(bucketsRanges), ignoreOverflows(ignoreOverflows) {}

    template<typename TElement>
        requires is_function_binable_on_doubles<TElement, TCallables...>
    [[nodiscard]] auto getBucket(TElement const &arg) {
        auto values = std::make_tuple(std::get<TCallables>(mCallables)(arg)...);
        return [&]<std::size_t... I>(std::index_sequence<I...>)
            {
                return std::make_tuple(findUpperIndex(std::get<I>(mBucketsRanges), std::get<I>(values))...);
            }(std::make_index_sequence<sizeof...(TCallables)>{});
    }

private:
    template<typename... TIndices>
    int calculateBucketAtIndices(std::tuple<TIndices...> const& indices) {
        constexpr int N = sizeof...(TIndices);
        auto indexSeq = std::make_index_sequence<N>();
        auto res = [&]<size_t... I, size_t... S>(std::index_sequence<I...> first, std::index_sequence<S...> second)
        {
        };
    }

    std::tuple<TCallables...> mCallables;
    std::array<std::vector<double>, sizeof...(TCallables)> mBucketsRanges;
    bool ignoreOverflows;

    template<typename TElement>
    auto getBucketRangeVal(TElement const &arg) {
        return std::make_tuple(std::get<TCallables>(mCallables)(arg)...);
    };
};

#endif // BUCKETPOLICY_H
