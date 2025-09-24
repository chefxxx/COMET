//
// Created by Mateusz Mikiciuk on 11/09/2025.
//

#ifndef BLOCKCOMBINATINOS_H
#define BLOCKCOMBINATINOS_H


#include "Combinations.h"

template <
    typename TBucketPolicy, typename TCombinationsPolicy, typename TCombinationsType,
    typename... TIters>
struct BlockCombinations {
    using BucketIterType = std::unordered_set<int>::const_iterator;

    BlockCombinations(
        const TBucketPolicy& bucketPolicy, const TCombinationsPolicy& combinationsPolicy,
        const std::tuple<Ranges<TIters>...>& ranges
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
        syncBuckets(mGroupedData);
        mCurrent = std::get<0>(mGroupedData).begin();
        mEnd     = std::get<0>(mGroupedData).end();
        setCombinations(std::make_index_sequence<sizeof...(TIters)>{}, mCurrent);
    }

    void addOne()
    {
        /* Case where we have to change buckets */
        if (mCombinationsPolicy.isEnd()) {
            ++mCurrent;
            if (mCurrent != mEnd) {
                setCombinations(std::make_index_sequence<sizeof...(TIters)>{}, mCurrent);
            }
        }
        else {
            mCombinationsPolicy.addOne();
        }
    }

    private:
    std::tuple<GroupedData<TIters>...> mGroupedData;
    TCombinationsPolicy mCombinationsPolicy;
    TBucketPolicy mBucketPolicy;
    BucketIterType mCurrent;
    BucketIterType mEnd;

    template <typename TIter>
    auto createRanges(const GroupedData<TIter>& data, BucketIterType current)
    {
        auto bucket = data.at(current);
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

template <typename TBucketPolicy, typename... TIters>
struct BlockFullCombinations {
    using CombinationsType = std::tuple<typename std::vector<TIters>::iterator...>;
    using PolicyType       = FullCombinationsPolicy<typename std::vector<TIters>::iterator...>;
    PolicyType mCombinationsPolicy{};

    BlockFullCombinations(
        const TBucketPolicy& bucketPolicy, const std::tuple<Ranges<TIters>...>& ranges
    )
        : mBase(bucketPolicy, mCombinationsPolicy, ranges)
    {
    }

    BlockCombinations<TBucketPolicy, PolicyType, CombinationsType, TIters...> mBase;

    auto data() { return mBase.mGroupedData; }
};

#endif  // BLOCKCOMBINATINOS_H
