//
// Created by mshamrai on 7/17/25.
//

#ifndef BUCKETPOLICY_H
#define BUCKETPOLICY_H

#include <string>

template <typename... TCallables>
struct BucketPolicy {
    BucketPolicy(std::tuple<TCallables...> callables, std::array<std::vector<double>, sizeof...(TCallables)> bucketsRanges) {}

private:
    std::tuple<TCallables...> mCallables;
    std::array<std::vector<double>, sizeof...(TCallables)> mBucketsRanges;

    auto getBucket();
    auto getBucketRangeVal();
};


#endif //BUCKETPOLICY_H
