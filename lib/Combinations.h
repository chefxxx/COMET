//
// Created by Mateusz Mikiciuk on 22/07/2025.
//

#ifndef COMBINATIONS_H
#define COMBINATIONS_H

#include <ios>
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
        : mBeginState(std::apply(
              [](auto const&... r) {
                  return std::make_tuple(r.mBegin...);
              },
              ranges
          )),
          mCurrentState(std::apply(
              [](auto const&... r) {
                  return std::make_tuple(r.mBegin...);
              },
              ranges
          )),
          mEndState(std::apply(
              [](auto const&... r) {
                  return std::make_tuple(r.mEnd...);
              },
              ranges
          ))
    {
        // Indices for positions into mCurrentState tuple for manipulating inside the addOne()
        rightIndex = sizeof...(TIter) - 1;
    }
    int rightIndex;
    IteratorType mBeginState;
    IteratorType mCurrentState;
    IteratorType mEndState;
    bool isEnd;
};

template <typename... TIter>
CombinationsPolicyBase(std::tuple<Ranges<TIter>...>&) -> CombinationsPolicyBase<TIter...>;

template <typename... TIter>
struct FullCombinationsPolicy {
    CombinationsPolicyBase<TIter...> mBase;
    explicit FullCombinationsPolicy(std::tuple<Ranges<TIter>...>& ranges) : mBase(ranges) {}
    // TODO: This function is partially done!!!
    // It is important to make one loop more, to check if we can add one to the left side
    void addOne()
    {
        if (!mBase.isEnd) {
            constexpr int right = mBase.rightIndex;
            // Add one to the right index
            auto iterRight = ++std::get<right>(mBase.mCurrentState);

            if (std::get<right>(mBase.mCurrentState) == mBase.mEndState) {
                right = --mBase.rightIndex;
            }

            // TODO: Find how to change this for to compile time
            for (int i = right + 1, len = sizeof...(TIter); i < len; ++i) {
                std::get<i>(mBase.mCurrentState) = std::get<i>(mBase.mBeginState);
            }
        }
    }
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
