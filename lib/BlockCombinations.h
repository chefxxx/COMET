//
// Created by Mateusz Mikiciuk on 11/09/2025.
//

#ifndef BLOCKCOMBINATINOS_H
#define BLOCKCOMBINATINOS_H

#include <cassert>
#include "Combinations.h"

template <typename TBucketPolicy, typename TCombinationsPolicy, typename... TIters>
struct BlockCombinations {
    using CombinationsType = std::tuple<TIters...>;
    using BucketIterType   = std::unordered_set<int>::const_iterator;

    BlockCombinations(
        TBucketPolicy bucketPolicy, TCombinationsPolicy combinationsPolicy,
        std::tuple<Ranges<TIters>...>& ranges
    )
        : mGroupedData(tuple_transform(
              ranges,
              [&](auto&& r) {
                  return groupData(r.mBegin, r.mEnd, bucketPolicy);
              }
          )),
          mCombinationsPolicy(combinationsPolicy),
          mBucketPolicy(bucketPolicy)
    {
        /* Now in each GroupedData<> object set of buckets numbers is the same.
         * So just take from first one begin() and end(). */
        mCurrent = std::get<0>(mGroupedData).begin();
        mEnd     = std::get<0>(mGroupedData).end();
        setCombinations(std::make_index_sequence<sizeof...(TIters)>{}, mCurrent);
    }

    /* Here we have GroupedData templated with TIter type, but inside this struct we store
     * std::unordered_map<int, std::vector<TIter>> - so we have vector of iterators */
    std::tuple<GroupedData<TIters>...> mGroupedData;

    CombinationsType moveForward()
    {
        assert(mCurrent != mEnd);
        return mCombinationsPolicy.state();
    }

    private:
    TCombinationsPolicy mCombinationsPolicy;
    TBucketPolicy mBucketPolicy;
    BucketIterType mCurrent;
    BucketIterType mEnd;

    template <typename TIter>
    auto createRanges(const GroupedData<TIter>& data, BucketIterType current)
    {
        auto bucket = data[current];
        return Ranges(bucket.begin(), bucket.end());
    }

    template <std::size_t... Is>
    void setCombinations(const std::index_sequence<Is...>&, BucketIterType current)
    {
        auto bucketsRanges = tuple_transform(mGroupedData, [&](auto&& data) {
            return createRanges(data, current);
        });
        mCombinationsPolicy.setData(bucketsRanges);
    }
};

#endif  // BLOCKCOMBINATINOS_H
