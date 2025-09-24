//
// Created by Mateusz Mikiciuk on 22/07/2025.
//

#ifndef COMBINATIONS_H
#define COMBINATIONS_H

#include <tuple>
#include "Helpers.h"

template <typename... TIters>
struct CombinationsPolicyBase {
    using CombinationsType   = std::tuple<TIters...>;
    CombinationsPolicyBase() = default;

    template <size_t I, typename TIter>
    void setDataHelper(Ranges<TIter>& range)
    {
        std::get<I>(mCurrentState) = range.mBegin;
        mEndIndexNumbers[I]        = std::distance(range.mBegin, range.mEnd);
        mCurrentIndexNumbers[I]    = 0;
    }

    void setData(std::tuple<Ranges<TIters>...>& ranges)
    {
        [&]<std::size_t... Is>(const std::index_sequence<Is...>&) {
            (setDataHelper<Is>(std::get<Is>(ranges)), ...);
            isEnd = ((mEndIndexNumbers[Is] == 0) || ...);
        }(std::make_index_sequence<sizeof...(TIters)>());
    }

    CombinationsType mCurrentState;
    std::array<int64_t, sizeof...(TIters)> mCurrentIndexNumbers{};
    std::array<int64_t, sizeof...(TIters)> mEndIndexNumbers;
    bool isEnd = false;
};

template <typename... TIters>
struct FullCombinationsPolicy {
    CombinationsPolicyBase<TIters...> mBase;
    FullCombinationsPolicy() : mBase() {}

    void addOne()
    {
        constexpr size_t N = sizeof...(TIters);
        bool wasModified   = true;
        [&]<std::size_t... Is>(const std::index_sequence<Is...>&) {
            (addOneFun<Is, N>(wasModified), ...);
        }(std::make_index_sequence<N>());
        mBase.isEnd = wasModified;
    }

    void setData(std::tuple<Ranges<TIters>...>& ranges) { mBase.setData(ranges); }

    auto state() { return mBase.mCurrentState; }

    bool isEnd() { return mBase.isEnd; }

    private:
    template <size_t I, size_t N>
    void addOneFun(bool& wasModified)
    {
        if (wasModified) {
            constexpr auto ind           = N - I - 1;
            int64_t currentPointersIndex = ++mBase.mCurrentIndexNumbers[ind];
            ++std::get<ind>(mBase.mCurrentState);
            if (currentPointersIndex != mBase.mEndIndexNumbers[ind]) {
                [&]<std::size_t... Is>(const std::index_sequence<Is...>&) {
                    (resetState<I, Is, N>(), ...);
                }(std::make_index_sequence<I>());
                wasModified = false;
            }
        }
    }

    // N - number of data sources
    // I - which position from the right side is considered
    // J - loop iterator, which pointer is set to 0 from the N - I to right position
    template <size_t I, size_t J, size_t N>
    void resetState()
    {
        // Clang format makes it look very strange
        constexpr auto ind = N - I + J;
        std::get<ind>(mBase.mCurrentState) -= mBase.mCurrentIndexNumbers[ind];
        mBase.mCurrentIndexNumbers[ind] = 0;
    }
};

template <typename... TIters>
struct StrictlyUpperCombinationsPolicy {
    CombinationsPolicyBase<TIters...> mBase;
    StrictlyUpperCombinationsPolicy() : mBase() {}

    void addOne()
    {
        constexpr size_t N = sizeof...(TIters);
        bool wasModified   = true;
        [&]<std::size_t... Is>(const std::index_sequence<Is...>&) {
            (addOneFun<Is, N>(wasModified), ...);
        }(std::make_index_sequence<N>());
        mBase.isEnd = wasModified;
    }

    void setData(std::tuple<Ranges<TIters>...>& ranges)
    {
        mBase.setData(ranges);

        // Set ranges here have the same logic as a loop into addOneFun with setting new
        // pointers for right side positions.
        constexpr auto N = sizeof...(TIters);
        if (!mBase.isEnd) {
            bool shouldEnd = true;
            setRanges<N - 1, N>(shouldEnd);
            mBase.isEnd = !shouldEnd;
        }
    }

    auto state() { return mBase.mCurrentState; }

    bool isEnd() { return mBase.isEnd; }

    private:
    // TODO: Is it better setRanges return boolean instead of using in/out parameter?
    template <size_t I, size_t N>
    void addOneFun(bool& wasModified)
    {
        if (wasModified) {
            constexpr auto ind           = N - I - 1;
            int64_t currentPointersIndex = ++mBase.mCurrentIndexNumbers[ind];
            ++std::get<ind>(mBase.mCurrentState);
            if (currentPointersIndex != mBase.mEndIndexNumbers[ind]) {
                bool wasChanged = true;
                setRanges<I, N>(wasChanged);
                wasModified = !wasChanged;
            }
        }
    }

    template <size_t I, size_t N>
    void setRanges(bool& wasChanged)
    {
        [&]<std::size_t... Is>(const std::index_sequence<Is...>&) {
            (resetState<I, Is, N>(wasChanged), ...);
        }(std::make_index_sequence<I>());
    }

    // Here there are changes from the Full version. The pointers must be set not to 0, but to
    // Position + 1 from the nearest left pointer.
    // N - number of data sources
    // I - which position from the right side is considered
    // J - loop iterator, which pointer is set to 0 from the N - I to right position
    template <size_t I, size_t J, size_t N>
    void resetState(bool& wasChanged)
    {
        if (wasChanged) {
            constexpr auto ind = N - I + J;
            int64_t tmpInd     = mBase.mCurrentIndexNumbers[ind - 1] + 1;
            if (tmpInd < mBase.mEndIndexNumbers[ind]) {
                std::get<ind>(mBase.mCurrentState) += tmpInd - mBase.mCurrentIndexNumbers[ind];
                mBase.mCurrentIndexNumbers[ind] = tmpInd;
            } else {
                wasChanged = false;
            }
        }
    }
};

#endif  // COMBINATIONS_H
