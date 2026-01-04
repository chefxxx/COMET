//
// Created by msh on 12/26/25.
//

#ifndef COMET_GROUPEDCOMBINATIONS_H
#define COMET_GROUPEDCOMBINATIONS_H

#include "BucketSortView.h"
#include "Producers.h"

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
        TCombinationsProducer&& combinations_producer, const TGrouping& grouping,
        const TAssociated&... associated
        ) : m_combinationsProducer(combinations_producer), m_views(std::make_tuple(BucketSortView(
                grouping.ProvidedSource, grouping.ProvidedCallable, associated.ProvidedSource,
                associated.ProvidedCallable
            )...)) {}

    private:
    TCombinationsProducer m_combinationsProducer;
    std::tuple<BucketSortView<
        typename TGrouping::source, typename TGrouping::callable, typename TAssociated::source,
        typename TAssociated::callable>...>
        m_views;

    // Helper structs for obtaining types from the views and combine them with Grouping type
    template <typename T>
    struct ExtractSpanViewTypes;

    template <typename... Views>
    struct ExtractSpanViewTypes<std::tuple<Views...>> {
        using type = std::tuple<typename Views::SpanViewType...>;
    };

    template <typename G, typename Tuple>
    struct InterleaveWithTuple;

    template <typename G, typename... As>
    struct InterleaveWithTuple<G, std::tuple<As...>> {
        using type = decltype(std::tuple_cat(std::declval<std::tuple<G, As>>()...));
    };

    public:
    using AssociatedTupleType = ExtractSpanViewTypes<decltype(m_views)>::type;
    using ResultTupleType =
        InterleaveWithTuple<typename TGrouping::source, AssociatedTupleType>::type;
};

template <
    template <typename...> class TCombinationsPolicy, typename TBucketPolicy, typename TGrouping,
    typename... TAssociated>
auto makeGroupedCombinations(
    const TBucketPolicy& t_bucketPolicy, const int t_minCatSize, const TGrouping& grouping,
    const TAssociated&... associated
)
{
    constexpr int N            = sizeof...(TAssociated);
    auto combinationsGenerator = [&]<size_t... Is>(std::index_sequence<Is...>) {
        return makeBlockCombinations<TCombinationsPolicy>(
            t_bucketPolicy, t_minCatSize, (static_cast<void>(Is), grouping.ProvidedSource)...
        );
    }(std::make_index_sequence<N>{});
    return GroupedCombinations(std::move(combinationsGenerator), grouping, associated...);
}

#endif  // COMET_GROUPEDCOMBINATIONS_H
