//
// Created by mshamrai on 12/30/25.
//

#ifndef COMET_BUCKETSORTVIEW_H
#define COMET_BUCKETSORTVIEW_H
#include <random>
#include <span>

#include "BucketSortHelpers.h"

template <
    typename GroupingType, typename GroupingCallable, typename AssociatedType,
    typename AssociatedCallable>
class BucketSortView
{
    public:
    using BucketType =
        std::decay_t<std::invoke_result_t<GroupingCallable, typename GroupingType::value_type>>;
    using AssociatedIteratorType = typename AssociatedType::const_iterator;
    using ViewSpanType           = typename std::span<AssociatedIteratorType>;

    BucketSortView(
        const GroupingType& groupingSource, const GroupingCallable& groupingCallable,
        const AssociatedType& associatedSource, const AssociatedCallable& associatedCallable
    )
    {
        m_availableBuckets = getAvailableBuckets<BucketType>(groupingSource, groupingCallable);
        auto amounts       = getAmountsInsideAvailableBuckets(
            associatedSource, associatedCallable, m_availableBuckets
        );
        m_offsets         = getOffsetsFromAmounts(amounts);
        m_sortedIterators = getIteratorsSorted(
            associatedSource, associatedCallable, m_offsets, m_availableBuckets, amounts
        );
    }

    ViewSpanType getSpanForBucket(const BucketType& bucketId) const
    {
        auto index = getIndexFromBucket(m_availableBuckets, bucketId);
        assert(index != -1);
        auto sortedStart = m_sortedIterators.begin();
        return index != -1
                   ? ViewSpanType(
                         sortedStart + m_offsets[index], m_offsets[index + 1] - m_offsets[index]
                     )
                   : ViewSpanType();
    }

    private:
    std::vector<BucketType> m_availableBuckets;
    std::vector<AssociatedIteratorType> m_sortedIterators;
    std::vector<size_t> m_offsets;
};

#endif  // COMET_BUCKETSORTVIEW_H
