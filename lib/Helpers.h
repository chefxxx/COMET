//
// Created by Mateusz Mikiciuk on 18/07/2025.
//

#ifndef HELPERS_H
#define HELPERS_H

#include <iterator>
#include <unordered_map>
#include <unordered_set>
#include "BucketPolicy.h"

template <typename TBucketPolicy, typename TIter>
concept is_bucket_policy = requires(TBucketPolicy policy, TIter iter) {
    { policy.getBucket(*iter) } -> std::same_as<int>;
    { policy.getMaximalBucketCount() } -> std::same_as<int>;
    policy.getValues(*iter);
};

template <typename TIter>
struct BucketIdx {
    BucketIdx(const int bucketIdx, const int dataIdx, TIter iter)
        : mBucketIdx(bucketIdx), mDataIdx(dataIdx), mIter(iter)
    {
    }

    int mBucketIdx;
    int mDataIdx;
    TIter mIter;
};

template <typename TIter>
struct GroupedData {
    std::unordered_map<int, std::vector<BucketIdx<TIter>>> buckets;
    std::unordered_set<int> bucketsNumbers;
};

template <std::forward_iterator TIter, typename TBucketPolicy>
    requires is_bucket_policy<TBucketPolicy, TIter>
[[nodiscard]] auto groupData(
    TIter start, TIter end, TBucketPolicy bucketPolicy, const std::size_t minCatSize = 1
)
{
    int dataIdx = 0;
    GroupedData<TIter> resultData;
    for (auto it = start; it != end; ++it) {
        const auto bucketNumber = bucketPolicy.getBucket(*it);
        BucketIdx<TIter> bucketIdx{bucketNumber, dataIdx++, it};
        resultData.buckets[bucketNumber].push_back(bucketIdx);
        resultData.bucketsNumbers.insert(bucketNumber);
    }

    // We need this second loop,
    // bc in the one above we do not know when we hit last element in bucket.
    for (auto it = resultData.bucketsNumbers.begin(); it != resultData.bucketsNumbers.end(); ) {
        if (resultData.buckets[*it].size() < minCatSize) {
            resultData.buckets.erase(*it);
            it = resultData.bucketsNumbers.erase(it);
        }
        else {
            ++it;
        }
    }

    return resultData;
}

template <std::size_t N, typename TIter>
void syncBuckets(
    std::array<std::unordered_map<int, std::vector<BucketIdx<TIter>>>, N> const& groupedData
);

#endif  // HELPERS_H
