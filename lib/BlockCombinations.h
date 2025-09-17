//
// Created by Mateusz Mikiciuk on 11/09/2025.
//

#ifndef BLOCKCOMBINATINOS_H
#define BLOCKCOMBINATINOS_H

#include "Combinations.h"

template <typename TBucketPolicy, typename TCombinationsPolicy, typename... TIter>
struct BlockCombinations {
    public:
    BlockCombinations(TBucketPolicy bucketPolicy, TCombinationsPolicy combinationsPolicy, std::tuple<Ranges<TIter>...>& ranges)
        : mBucketPolicy(bucketPolicy), mCombinationsPolicy(combinationsPolicy)
    {

    }

    private:
    std::tuple<GroupedData<TIter...>> mGroupedData;
    TCombinationsPolicy mCombinationsPolicy;
    TBucketPolicy mBucketPolicy;
};

#endif  // BLOCKCOMBINATINOS_H
