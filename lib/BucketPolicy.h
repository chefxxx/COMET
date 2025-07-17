//
// Created by mshamrai on 7/17/25.
//

#ifndef BUCKETPOLICY_H
#define BUCKETPOLICY_H

#include <vector>
#include <tuple>
#include <array>

template <typename... TCallables>
struct BucketPolicy final{
    BucketPolicy(std::tuple<TCallables...> const& callables, std::array<std::vector<double>, sizeof...(TCallables)> const& bucketsRanges, bool ignoreOverflows = true):
        mCallables(callables), mBucketsRanges(mBucketsRanges), ignoreOverflows(ignoreOverflows){}

    template <typename T>
    auto getBucket(T const& arg) {
        auto values = getBucketRangeVal(arg);
        // TODO: define how to find number of bin

    };
private:
    std::tuple<TCallables...> mCallables;
    std::array<std::vector<double>, sizeof...(TCallables)> mBucketsRanges;
    bool ignoreOverflows;

    template <typename T>
    auto getBucketRangeVal(T const& arg) {
        return std::make_tuple(std::get<TCallables>(mCallables)(arg)...);
    };
};


#endif //BUCKETPOLICY_H
