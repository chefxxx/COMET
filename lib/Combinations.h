//
// Created by Mateusz Mikiciuk on 22/07/2025.
//

#ifndef COMBINATIONS_H
#define COMBINATIONS_H

#include <tuple>
#include "Helpers.h"

template <typename TCombinationsPolicy, typename... TIters>
struct CombinationsProducer {
    using CombinationsType = std::tuple<TIters...>;
    explicit CombinationsProducer(TCombinationsPolicy combinationsPolicy)
        : mCombinationsPolicy(combinationsPolicy)
    {
    }
    explicit CombinationsProducer(
        TCombinationsPolicy combinationsPolicy, std::tuple<Ranges<TIters>...>& ranges
    )
        : mCombinationsPolicy(combinationsPolicy)
    {
        setData(ranges);
    }

    template <size_t I, typename TIter>
    void setDataHelper(Ranges<TIter>& range)
    {
        std::get<I>(mBeginState)   = range.mBegin;
        std::get<I>(mCurrentState) = range.mBegin;
        std::get<I>(mSentinel)     = range.mEnd;
        mEndIndexNumbers[I]        = std::distance(range.mBegin, range.mEnd);
        mCurrentIndexNumbers[I]    = 0;
    }

    void setData(std::tuple<Ranges<TIters>...>& ranges)
    {
        [&]<std::size_t... Is>(const std::index_sequence<Is...>&) {
            (setDataHelper<Is>(std::get<Is>(ranges)), ...);
            isEnd = ((mEndIndexNumbers[Is] == 0) || ...);
        }(std::make_index_sequence<sizeof...(TIters)>{});
    }

    void addOne()
    {
        if (!isEnd)
            mCombinationsPolicy.addOne(
                mCurrentState, mCurrentIndexNumbers, mEndIndexNumbers, isEnd
            );
    }

    [[nodiscard]] bool finished() const { return isEnd; }

    struct CombinationsIterator {
        using iterator_category = std::input_iterator_tag;
        using difference_type   = std::ptrdiff_t;
        using value_type        = CombinationsType;
        using pointer           = const CombinationsType*;
        using reference         = const CombinationsType&;

        CombinationsType* mPtr;
        CombinationsProducer* mProducer;
        CombinationsIterator() = default;
        explicit CombinationsIterator(CombinationsType* state, CombinationsProducer* producer)
            : mPtr(state), mProducer(producer)
        {
        }
        CombinationsIterator(const CombinationsIterator&)            = default;
        CombinationsIterator& operator=(const CombinationsIterator&) = default;

        CombinationsIterator& operator++()
        {
            mProducer->addOne();
            mPtr = &mProducer->mCurrentState;
            return *this;
        }
        CombinationsIterator operator++(int)
        {
            CombinationsIterator copy = *this;
            ++(*this);
            return copy;
        }
        reference operator*() const { return *mPtr; }
        pointer operator->() { return mPtr; }

        friend bool operator==(const CombinationsIterator& lhs, const CombinationsIterator& rhs)
        {
            if (lhs.mProducer->isEnd && rhs.mProducer->isEnd) return true;
            return lhs.mProducer == rhs.mProducer && lhs.mPtr == rhs.mPtr;
        }
        friend bool operator!=(const CombinationsIterator& lhs, const CombinationsIterator& rhs)
        {
            return !(lhs == rhs);
        }
    };

    CombinationsIterator begin() { return CombinationsIterator(&mBeginState, this); }
    CombinationsIterator end() { return CombinationsIterator(&mSentinel, this); }

    private:
    bool isEnd = false;
    CombinationsType mCurrentState;
    CombinationsType mSentinel;
    CombinationsType mBeginState;
    TCombinationsPolicy mCombinationsPolicy;
    std::array<int64_t, sizeof...(TIters)> mEndIndexNumbers;
    std::array<int64_t, sizeof...(TIters)> mCurrentIndexNumbers;
};

template <typename... TIters>
struct FullCombinationsPolicy {
    FullCombinationsPolicy() = default;

    void addOne(
        std::tuple<TIters...>& currentState,
        std::array<int64_t, sizeof...(TIters)>& currentIndexNumbers,
        std::array<int64_t, sizeof...(TIters)>& endIndexNumbers, bool& isEnd
    )
    {
        constexpr size_t N = sizeof...(TIters);
        bool wasModified   = true;
        [&]<std::size_t... Is>(const std::index_sequence<Is...>&) {
            (addOneHelper<Is, N>(wasModified, currentState, currentIndexNumbers, endIndexNumbers),
             ...);
        }(std::make_index_sequence<N>());
        isEnd = wasModified;
    }

    private:
    template <size_t I, size_t N>
    void addOneHelper(
        bool& wasModified, std::tuple<TIters...>& currentState,
        std::array<int64_t, sizeof...(TIters)>& currentIndexNumbers,
        std::array<int64_t, sizeof...(TIters)>& endIndexNumbers
    )
    {
        if (wasModified) {
            constexpr auto ind           = N - I - 1;
            int64_t currentPointersIndex = ++currentIndexNumbers[ind];
            ++std::get<ind>(currentState);
            if (currentPointersIndex != endIndexNumbers[ind]) {
                [&]<std::size_t... Is>(const std::index_sequence<Is...>&) {
                    (resetState<I, Is, N>(currentState, currentIndexNumbers), ...);
                }(std::make_index_sequence<I>());
                wasModified = false;
            }
        }
    }

    // N - number of data sources
    // I - which position from the right side is considered
    // J - loop iterator, which pointer is set to 0 from the N - I to right position
    template <size_t I, size_t J, size_t N>
    void resetState(
        std::tuple<TIters...>& currentState,
        std::array<int64_t, sizeof...(TIters)>& currentIndexNumbers
    )
    {
        constexpr auto ind = N - I + J;
        std::get<ind>(currentState) -= currentIndexNumbers[ind];
        currentIndexNumbers[ind] = 0;
    }
};

// template <typename... TIters>
// struct StrictlyUpperCombinationsPolicy {
//
//     void addOne()
//     {
//         constexpr size_t N = sizeof...(TIters);
//         bool wasModified   = true;
//         [&]<std::size_t... Is>(const std::index_sequence<Is...>&) {
//             (addOneFun<Is, N>(wasModified), ...);
//         }(std::make_index_sequence<N>());
//         mBase.isEnd = wasModified;
//     }
//
//     void setData(std::tuple<Ranges<TIters>...>& ranges)
//     {
//         mBase.setData(ranges);
//
//         // Set ranges here have the same logic as a loop into addOneFun with setting new
//         // pointers for right side positions.
//         constexpr auto N = sizeof...(TIters);
//         if (!mBase.isEnd) {
//             bool shouldEnd = true;
//             setRanges<N - 1, N>(shouldEnd);
//             mBase.isEnd = !shouldEnd;
//         }
//     }
//
//     private:
//     // TODO: Is it better setRanges return boolean instead of using in/out parameter?
//     template <size_t I, size_t N>
//     void addOneFun(bool& wasModified)
//     {
//         if (wasModified) {
//             constexpr auto ind           = N - I - 1;
//             int64_t currentPointersIndex = ++mBase.mCurrentIndexNumbers[ind];
//             ++std::get<ind>(mBase.mCurrentState);
//             if (currentPointersIndex != mBase.mEndIndexNumbers[ind]) {
//                 bool wasChanged = true;
//                 setRanges<I, N>(wasChanged);
//                 wasModified = !wasChanged;
//             }
//         }
//     }
//
//     template <size_t I, size_t N>
//     void setRanges(bool& wasChanged)
//     {
//         [&]<std::size_t... Is>(const std::index_sequence<Is...>&) {
//             (resetState<I, Is, N>(wasChanged), ...);
//         }(std::make_index_sequence<I>());
//     }
//
//     // Here there are changes from the Full version. The pointers must be set not to 0, but to
//     // Position + 1 from the nearest left pointer.
//     // N - number of data sources
//     // I - which position from the right side is considered
//     // J - loop iterator, which pointer is set to 0 from the N - I to right position
//     template <size_t I, size_t J, size_t N>
//     void resetState(bool& wasChanged)
//     {
//         if (wasChanged) {
//             constexpr auto ind = N - I + J;
//             int64_t tmpInd     = mBase.mCurrentIndexNumbers[ind - 1] + 1;
//             if (tmpInd < mBase.mEndIndexNumbers[ind]) {
//                 std::get<ind>(mBase.mCurrentState) += tmpInd - mBase.mCurrentIndexNumbers[ind];
//                 mBase.mCurrentIndexNumbers[ind] = tmpInd;
//             } else {
//                 wasChanged = false;
//             }
//         }
//     }
// };

template <template <typename...> class TCombinationsPolicy, typename... TIters>
auto makeCombinations()
{
    using CombinationsPolicy = TCombinationsPolicy<TIters...>;
    return CombinationsProducer<CombinationsPolicy, TIters...>(CombinationsPolicy{});
}

template <template <typename...> class TCombinationsPolicy, typename... TIters>
auto makeCombinations(std::tuple<Ranges<TIters>...>& ranges)
{
    using CombinationsPolicy = TCombinationsPolicy<TIters...>;
    return CombinationsProducer(CombinationsPolicy{}, ranges);
}

#endif  // COMBINATIONS_H
