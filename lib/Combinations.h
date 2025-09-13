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
    void addOne()
    {
        constexpr size_t N = sizeof...(TIter);
        bool wasModified   = true;
        [&]<std::size_t... Is>(const std::index_sequence<Is...>&) {
            (addOneFun<N - 1 - Is>(), ...);
        }(std::make_index_sequence<N>());
        mBase.isEnd = wasModified;
    }

    private:
    template <size_t I, size_t N>
    void addOneFun(bool& wasModified)
    {
        if (wasModified) {
            auto it = ++std::get<I>(mBase.mCurrentState);
            if (it != std::get<I>(mBase.mEndState)) {
                [&]<std::size_t... Is>(const std::index_sequence<Is...>&) {
                    (addOneHelper<N - 1 - Is>(), ...);
                }(std::make_index_sequence<I>());
                wasModified = true;
            }
        }
    }

    template <size_t I>
    void addOneHelper()
    {
        std::get<I>(mBase.mCurrentState) = std::get<I>(mBase.mStartState);
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
