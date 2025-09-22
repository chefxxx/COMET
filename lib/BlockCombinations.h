//
// Created by Mateusz Mikiciuk on 11/09/2025.
//

#ifndef BLOCKCOMBINATINOS_H
#define BLOCKCOMBINATINOS_H

#include "Combinations.h"

template <typename TBucketPolicy, typename TCombinationsPolicy, typename... TIters>
struct BlockCombinations {
    BlockCombinations(
        TBucketPolicy bucketPolicy, TCombinationsPolicy combinationsPolicy,
        std::tuple<Ranges<TIters>...>& ranges
    )
        : mCombinationsPolicy(combinationsPolicy),
          mBucketPolicy(bucketPolicy),
          mGroupedData(tuple_transform(ranges, [&](auto&& r) {
              return groupData(r.mBegin, r.mEnd, mBucketPolicy);
          }))
    {
    }

    private:
    TCombinationsPolicy mCombinationsPolicy;
    TBucketPolicy mBucketPolicy;

    public:
    std::tuple<GroupedData<TIters>...> mGroupedData;
};

#endif  // BLOCKCOMBINATINOS_H
