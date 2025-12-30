//
// Created by mshamrai on 12/30/25.
//

#ifndef COMET_BUCKETSORTVIEW_H
#define COMET_BUCKETSORTVIEW_H
#include <span>

#include "BucketSortHelpers.h"

template <typename GroupingType, typename AssociatedType, typename BucketType>
class BucketSortView
{
    public:
    using AssociatedIteratorType = typename AssociatedType::const_iterator;
    using ViewSpanType           = typename std::span<AssociatedIteratorType>;

    template <typename GroupingCallable, typename AssociatedCallable>
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
        m_sortedIterators = getIteratorsSorted(amounts);
    }

    ViewSpanType getSpanForBucket(const BucketType& bucketId) const
    {
        auto index = getIndexFromBucket(m_availableBuckets, bucketId);
        assert(index != -1);
        return index != -1 ? ViewSpanType(m_sortedIterators[m_offsets[index]], m_offsets[index + 1])
                           : ViewSpanType();
    }

    private:
    std::vector<BucketType> m_availableBuckets;
    std::vector<AssociatedIteratorType> m_sortedIterators;
    std::vector<size_t> m_offsets;
};

#endif  // COMET_BUCKETSORTVIEW_H
