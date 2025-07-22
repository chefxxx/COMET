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
    BucketIdx(const int dataIdx, TIter iter, const int bucketIdx) :
        mBucketIdx(bucketIdx), mDataIdx(dataIdx), mIter(iter) {}

    int mBucketIdx;
    int mDataIdx;
    TIter mIter;
};

template<std::forward_iterator TIter, typename TBucketPolicy>
    requires is_bucket_policy<TBucketPolicy, TIter>
[[nodiscard]] auto groupData(TIter start, TIter end, TBucketPolicy bucketPolicy, const int minCatSize = 1,
                             const int outsider = -1) {
    int dataIdx = 0;
    std::map<int, std::vector<BucketIdx<TIter>>> buckets;
    for (auto it = start; it != end; ++it) {
        const auto bucketNumber = bucketPolicy.getBucket(*it);
        buckets[bucketNumber].emplace_back(dataIdx++, it, bucketNumber);
    }

    std::vector<BucketIdx<TIter>> resultData;
    for (const auto &b: buckets) {
        if (b.second.size() >= minCatSize && b.first != outsider) {
            resultData.insert(resultData.end(), b.second.begin(), b.second.end());
        }
    }
    return resultData;
}

#endif // HELPERS_H
