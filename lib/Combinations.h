//
// Created by Mateusz Mikiciuk on 22/07/2025.
//

#ifndef COMBINATIONS_H
#define COMBINATIONS_H

#include <tuple>
#include "Helpers.h"

template <typename TCombinationsPolicy, typename... TIters>
struct CombinationsProducer {
    using CombinationsType = typename TCombinationsPolicy::CombinationsType;
    explicit CombinationsProducer(TCombinationsPolicy combinationsPolicy)
        : mCombinationsPolicy(combinationsPolicy)
    {
    }
    explicit CombinationsProducer(
        TCombinationsPolicy combinationsPolicy, std::tuple<Ranges<TIters>...>& ranges
    )
        : mCombinationsPolicy(combinationsPolicy)
    {
        mCombinationsPolicy.setData(
            ranges, mCurrentState, mSentinel, mCurrentIndexNumbers, mEndIndexNumbers, mIsEnd
        );
    }

    void setData(std::tuple<Ranges<TIters>...>& ranges)
    {
        mCombinationsPolicy.setData(
            ranges, mCurrentState, mSentinel, mCurrentIndexNumbers, mEndIndexNumbers, mIsEnd
        );
    }

    // void setData(std::tuple<Ranges<typename std::vector<TIters>::const_iterator>...>& ranges)
    // {
    //     mCombinationsPolicy.setData(
    //         ranges, mCurrentState, mSentinel, mCurrentIndexNumbers, mEndIndexNumbers, mIsEnd
    //     );
    // }

    [[nodiscard]] bool isEnd() const { return mIsEnd; }

    struct CombinationsIterator {
        using iterator_category = std::input_iterator_tag;
        using difference_type   = std::ptrdiff_t;
        using value_type        = CombinationsType;
        using pointer           = const CombinationsType*;
        using reference         = const CombinationsType&;

        CombinationsType* mCombinationsPtr;
        CombinationsProducer* mProducerPtr;
        CombinationsIterator() = default;
        explicit CombinationsIterator(CombinationsType* state, CombinationsProducer* producer
        )
            : mCombinationsPtr(state), mProducerPtr(producer)
        {
        }
        CombinationsIterator(const CombinationsIterator&)            = default;
        CombinationsIterator& operator=(const CombinationsIterator&) = default;

        CombinationsIterator& operator++()
        {
            mProducerPtr->addOne();
            mCombinationsPtr = &mProducerPtr->mCurrentState;
            return *this;
        }
        CombinationsIterator operator++(int)
        {
            CombinationsIterator copy = *this;
            ++(*this);
            return copy;
        }
        reference operator*() const { return *mCombinationsPtr; }
        pointer operator->() { return mCombinationsPtr; }

        friend bool operator==(const CombinationsIterator& lhs, const CombinationsIterator& rhs)
        {
            if (lhs.mProducerPtr->mIsEnd && rhs.mProducerPtr->mIsEnd)
                return true;
            return &lhs.mProducerPtr == &rhs.mProducerPtr && lhs.mCombinationsPtr == rhs.mCombinationsPtr;
        }
        friend bool operator!=(const CombinationsIterator& lhs, const CombinationsIterator& rhs)
        {
            return !(lhs == rhs);
        }
    };

    [[nodiscard]] CombinationsIterator begin()
    {
        return CombinationsIterator(&mCurrentState, this);
    }
    [[nodiscard]] CombinationsIterator end()
    {
        return CombinationsIterator(&mSentinel, this);
    }

    private:
    bool mIsEnd = false;
    CombinationsType mSentinel;
    CombinationsType mCurrentState;
    TCombinationsPolicy mCombinationsPolicy;
    std::array<int64_t, sizeof...(TIters)> mEndIndexNumbers;
    std::array<int64_t, sizeof...(TIters)> mCurrentIndexNumbers;

    void addOne()
    {
        if (!mIsEnd)
            mCombinationsPolicy.addOne(
                mCurrentState, mCurrentIndexNumbers, mEndIndexNumbers, mIsEnd
            );
    }
};

template <size_t I, typename TIter, typename TCombinationsType>
void setDataHelper(
    Ranges<TIter>& range, TCombinationsType& currentState, TCombinationsType& sentinel,
    int64_t& currentIndexNumber, int64_t& endIndexNumber
)
{
    std::get<I>(currentState) = range.mBegin;
    std::get<I>(sentinel)     = range.mEnd;
    currentIndexNumber        = 0;
    endIndexNumber            = std::distance(range.mBegin, range.mEnd);
}

template <typename TCombinationsType, typename... TIters>
void setDataPolicies(
    std::tuple<Ranges<TIters>...>& ranges, TCombinationsType& currentState,
    TCombinationsType& sentinel, std::array<int64_t, sizeof...(TIters)>& currentIndexNumbers,
    std::array<int64_t, sizeof...(TIters)>& endIndexNumbers, bool& isEnd
)
{
    [&]<std::size_t... Is>(const std::index_sequence<Is...>&) {
        (setDataHelper<Is>(
             std::get<Is>(ranges), currentState, sentinel, currentIndexNumbers[Is],
             endIndexNumbers[Is]
         ),
         ...);
        isEnd = ((endIndexNumbers[Is] == 0) || ...);
    }(std::make_index_sequence<sizeof...(TIters)>{});
}

template <typename... TIters>
struct FullCombinationsPolicy {
    FullCombinationsPolicy() = default;
    using CombinationsType   = std::tuple<TIters...>;

    void setData(
        std::tuple<Ranges<TIters>...>& ranges, CombinationsType& currentState,
        CombinationsType& sentinel, std::array<int64_t, sizeof...(TIters)>& currentIndexNumbers,
        std::array<int64_t, sizeof...(TIters)>& endIndexNumbers, bool& isEnd
    )
    {
        setDataPolicies(
            ranges, currentState, sentinel, currentIndexNumbers, endIndexNumbers, isEnd
        );
    }

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

template <typename... TIters>
struct StrictlyUpperCombinationsPolicy {
    using CombinationsType            = std::tuple<TIters...>;
    StrictlyUpperCombinationsPolicy() = default;

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

    void setData(
        std::tuple<Ranges<TIters>...>& ranges, CombinationsType& currentState,
        CombinationsType& sentinel, std::array<int64_t, sizeof...(TIters)>& currentIndexNumbers,
        std::array<int64_t, sizeof...(TIters)>& endIndexNumbers, bool& isEnd
    )
    {
        setDataPolicies(
            ranges, currentState, sentinel, currentIndexNumbers, endIndexNumbers, isEnd
        );

        // Set ranges here have the same logic as a loop into addOneFun with setting new
        // pointers for right side positions.
        constexpr auto N = sizeof...(TIters);
        if (!isEnd) {
            bool shouldEnd = true;
            setRanges<N - 1, N>(shouldEnd, currentState, currentIndexNumbers, endIndexNumbers);
            isEnd = !shouldEnd;
        }
    }

    private:
    // TODO: Is it better setRanges return boolean instead of using in/out parameter?
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
                bool wasChanged = true;
                setRanges<I, N>(wasChanged, currentState, currentIndexNumbers, endIndexNumbers);
                wasModified = !wasChanged;
            }
        }
    }

    template <size_t I, size_t N>
    void setRanges(
        bool& wasChanged, CombinationsType& currentState,
        std::array<int64_t, sizeof...(TIters)>& currentIndexNumbers,
        std::array<int64_t, sizeof...(TIters)>& endIndexNumbers
    )
    {
        [&]<std::size_t... Is>(const std::index_sequence<Is...>&) {
            (resetState<I, Is, N>(wasChanged, currentState, currentIndexNumbers, endIndexNumbers),
             ...);
        }(std::make_index_sequence<I>());
    }

    // Here there are changes from the Full version. The pointers must be set not to 0, but to
    // Position + 1 from the nearest left pointer.
    // N - number of data sources
    // I - which position from the right side is considered
    // J - loop iterator, which pointer is set to 0 from the N - I to right position
    template <size_t I, size_t J, size_t N>
    void resetState(
        bool& wasChanged, CombinationsType& currentState,
        std::array<int64_t, sizeof...(TIters)>& currentIndexNumbers,
        std::array<int64_t, sizeof...(TIters)>& endIndexNumbers
    )
    {
        if (wasChanged) {
            constexpr auto ind = N - I + J;
            int64_t tmpInd     = currentIndexNumbers[ind - 1] + 1;
            if (tmpInd < endIndexNumbers[ind]) {
                std::get<ind>(currentState) += tmpInd - currentIndexNumbers[ind];
                currentIndexNumbers[ind] = tmpInd;
            } else {
                wasChanged = false;
            }
        }
    }
};

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
