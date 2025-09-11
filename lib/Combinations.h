//
// Created by Mateusz Mikiciuk on 22/07/2025.
//

#ifndef COMBINATIONS_H
#define COMBINATIONS_H

#include <tuple>

// TODO: think where we need copies and where we want forward values
template <typename TIter>
struct Ranges {
    Ranges(TIter begin, TIter end) : mBegin(begin), mEnd(end) {}
    TIter mBegin;
    TIter mEnd;
};

template <typename... TIter>
struct CombinationsPolicyBase {
    using IteratorType = std::tuple<TIter...>;
    explicit CombinationsPolicyBase(std::tuple<Ranges<TIter>...>& ranges)
        : mCurrentState(
              std::apply(
                  [](auto const&... r) {
                      return std::make_tuple(r.mBegin...);
                  },
                  ranges
              )
          ),
          mEndState(
              std::apply(
                  [](auto const&... r) {
                      return std::make_tuple(r.mEnd...);
                  },
                  ranges
              )
          )
    {
    }

    protected:
    IteratorType mCurrentState;
    IteratorType mEndState;
};

template <typename... TIter>
CombinationsPolicyBase(std::tuple<Ranges<TIter>...>&) -> CombinationsPolicyBase<TIter...>;

template <typename... TIter>
struct FullCombinationsPolicy {
    CombinationsPolicyBase<TIter...> mBase;
    explicit FullCombinationsPolicy(std::tuple<Ranges<TIter>...>& ranges) : mBase(ranges) {}

    void addOne() {}
};

template <typename... TIter>
struct StrictlyUpperCombinationsPolicy {
    CombinationsPolicyBase<TIter...> mBase;
    explicit StrictlyUpperCombinationsPolicy(std::tuple<Ranges<TIter>...>& ranges) : mBase(ranges)
    {
    }

    void addOne() {}
};

#endif  // COMBINATIONS_H
