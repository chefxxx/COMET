//
// Created by Mateusz Mikiciuk on 22/07/2025.
//

#ifndef COMBINATIONS_H
#define COMBINATIONS_H

#include <tuple>

template <typename TIter>
struct Ranges {
    TIter begin;
    TIter end;
};

template <typename... TIter>
struct CombinationsPolicyBase {
    using IteratorType = typename std::tuple<TIter...>;
    explicit CombinationsPolicyBase(std::tuple<Ranges<TIter>...>& ranges)
        : mCurrentState(
              std::apply(
                  [](auto const&... r) {
                      std::make_tuple(r.begin...);
                  },
                  ranges
              )
          ),
          mEndState(
              std::apply(
                  [](auto const&... r) {
                      std::make_tuple(r.end...);
                  },
                  ranges
              )
          )
    {
    }

    IteratorType mCurrentState;
    IteratorType mEndState;
};

template <typename... TIter>
struct FullCombinationsPolicy : CombinationsPolicyBase<TIter...> {
};

template <typename... TIter>
struct StrictlyUpperCombinationsPolicy : CombinationsPolicyBase<TIter...> {
};

#endif  // COMBINATIONS_H
