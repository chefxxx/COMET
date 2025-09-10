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
    std::unordered_set<int> tooSmallBuckets;
    for (const auto& num : resultData.bucketsNumbers) {
        if (resultData.buckets[num].size() < minCatSize) {
            resultData.buckets.erase(num);
            tooSmallBuckets.insert(num);
        }
    }

    // Furthermore we need this to remove buckets that do not
    // meet the requirement of size (minCatSize). It is so clumsy,
    // bc std::unordered_set does not have operator[] and cannot be
    // modified while above loop takes place.
    for (const auto& num : tooSmallBuckets) {
        resultData.bucketsNumbers.erase(num);
    }
    return resultData;
}

template <std::size_t N, typename TIter>
void syncBuckets(
    std::array<std::unordered_map<int, std::vector<BucketIdx<TIter>>>, N> const& groupedData
);

#endif  // HELPERS_H
