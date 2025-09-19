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
    {
        policy.getBucket(*iter)
    } -> std::same_as<int>;
    {
        policy.getMaximalBucketCount()
    } -> std::same_as<int>;
    policy.getValues(*iter);
};

template <typename TIter>
struct GroupedData {
    using IterType = std::unordered_set<int>::iterator;
    std::unordered_map<int, std::vector<TIter>> buckets;
    std::unordered_set<int> bucketsNumbers;

    auto operator[](const int& bucketIdx) const { return buckets[bucketIdx]; }
    [[nodiscard]] size_t size() const
    {
        if (bucketsNumbers.size() == buckets.size())
            return buckets.size();
        return -1;
    }
    void insert(int bucketNo, TIter iter)
    {
        buckets[bucketNo].push_back(iter);
        bucketsNumbers.insert(bucketNo);
    }
    [[nodiscard]] IterType erase(IterType iter)
    {
        buckets.erase(*iter);
        return bucketsNumbers.erase(iter);
    }
    [[nodiscard]] IterType begin() const { return bucketsNumbers.begin(); }
    [[nodiscard]] IterType end() const { return bucketsNumbers.end(); }
};

// TODO: think where we need copies and where we want forward values
template <typename TIter>
struct Ranges {
    Ranges(TIter begin, TIter end) : mBegin(begin), mEnd(end) {}
    TIter mBegin;
    TIter mEnd;
};

template <typename... Ts, typename F>
auto tuple_transform(const std::tuple<Ts...>& tuple, F&& f)
{
    return std::apply(
        [&](auto&&... elems) {
            return std::make_tuple(f(elems)...);
        },
        tuple
    );
}

template <std::forward_iterator TIter, typename TBucketPolicy>
    requires is_bucket_policy<TBucketPolicy, TIter>
[[nodiscard]] auto groupData(
    TIter start, TIter end, TBucketPolicy bucketPolicy, const std::size_t minCatSize = 1
)
{
    GroupedData<TIter> resultData;
    for (auto it = start; it != end; ++it) {
        const int bucketNumber = bucketPolicy.getBucket(*it);
        resultData.insert(bucketNumber, it);
    }

    // We need this second loop,
    // bc in the one above we do not know when we hit last element in bucket.
    for (auto it = resultData.begin(); it != resultData.end();) {
        if (resultData.buckets[*it].size() < minCatSize) {
            it = resultData.erase(it);
        } else {
            ++it;
        }
    }

    return resultData;
}

template <typename TIter0, typename TIterI>
void syncHelper(GroupedData<TIter0>& firstData, GroupedData<TIterI>& comparedData)
{
    auto& [firstBuckets, firstBucketsNumbers] = firstData;
    auto& [buckets, bucketsNumbers]           = comparedData;

    for (auto it = firstBucketsNumbers.begin(); it != firstBucketsNumbers.end();) {
        if (!bucketsNumbers.contains(*it)) {
            firstBuckets.erase(*it);
            it = firstBucketsNumbers.erase(it);
        } else {
            ++it;
        }
    }

    for (auto it = bucketsNumbers.begin(); it != bucketsNumbers.end();) {
        if (!firstBuckets.contains(*it)) {
            buckets.erase(*it);
            it = bucketsNumbers.erase(it);
        } else {
            ++it;
        }
    }
}

template <typename... TIter>
void syncBuckets(std::tuple<GroupedData<TIter>...>& groupedData)
{
    constexpr size_t N = sizeof...(TIter);
    auto& firstData    = std::get<0>(groupedData);
    [&]<std::size_t... Is>(const std::index_sequence<Is...>&) {
        (syncHelper(firstData, std::get<Is>(groupedData)), ...);
    }(std::make_index_sequence<N>());
}

#endif  // HELPERS_H
