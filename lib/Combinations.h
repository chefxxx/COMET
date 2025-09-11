//
// Created by Mateusz Mikiciuk on 22/07/2025.
//

#ifndef COMBINATIONS_H
#define COMBINATIONS_H

#include <tuple>

template <typename TIter1, typename TIter2, typename... TRest>
struct IteratorType {
    using type = typename IteratorType<TIter1, TRest...>::type;
};

template <typename... TIter>
struct CombinationsPolicyBase {
    using CombinationsType = IteratorType<TIter...>::type;
    public:
    CombinationsPolicyBase(TIter... iterators) : mCurrentState() {}

    private:
    std::tuple<CombinationsType> mCurrentState;
    std::tuple<CombinationsType> mEndState;
};

template <typename... TIter>
struct FullCombinationsPolicy : CombinationsPolicyBase<TIter...> {
};

template <typename... TIter>
struct StrictlyUpperCombinationsPolicy : CombinationsPolicyBase<TIter...> {
};

#endif  // COMBINATIONS_H
