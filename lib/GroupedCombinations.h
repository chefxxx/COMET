//
// Created by msh on 12/26/25.
//

#ifndef COMET_GROUPEDCOMBINATIONS_H
#define COMET_GROUPEDCOMBINATIONS_H

#include "BucketSortView.h"

template <typename TSource, typename TCallable>
struct SourceWithCallable {
    using source   = TSource;
    using callable = TCallable;
    source ProvidedSource;
    callable ProvidedCallable;
};

template <typename TGrouping, typename TAssociated>
concept ValidAssociated = AssociatedCallableResultIsConvertibleToGrouping<
    typename TGrouping::source, typename TGrouping::callable, typename TAssociated::source,
    typename TAssociated::callable>;

template <typename TCombinationsProducer, typename TGrouping, typename... TAssociated>
    requires(ValidAssociated<TGrouping, TAssociated> && ...)
struct GroupedCombinations {
    public:
    explicit GroupedCombinations(
        const TCombinationsProducer& combinations_producer, const TGrouping& grouping,
        const TAssociated&... associated
    )
    {
        m_combinationsProducer = combinations_producer;
        m_views                = std::make_tuple(BucketSortView(
            grouping.ProvidedSource, grouping.ProvidedCallable, associated.ProvidedSource,
            associated.ProvidedCallable
        )...);
    }

    private:
    TCombinationsProducer m_combinationsProducer;
    std::tuple<BucketSortView<
        typename TGrouping::source, typename TGrouping::callable, typename TAssociated::source,
        typename TAssociated::callable>...>
        m_views;
};

template <
    template <typename...> class TCombinationsPolicy, typename TBucketPolicy, typename TGrouping>

auto makeGroupedCombinations()
{
}

#endif  // COMET_GROUPEDCOMBINATIONS_H
