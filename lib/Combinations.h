//
// Created by Mateusz Mikiciuk on 22/07/2025.
//

#ifndef COMBINATIONS_H
#define COMBINATIONS_H

#include <tuple>

template <typename... TData>
struct CombinationsPolicyBase {
    public:
    CombinationsPolicyBase(TData &...data) {}

    private:
    std::tuple<typename TData::iterator...> mCurrentState;
};

template <typename... TData>
struct FullCombinationsPolicy : CombinationsPolicyBase<TData...> {
};

template <typename... TData>
struct StrictlyUpperCombinationsPolicy : CombinationsPolicyBase<TData...> {
};

#endif  // COMBINATIONS_H
