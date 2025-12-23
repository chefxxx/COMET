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
    using BucketIterType = std::set<int>::const_iterator;

    const std::vector<TIter> &at(BucketIterType t_iter) const { return m_Buckets.at(*t_iter); }
    const std::vector<TIter> &at(const int t_bucketNo) const { return m_Buckets.at(t_bucketNo); }

    [[nodiscard]] size_t size() const { return m_BucketsNumbers.size(); }

    void insert(int t_bucketNo, TIter t_iter)
    {
        m_Buckets[t_bucketNo].push_back(t_iter);
        m_BucketsNumbers.insert(t_bucketNo);
    }

    [[nodiscard]] BucketIterType erase(BucketIterType t_iter)
    {
        m_Buckets.erase(*t_iter);
        return m_BucketsNumbers.erase(t_iter);
    }

    [[nodiscard]] bool contains(const int t_bucketNo) const
    {
        return m_BucketsNumbers.contains(t_bucketNo);
    }

    [[nodiscard]] BucketIterType begin() const { return m_BucketsNumbers.begin(); }
    [[nodiscard]] BucketIterType end() const { return m_BucketsNumbers.end(); }

    private:
    std::unordered_map<int, std::vector<TIter>> m_Buckets;
    std::set<int> m_BucketsNumbers;
};

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

template <typename... Types, typename Func>
auto tupleTransform(const std::tuple<Types...> &tuple, Func &&f)
{
    return std::apply(
        [&]<typename... InnerTypes>(InnerTypes &&...elem) {
            return std::make_tuple(f(std::forward<InnerTypes>(elem))...);
        },
        tuple
    );
}

#endif  // HELPERS_H
