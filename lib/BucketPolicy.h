//
// Created by mshamrai on 7/17/25.
//

#ifndef BUCKETPOLICY_H
#define BUCKETPOLICY_H

#include <array>
#include <tuple>
#include <utility>
#include <vector>

template<typename T>
concept is_less_comparable = requires(T t, T u) {
    { t < u } -> std::same_as<bool>;
};

template<typename TElement, typename... TCallables>
concept is_function_callable_on_element = (std::invocable<TCallables, TElement> && ...);

template<typename TElement, typename... TCallables>
concept is_function_binable_on_doubles =
        is_function_callable_on_element<TElement, TCallables...> &&
        (std::is_convertible_v<std::invoke_result_t<TCallables, TElement>, double> && ...);

template<typename T>
    requires is_less_comparable<T>
int findUpperIndex(std::vector<T> const &data, T const &value) {
    return static_cast<int>(distance(data.begin(), std::upper_bound(data.begin(), data.end(), value)));
}

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
        return [&]<std::size_t... I>(std::index_sequence<I...>) {
            return std::make_tuple(findUpperIndex(std::get<I>(mBucketsRanges), std::get<I>(values))...);
        }(std::make_index_sequence<sizeof...(TCallables)>{});
    }

private:
    std::tuple<TCallables...> mCallables;
    std::array<std::vector<double>, sizeof...(TCallables)> mBucketsRanges;
    bool ignoreOverflows;
};

#endif // BUCKETPOLICY_H
