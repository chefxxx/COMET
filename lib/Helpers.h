//
// Created by Mateusz Mikiciuk on 18/07/2025.
//

#ifndef HELPERS_H
#define HELPERS_H

#include <iterator>
#include <map>
#include "BucketPolicy.h"

template<typename TBucketPolicy, typename TIter>
concept is_bucket_policy = requires(TBucketPolicy policy, TIter iter) {
    { policy.getBucket(*iter) } -> std::same_as<int>;
};

template<typename TIter>
struct BucketIdx {
    BucketIdx(const int dataIdx, TIter iter) : mDataIdx(dataIdx), mIter(iter) {}

    int mDataIdx;
    TIter mIter;
};

template<std::forward_iterator TIter, typename TBucketPolicy>
    requires is_bucket_policy<TBucketPolicy, TIter>
[[nodiscard]] auto groupData(TIter start, TIter end, TBucketPolicy bucketPolicy, const int outsider = -1,
                             const int minCatSize = -1) {
    int dataIdx = 0;
    std::map<int, std::vector<BucketIdx<TIter>>> buckets;
    for (auto it = start; it != end; ++it) {
        const int bucketNumber = bucketPolicy.getBucket(*it);
        if (bucketNumber != outsider) {
            buckets[bucketNumber].emplace_back(dataIdx++, it);
        }
    }

    // TODO: here we have to sort data in buckets
    // TODO: then select common categories
    return buckets;
}

#endif // HELPERS_H
