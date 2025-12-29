//
// Created by Mateusz Mikiciuk on 18/07/2025.
//

#ifndef HELPERS_H
#define HELPERS_H

#include <iterator>
#include <set>
#include <unordered_map>
#include "BucketPolicy.h"

#if 0

// TODO: rework of grouped buckets
template <std::forward_iterator TIter>
struct GroupedBuckets {
    using bucket_type = std::set<int>::const_iterator;

    const std::vector<TIter> &at(bucket_type t_iter) const { return m_buckets.at(*t_iter); }
    const std::vector<TIter> &at(const int t_bucketNo) const { return m_buckets.at(t_bucketNo); }

    [[nodiscard]] size_t size() const { return m_bucketsNumbers.size(); }

    void insert(int t_bucketNo, TIter t_iter)
    {
        m_buckets[t_bucketNo].push_back(t_iter);
        m_bucketsNumbers.insert(t_bucketNo);
    }

    [[nodiscard]] bucket_type erase(bucket_type t_iter)
    {
        m_buckets.erase(*t_iter);
        return m_bucketsNumbers.erase(t_iter);
    }

    [[nodiscard]] bool contains(const int t_bucketNo) const
    {
        return m_bucketsNumbers.contains(t_bucketNo);
    }

    [[nodiscard]] bucket_type begin() const { return m_bucketsNumbers.begin(); }
    [[nodiscard]] bucket_type end() const { return m_bucketsNumbers.end(); }

    private:
    std::unordered_map<int, std::vector<TIter>> m_buckets;
    std::set<int> m_bucketsNumbers;
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
auto tupleTransform(const std::tuple<Types...> &t_tuple, Func &&t_fn)
{
    return std::apply(
        [&]<typename... InnerTypes>(InnerTypes &&...elem) {
            return std::make_tuple(t_fn(std::forward<InnerTypes>(elem))...);
        },
        t_tuple
    );
}

#endif

#endif  // HELPERS_H
