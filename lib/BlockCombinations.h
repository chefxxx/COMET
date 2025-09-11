//
// Created by Mateusz Mikiciuk on 11/09/2025.
//

#ifndef BLOCKCOMBINATINOS_H
#define BLOCKCOMBINATINOS_H

#include "Combinations.h"
#include "Helpers.h"
// Why block combinations and why it is in another file?
template <typename... TData>
struct BlockCombinations {
    public:
    BlockCombinations(TData &...data) {}

    private:
    std::tuple<GroupedData<typename TData::iterator...>> mGroupedData;
    CombinationsPolicyBase<TData...> mCombinationsPolicy;
};

#endif  // BLOCKCOMBINATINOS_H
