//
// Created by Mateusz Mikiciuk on 11/09/2025.
//

#ifndef BLOCKCOMBINATINOS_H
#define BLOCKCOMBINATINOS_H

#include "Combinations.h"

template <
    typename TBucketPolicy, typename TCombinationsPolicy, typename TCombinations,
    typename... TIters>
struct BlockCombinationsProducer {
    using BucketIterType           = std::set<int>::const_iterator;
    using CombinationsType         = std::tuple<TIters...>;
    using CombinationsProducerType = CombinationsProducer<TCombinationsPolicy, TIters...>;
    using CombinationsIterType     = CombinationsProducerType::CombinationsIterator;
    BlockCombinationsProducer(
        const TBucketPolicy& bucketPolicy, const TCombinationsPolicy& combinationsPolicy,
        const std::tuple<Ranges<TIters>...>& ranges
    )
        : mGroupedData(tupleTransform(
              ranges,
              [&](auto&& r) {
                  return groupData(r.mBegin, r.mEnd, bucketPolicy);
              }
          )),
          mCombinationsProducer(combinationsPolicy),
          mBucketPolicy(bucketPolicy)
    {
        syncBuckets(mGroupedData);
        mCurrent = std::get<0>(mGroupedData).begin();
        mEnd     = std::get<0>(mGroupedData).end();
        setCombinations(std::make_index_sequence<sizeof...(TIters)>{}, mCurrent);
        mCombinationsIterator = mCombinationsProducer.begin();
        mCombinationsEnd      = mCombinationsProducer.end();
    }

    auto& data() { return mGroupedData; }

    struct BlockIterator {
        using iterator_category = std::input_iterator_tag;
        using difference_type   = std::ptrdiff_t;
        using value_type        = TCombinations;
        using pointer           = const TCombinations*;
        using reference         = const TCombinations&;

        BlockCombinationsProducer* mBlockCombinations;
        CombinationsIterType iterator;
        explicit BlockIterator(
            BlockCombinationsProducer* blockCombinations, CombinationsIterType iterator
        )
            : mBlockCombinations(blockCombinations), iterator(iterator)
        {
        }

        BlockIterator& operator++()
        {
            mBlockCombinations->addOne();
            iterator = mBlockCombinations->mCombinationsIterator;
            return *this;
        }

        BlockIterator operator++(int)
        {
            BlockIterator copy = *this;
            ++(*this);
            return copy;
        }

        reference operator*() const { return *iterator; }
        pointer operator->() const { return &(*iterator); }

        friend bool operator==(const BlockIterator& lhs, const BlockIterator& rhs)
        {
            return lhs.mBlockCombinations == rhs.mBlockCombinations && lhs.iterator == rhs.iterator;
        }
        friend bool operator!=(const BlockIterator& lhs, const BlockIterator& rhs)
        {
            return !(lhs == rhs);
        }
    };

    BlockIterator begin() { return BlockIterator(this, mCombinationsIterator); }
    BlockIterator end() { return BlockIterator(this, mCombinationsEnd); }

    private:
    std::tuple<GroupedData<TIters>...> mGroupedData;
    CombinationsProducerType mCombinationsProducer;
    CombinationsIterType mCombinationsIterator;
    CombinationsIterType mCombinationsEnd;
    TBucketPolicy mBucketPolicy;
    BucketIterType mCurrent;
    BucketIterType mEnd;

    [[nodiscard]] bool isEnd() const
    {
        return mCombinationsProducer.finished() && mCurrent == mEnd;
    }

    void addOne()
    {
        ++mCombinationsIterator;
        if (mCombinationsIterator == mCombinationsEnd) {
            ++mCurrent;
            if (mCurrent != mEnd) {
                setCombinations(std::make_index_sequence<sizeof...(TIters)>{}, mCurrent);
                mCombinationsIterator = mCombinationsProducer.begin();
                mCombinationsEnd      = mCombinationsProducer.end();
            }
        }
    }

    template <typename TIter>
    auto createRanges(const GroupedData<TIter>& data, BucketIterType current)
    {
        const std::vector<TIter>& bucket = data.at(current);
        using IterType                   = std::vector<TIter>::const_iterator;
        return Ranges<IterType>(bucket.begin(), bucket.end());
    }

    template <std::size_t... Is>
    void setCombinations(const std::index_sequence<Is...>&, BucketIterType current)
    {
        auto bucketsRanges = tupleTransform(mGroupedData, [&](auto&& data) {
            return createRanges(data, current);
        });
        mCombinationsProducer.setData(bucketsRanges);
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
    return BlockCombinationsProducer<TBucketPolicy, PolicyType, CombinationsType, TIters...>(
        bucketPolicy, PolicyType{}, ranges
    );
}

#endif  // BLOCKCOMBINATINOS_H
