//
// Created by Mateusz Mikiciuk on 11/09/2025.
//

#ifndef BLOCKCOMBINATINOS_H
#define BLOCKCOMBINATINOS_H

#include "Combinations.h"

template <
    typename TBucketPolicy, typename TCombinationsPolicy, typename TCombinations,
    typename... TIters>
struct BlockCombinations {

    using BucketIterType = std::set<int>::const_iterator;

    BlockCombinations(
        const TBucketPolicy& bucketPolicy, const TCombinationsPolicy& combinationsPolicy,
        const std::tuple<Ranges<TIters>...>& ranges
    )
        : mGroupedData(tuple_transform(
              ranges,
              [&](auto&& r) {
                  return groupData(r.mBegin, r.mEnd, bucketPolicy);
              }
          )),
          mCombinationsPolicy(combinationsPolicy),
          mBucketPolicy(bucketPolicy)
    {
        syncBuckets(mGroupedData);
        mCurrent = std::get<0>(mGroupedData).begin();
        mEnd     = std::get<0>(mGroupedData).end();
        setCombinations(std::make_index_sequence<sizeof...(TIters)>{}, mCurrent);
    }

    auto& data() { return mGroupedData; }

    struct BlockIterator {
        using iterator_category = std::forward_iterator_tag;
        using difference_type   = std::ptrdiff_t;
        // using value_type = std::tuple<typename TIters::value_type...>;
        // using pointer =
        // using reference

        BucketIterType mIter;
        BlockCombinations mBlockCombinations;
        explicit BlockIterator(const BucketIterType iter, BlockCombinations& blockCombinations)
            : mIter(iter), mBlockCombinations(blockCombinations)
        {
        }

        BlockIterator& operator++()
        {
            mBlockCombinations.addOne();
            ++mIter;
            return *this;
        }

        BlockIterator operator++(int)
        {
            BlockIterator copy = *this;
            ++(*this);
            return copy;
        }

        friend bool operator==(const BlockIterator& lhs, const BlockIterator& rhs)
        {
            return lhs.mIter == rhs.mIter;
        }
        friend bool operator!=(const BlockIterator& lhs, const BlockIterator& rhs)
        {
            return !(lhs.mIter == rhs.mIter);
        }
    };

    // struct BlockSentinel {
    // };
    // BlockIterator begin() { return BlockIterator(*this); }
    // BlockSentinel end() { return BlockSentinel{}; }
    //
    // friend bool operator!=(const BlockIterator& it, const BlockSentinel&)
    // {
    //     return !it.mBlock.isEnd();
    // }
    //
    // friend bool operator!=(const BlockSentinel& s, const BlockIterator& it) { return it != s; }
    //
    // friend bool operator==(const BlockIterator& it, const BlockSentinel& s) { return !(it != s);
    // }

    private:
    std::tuple<GroupedData<TIters>...> mGroupedData;
    TCombinationsPolicy mCombinationsPolicy;
    TBucketPolicy mBucketPolicy;
    BucketIterType mCurrent;
    BucketIterType mEnd;

    [[nodiscard]] bool isEnd() const { return mCombinationsPolicy.isEnd() && mCurrent == mEnd; }

    void addOne()
    {
        mCombinationsPolicy.addOne();
        if (mCombinationsPolicy.isEnd()) {
            ++mCurrent;
            if (mCurrent != mEnd) {
                setCombinations(std::make_index_sequence<sizeof...(TIters)>{}, mCurrent);
            }
        }
    }

    auto& state() { return mCombinationsPolicy.state(); }

    template <typename TIter>
    auto createRanges(const GroupedData<TIter>& data, BucketIterType current)
    {
        const std::vector<TIter>& bucket = data.at(current);
        using IterType                   = typename std::vector<TIter>::const_iterator;
        return Ranges<IterType>(bucket.begin(), bucket.end());
    }

    template <std::size_t... Is>
    void setCombinations(const std::index_sequence<Is...>&, BucketIterType current)
    {
        auto bucketsRanges = tuple_transform(mGroupedData, [&](auto&& data) {
            return createRanges(data, current);
        });
        mCombinationsPolicy.setData(bucketsRanges);
    }
};

template <
    template <typename...> class TCombinationsPolicy, typename TBucketPolicy, typename... TIters>
auto makeBlockCombinations(
    const TBucketPolicy& bucketPolicy, const std::tuple<Ranges<TIters>...>& ranges
)
{
    using CombinationsType = std::tuple<typename std::vector<TIters>::const_iterator...>;
    using PolicyType       = TCombinationsPolicy<typename std::vector<TIters>::const_iterator...>;
    return BlockCombinations<TBucketPolicy, PolicyType, CombinationsType, TIters...>(
        bucketPolicy, PolicyType{}, ranges
    );
}

#endif  // BLOCKCOMBINATINOS_H
