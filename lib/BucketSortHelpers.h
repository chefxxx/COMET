//
// Created by mshamrai on 12/28/25.
//

#ifndef COMET_BUCKETSORTHELPERS_H
#define COMET_BUCKETSORTHELPERS_H
#include <map>
#include <set>
#include <unordered_map>

template <typename BucketType, typename T, typename Callable>
std::set<BucketType> getAvailableBuckets(T container, Callable callable)
{
    std::set<BucketType> availableBuckets = std::set<BucketType>();
    for (auto element : container) {
        availableBuckets.insert(callable(element));
    }
    return availableBuckets;
}

template <typename BucketType, typename T, typename Callable>
std::map<BucketType, int> getAmountsInAvailableBuckets(T container, Callable callable,
    std::set<BucketType> availableBuckets)
{
    std::map<BucketType, int> availableBucketsWithAmounts = std::map<BucketType, int>();
    for (auto bucket : availableBuckets) {
        availableBucketsWithAmounts.insert(callable(bucket), 0);
    }
    for (auto element : container) {
        ++availableBucketsWithAmounts[callable(element)];
    }
    return availableBucketsWithAmounts;
}

template <typename BucketType>
std::unordered_map<BucketType, int> getMappingFromBucketsToIndices(std::set<BucketType> availableBuckets){
    std::unordered_map<BucketType, int> mapping = std::unordered_map<BucketType, int>(availableBuckets.size());
    int i = 0;
    for (auto bucket : availableBuckets) {
        mapping[bucket] = i;
        ++i;
    }
    return mapping;
}



#endif  // COMET_BUCKETSORTHELPERS_H
