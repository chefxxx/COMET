//
// Created by mshamrai on 7/17/25.
//

#ifndef BUCKETPOLICY_H
#define BUCKETPOLICY_H

#include <array>
#include <tuple>
#include <vector>

template <typename TElement, typename... TCallables>
concept is_function_callable_on_element =
    (std::invocable<TCallables, TElement> && ...);

template <typename TElement, typename... TCallables> struct BucketPolicy final {
  BucketPolicy(std::tuple<TCallables...> const &callables,
               std::array<std::vector<double>, sizeof...(TCallables)> const
                   &bucketsRanges,
               const bool ignoreOverflows = true)
      : mCallables(callables), mBucketsRanges(bucketsRanges),
        ignoreOverflows(ignoreOverflows) {}

  auto getBucket(TElement const &arg) {
    auto values = getBucketRangeVal(arg);
    // TODO: define how to find number of a bin
  };

private:
  std::tuple<TCallables...> mCallables;
  std::array<std::vector<double>, sizeof...(TCallables)> mBucketsRanges;
  bool ignoreOverflows;

  auto getBucketRangeVal(TElement const &arg) {
    return std::make_tuple(std::get<TCallables>(mCallables)(arg)...);
  };
};

#endif // BUCKETPOLICY_H
