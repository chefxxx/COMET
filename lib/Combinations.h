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
    }
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

    // N - number of data sources
    // I - which position from the right side is considered
    // J - loop iterator, which pointer is set to 0 from the N - I to right position
    template <size_t I, size_t J, size_t N>
    void addOneHelper()
    {
        // Clang format makes it look very strange
        constexpr auto ind                 = N - I + J;
        std::get<ind>(mBase.mCurrentState) = std::get<ind>(mBase.mBeginState);
    }
};

template <typename... TIter>
struct StrictlyUpperCombinationsPolicy {
    CombinationsPolicyBase<TIter...> mBase;
    explicit StrictlyUpperCombinationsPolicy(std::tuple<Ranges<TIter>...>& ranges)
        : mBase(ranges),
          mCurrentIndices(std::apply(
              [](auto const&... r) {
                  return std::array<int64_t, sizeof...(TIter)>{[](const auto&) {
                      return 0;
                  }(r)...};
              },
              ranges
          )),
          mEndIndices(std::apply(
              [](auto const&... r) {
                  return std::array<int64_t, sizeof...(TIter)>{[](const auto& arg) {
                      return std::distance(arg.mBegin, arg.mEnd);
                  }(r)...};
              },
              ranges
          ))
    {
        // Set ranges here have the same logic as a loop into addOneFun with setting new
        // pointers for right side positions.
        constexpr auto N = sizeof...(TIter);
        setRanges<N - 1, N>();
    }

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
            bool wasChanged    = true;
            constexpr auto ind = N - I - 1;
            auto it            = ++std::get<ind>(mBase.mCurrentState);
            ++mCurrentIndices[ind];
            if (it != std::get<ind>(mBase.mEndState)) {
                [&]<std::size_t... Is>(const std::index_sequence<Is...>&) {
                    (addOneHelper<I, Is, N>(wasChanged), ...);
                }(std::make_index_sequence<I>());
                wasModified = !wasChanged;
            }
        }
    }

    // Here there are changes from the Full version. The pointers must be set not to 0, but to
    // Position + 1 from the nearest left pointer.
    // N - number of data sources
    // I - which position from the right side is considered
    // J - loop iterator, which pointer is set to 0 from the N - I to right position
    template <size_t I, size_t J, size_t N>
    void addOneHelper(bool& wasChanged)
    {
        if (wasChanged) {
            // Clang format makes it look very strange
            constexpr auto ind = N - I + J;
            int64_t tmpInd     = mCurrentIndices[ind - 1] + 1;
            if (tmpInd < mEndIndices[ind]) {
                std::get<ind>(mBase.mCurrentState) = std::get<ind>(mBase.mBeginState) + tmpInd;
                mCurrentIndices[ind]               = tmpInd;
            } else {
                wasChanged = false;
            }
        }
    }

    // TODO: I consider strong refactor here, I don't like this solution, there are a lot of
    // TODO: code repetition and it is very hardly understandable
    template <size_t I, size_t N>
    void setRanges()
    {
        bool wasChanged = true;
        [&]<std::size_t... Is>(const std::index_sequence<Is...>&) {
            (addOneHelper<I, Is, N>(wasChanged), ...);
        }(std::make_index_sequence<I>());
    }

    // TODO: here I need to know distances between iterators and I want have additional structure
    // TODO: to store current indices.

    std::array<int64_t, sizeof...(TIter)> mCurrentIndices;
    std::array<int64_t, sizeof...(TIter)> mEndIndices;
};

#endif  // COMBINATIONS_H
