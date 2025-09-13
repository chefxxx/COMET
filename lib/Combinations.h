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
    bool isEnd = false;
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
            (addOneFun<Is, N>(wasModified), ...);
        }(std::make_index_sequence<N>());
        mBase.isEnd = wasModified;
    }

    private:
    template <size_t I, size_t N>
    void addOneFun(bool& wasModified)
    {
        if (wasModified) {
            constexpr auto ind = N - I - 1;
            auto it            = ++std::get<ind>(mBase.mCurrentState);
            if (it != std::get<ind>(mBase.mEndState)) {
                [&]<std::size_t... Is>(const std::index_sequence<Is...>&) {
                    (addOneHelper<I, Is, N>(), ...);
                }(std::make_index_sequence<I>());
                wasModified = false;
            }
        }
    }

    template <size_t I, size_t J, size_t N>
    void addOneHelper()
    {
        constexpr auto ind                 = N - I + J;
        std::get<ind>(mBase.mCurrentState) = std::get<ind>(mBase.mBeginState);
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
