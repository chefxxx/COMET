//
// Created by Mateusz Mikiciuk on 18/07/2025.
//

#ifndef HELPERS_H
#define HELPERS_H

#include <iterator>
#include <set>
#include <unordered_map>
#include "BucketPolicy.h"

template <std::forward_iterator TIter>
struct GroupedBuckets {
    using IterType = std::set<int>::const_iterator;

    const std::vector<TIter> &at(IterType iter) const { return mBuckets.at(*iter); }
    const std::vector<TIter> &at(const int bucketNo) const { return mBuckets.at(bucketNo); }

    [[nodiscard]] size_t size() const { return mBucketsNumbers.size(); }

    void insert(int bucketNo, TIter iter)
    {
        mBuckets[bucketNo].push_back(iter);
        mBucketsNumbers.insert(bucketNo);
    }

    [[nodiscard]] IterType erase(IterType iter)
    {
        mBuckets.erase(*iter);
        return mBucketsNumbers.erase(iter);
    }

    [[nodiscard]] bool contains(const int bucketNo) const
    {
        return mBucketsNumbers.contains(bucketNo);
    }

    [[nodiscard]] IterType begin() const { return mBucketsNumbers.begin(); }
    [[nodiscard]] IterType end() const { return mBucketsNumbers.end(); }

    private:
    std::unordered_map<int, std::vector<TIter>> mBuckets;
    std::set<int> mBucketsNumbers;
};

template <typename... Types, typename Func>
auto tupleTransform(const std::tuple<Types...> &tuple, Func &&f)
{
    return std::apply(
        [&](auto &&...elem) {
            return std::make_tuple(f(elem)...);
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
    GroupedBuckets<TIter> resultData;
    for (auto it = start; it != end; ++it) {
        const int bucketNumber = bucketPolicy.getBucket(*it);
        resultData.insert(bucketNumber, it);
    }

    // We need this second loop,
    // bc in the one above we do not know when we hit last element in bucket.
    for (auto it = resultData.begin(); it != resultData.end();) {
        if (resultData.at(it).size() < minCatSize) {
            it = resultData.erase(it);
        } else {
            ++it;
        }
    }
    return resultData;
}

template <std::forward_iterator TIter1, std::forward_iterator TIter2>
void syncHelper(GroupedBuckets<TIter1> &firstData, const GroupedBuckets<TIter2> &comparedData)
{
    for (auto it = firstData.begin(); it != firstData.end();) {
        if (!comparedData.contains(*it)) {
            it = firstData.erase(it);
        } else {
            ++it;
        }
    }
}

template <std::forward_iterator... TIters>
void syncBuckets(std::tuple<GroupedBuckets<TIters>...> &groupedData)
{
    constexpr size_t N = sizeof...(TIters);
    auto &firstData    = std::get<0>(groupedData);
    [&]<std::size_t... Is>(const std::index_sequence<Is...> &) {
        (syncHelper(firstData, std::get<Is>(groupedData)), ...);
        (syncHelper(std::get<Is>(groupedData), firstData), ...);
    }(std::make_index_sequence<N>());
}

#endif  // HELPERS_H
