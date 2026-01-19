//
// Created by mshamrai on 12/30/25.
//

#ifndef COMET_BUCKETSORTVIEW_H
#define COMET_BUCKETSORTVIEW_H
#include <span>
#include "SpanView.h"

#include "BucketSortHelpers.h"

template <
    typename GroupingType, typename GroupingCallable, typename AssociatedType,
    typename AssociatedCallable>
concept AssociatedCallableResultIsConvertibleToGrouping = std::same_as<
    std::decay_t<std::invoke_result_t<GroupingCallable, typename GroupingType::value_type>>,
    std::decay_t<std::invoke_result_t<AssociatedCallable, typename AssociatedType::value_type>>>;

template <
    typename GroupingType, typename GroupingCallable, typename AssociatedType,
    typename AssociatedCallable>
    requires AssociatedCallableResultIsConvertibleToGrouping<
        GroupingType, GroupingCallable, AssociatedType, AssociatedCallable>
class BucketSortView
{
    public:
    using BucketType =
        std::decay_t<std::invoke_result_t<GroupingCallable, typename GroupingType::value_type>>;
    using AssociatedIteratorType = typename AssociatedType::const_iterator;
    using OriginalViewSpanType   = std::span<const AssociatedIteratorType>;
    using SpanViewType           = SpanView<OriginalViewSpanType>;

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

    auto getSpanForBucket(const BucketType& bucketId) const
    {
        auto index       = getIndexFromBucket(m_availableBuckets, bucketId);
        auto sortedStart = m_sortedIterators.begin();
        return index != -1
                   ? SpanView(OriginalViewSpanType(
                         sortedStart + m_offsets[index], m_offsets[index + 1] - m_offsets[index]
                     ))
                   : SpanView(OriginalViewSpanType());
    }

    private:
    std::vector<BucketType> m_availableBuckets;
    std::vector<AssociatedIteratorType> m_sortedIterators;
    std::vector<size_t> m_offsets;
};

#endif  // COMET_BUCKETSORTVIEW_H
