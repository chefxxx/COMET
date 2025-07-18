//
// Created by mshamrai on 7/17/25.
//

#ifndef BUCKETPOLICY_H
#define BUCKETPOLICY_H

#include <array>
#include <tuple>
#include <vector>

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
    [[nodiscard]] int getBucket(TElement const &arg) {
        auto values = getBucketRangeVal(arg);
        // TODO: define how to find number of a bin
        return 1;
    };

private:
    std::tuple<TCallables...> mCallables;
    std::array<std::vector<double>, sizeof...(TCallables)> mBucketsRanges;
    bool ignoreOverflows;

    template<typename TElement>
    auto getBucketRangeVal(TElement const &arg) {
        return std::make_tuple(std::get<TCallables>(mCallables)(arg)...);
    };
};

#endif // BUCKETPOLICY_H
