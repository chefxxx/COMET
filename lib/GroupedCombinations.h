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

template <
    typename TProducerType, template <typename...> typename TCombinationsPolicy,
    typename TBucketPolicy, typename TGrouping, typename... TAssociated>
    requires(ValidAssociated<TGrouping, TAssociated> && ...)
struct GroupedCombinations {
    using GroupingCallableType = TGrouping::callable;
    using ViewsType            = std::tuple<BucketSortView<
        typename TGrouping::source, typename TGrouping::callable, typename TAssociated::source,
        typename TAssociated::callable>...>;

    template <std::size_t... Is>
    static TProducerType
    makeProducer(const TBucketPolicy& bucketPolicy, int minCatSize, const TGrouping& grouping, std::index_sequence<Is...>)
    {
        return makeBlockCombinations<TCombinationsPolicy>(
            bucketPolicy, minCatSize, (static_cast<void>(Is), grouping.ProvidedSource)...
        );
    }

    explicit GroupedCombinations(
        const TBucketPolicy& t_bucketPolicy, const int t_minCatSize, const TGrouping& grouping,
        const TAssociated&... associated
    )
        : m_views(std::make_tuple(BucketSortView(
              grouping.ProvidedSource, grouping.ProvidedCallable, associated.ProvidedSource,
              associated.ProvidedCallable
          )...)),
          m_groupingCallable(grouping.ProvidedCallable),
          m_combinationsProducer(makeProducer(
              t_bucketPolicy, t_minCatSize, grouping,
              std::make_index_sequence<sizeof...(TAssociated)>{}
          ))
    {
    }

    private:
    ViewsType m_views;
    GroupingCallableType m_groupingCallable;
    TProducerType m_combinationsProducer;

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
        InterleaveWithTuple<typename TGrouping::source::value_type, AssociatedTupleType>::type;
    using ResultTupleTypeRef = InterleaveWithTuple<
        typename std::iterator_traits<typename TGrouping::source::const_iterator>::reference,
        AssociatedTupleType>::type;

    struct GroupingSentinel {
    };
    struct GroupedIterator {
        using iterator_category = std::input_iterator_tag;
        using difference_type   = std::ptrdiff_t;
        using value_type        = ResultTupleType;
        using pointer           = void;
        using reference         = ResultTupleTypeRef;

        using BlockIteratorType    = TProducerType::iterator;
        using GroupingCallableType = TGrouping::callable;

        GroupedIterator() = default;
        GroupedIterator(
            ViewsType* viewsPtr, GroupingCallableType* callablePtr,
            TProducerType* combinationsProducerPtr
        )
            : m_viewsPtr(viewsPtr),
              m_callablePtr(callablePtr),
              m_combinationsProducerPtr(combinationsProducerPtr),
              m_blockIteratorPtr(combinationsProducerPtr->begin())
        {
        }

        GroupedIterator& operator++()
        {
            ++m_blockIteratorPtr;
            return *this;
        }

        GroupedIterator operator++(int)
        {
            auto copy = *this;
            ++(*this);
            return copy;
        }

        reference operator*() const
        {
            auto currentCombination = *m_blockIteratorPtr;
            constexpr auto N        = sizeof...(TAssociated);
            return [&]<size_t... Is>(std::index_sequence<Is...>) {
                return std::tuple_cat(std::make_tuple(
                    std::get<Is>(currentCombination),
                    std::get<Is>(*m_viewsPtr)
                        .getSpanForBucket((*m_callablePtr)(std::get<Is>(currentCombination)))
                )...);
            }(std::make_index_sequence<N>{});
        }

        friend bool operator==(const GroupedIterator& lhs, const GroupedIterator& rhs)
        {
            return lhs.m_blockIteratorPtr == rhs.m_blockIteratorPtr;
        }

        friend bool operator==(const GroupedIterator& lhs, GroupingSentinel)
        {
            return lhs.m_blockIteratorPtr == lhs.m_combinationsProducerPtr->end();
        }

        private:
        ViewsType* m_viewsPtr;
        GroupingCallableType* m_callablePtr;
        TProducerType* m_combinationsProducerPtr;
        BlockIteratorType m_blockIteratorPtr;
    };

    [[nodiscard]] GroupedIterator begin()
    {
        return GroupedIterator(&m_views, &m_groupingCallable, &m_combinationsProducer);
    }
    [[nodiscard]] GroupingSentinel end() { return GroupingSentinel{}; }
};

template <
    template <typename...> typename TCombinationsPolicy, typename TBucketPolicy, typename TGrouping,
    typename... TAssociated>
auto makeGroupedCombinations(
    const TBucketPolicy& t_bucketPolicy, const int t_minCatSize, const TGrouping& grouping,
    const TAssociated&... associated
)
{
    constexpr int N     = sizeof...(TAssociated);
    using TProducerType = decltype([&]<size_t... Is>(std::index_sequence<Is...>) {
        return makeBlockCombinations<TCombinationsPolicy>(
            t_bucketPolicy, t_minCatSize, (static_cast<void>(Is), grouping.ProvidedSource)...
        );
    }(std::make_index_sequence<N>{}));
    return GroupedCombinations<
        TProducerType, TCombinationsPolicy, TBucketPolicy, TGrouping, TAssociated...>(
        t_bucketPolicy, t_minCatSize, grouping, associated...
    );
}

#endif  // COMET_GROUPEDCOMBINATIONS_H
