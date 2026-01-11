//
// Created by Mateusz Mikiciuk on 23/12/2025.
//

#ifndef PRODUCERS_H
#define PRODUCERS_H

#include <tuple>
#include "BucketSortView.h"
#include "Helpers.h"

/**
 * @brief This class just serves as an iterable wrapper around policies.
 * @tparam TCombinationsPolicy policy object that defines behaviour of the iterator
 */
template <typename TCombinationsPolicy>
struct CombinationsProducer {
    // This constructor is used in makeCombinations func
    explicit CombinationsProducer(TCombinationsPolicy &&policy) : m_policy(std::move(policy)) {}

    struct CombinationsSentinel {
    };

    struct CombinationsIterator {
        using iterator_category = std::input_iterator_tag;
        using difference_type   = std::ptrdiff_t;
        using value_type        = typename TCombinationsPolicy::combinations_value;
        using pointer           = void;
        using reference         = typename TCombinationsPolicy::combinations_reference;

        CombinationsIterator() = default;
        explicit CombinationsIterator(TCombinationsPolicy *t_policy) : m_policyPtr(t_policy) {}
        CombinationsIterator(const CombinationsIterator &)            = default;
        CombinationsIterator &operator=(const CombinationsIterator &) = default;

        CombinationsIterator &operator++()
        {
            m_policyPtr->addOne();
            return *this;
        }
        CombinationsIterator operator++(int)
        {
            CombinationsIterator copy = *this;
            ++(*this);
            return copy;
        }
        // dereference each pointer in the current tuple
        reference operator*() const
        {
            return std::apply(
                [](auto &&...args) {
                    return std::forward_as_tuple(*args...);
                },
                m_policyPtr->current()
            );
        }

        // Actually this is not needed directly by our lib,
        // some third-party libs may need it.
        // If both iterators point to the same policy,
        // they are considered equal.
        friend bool operator==(const CombinationsIterator &lhs, const CombinationsIterator &rhs)
        {
            return lhs.m_policyPtr == rhs.m_policyPtr;
        }
        // This is the version that actually is used in 'for' loops.
        friend bool operator==(const CombinationsIterator &lhs, CombinationsSentinel)
        {
            return lhs.m_policyPtr->isEnd();
        }

        private:
        TCombinationsPolicy *m_policyPtr = nullptr;
    };

    [[nodiscard]] CombinationsIterator begin() { return CombinationsIterator(&m_policy); }
    [[nodiscard]] CombinationsSentinel end() { return CombinationsSentinel{}; }
    [[nodiscard]] bool isEnd() const { return m_policy.isEnd(); }

    private:
    TCombinationsPolicy m_policy;
};

template <template <typename...> class TCombinationsPolicy, typename... TInputs>
auto makeCombinations(const TInputs &...t_inputs)
{
    using PolicyType = TCombinationsPolicy<TInputs...>;
    return CombinationsProducer<PolicyType>(PolicyType(t_inputs...));
}

template <typename TBucketPolicy, typename TCombinationsPolicy, typename... TInputs>
struct BlockProducer {
    explicit BlockProducer(
        const TBucketPolicy &t_bucketPolicy, TCombinationsPolicy &&t_combinationsPolicy,
        const int t_minCatSize, const TInputs &...t_inputs
    )
        : m_bucketPolicy(t_bucketPolicy),
          m_groupedData(t_bucketPolicy, t_minCatSize, t_inputs...),
          m_combinationsPolicy(std::move(t_combinationsPolicy))
    {
    }

    struct BlockSentinel {
    };
    struct BlockIterator {
        using iterator_category = std::input_iterator_tag;
        using difference_type   = std::ptrdiff_t;
        using value_type        = std::tuple<typename TInputs::value_type...>;
        using pointer           = void;
        using reference         = std::tuple<
            typename std::iterator_traits<typename TInputs::const_iterator>::reference...>;

        BlockIterator() = default;
        explicit BlockIterator(
            TCombinationsPolicy *t_policy, CoupledBlockBuckets<TBucketPolicy, TInputs...> *t_coupled
        )
            : m_dataPtr(t_coupled), m_policyPtr(t_policy)
        {
            std::apply(
                [&](auto const &...buckets) {
                    m_policyPtr->setData(buckets...);
                },
                m_dataPtr->currentBuckets()
            );
        }

        BlockIterator &operator++()
        {
            m_policyPtr->addOne();
            if (m_policyPtr->isEnd()) {
                m_dataPtr->nextIterators();
                if (!m_dataPtr->isEnd()) {
                    std::apply(
                        [&](auto const &...buckets) {
                            m_policyPtr->setData(buckets...);
                        },
                        m_dataPtr->currentBuckets()
                    );
                }
            }
            return *this;
        }

        BlockIterator operator++(int)
        {
            BlockIterator copy = *this;
            ++(*this);
            return copy;
        }

        reference operator*() const
        {
            // TODO: why do we use this type instead of tuple of iterators
            return std::apply(
                [](auto &&...args) {
                    return std::forward_as_tuple(**args...);
                },
                m_policyPtr->current()
            );
        }

        // Actually this is not needed directly by our lib,
        // some third-party libs may need it.
        // If both iterators point to the same policy,
        // they are considered equal.
        friend bool operator==(const BlockIterator &lhs, const BlockIterator &rhs)
        {
            return lhs.m_policyPtr == rhs.m_policyPtr;
        }

        // This is the version that actually is used in 'for' loops.
        friend bool operator==(const BlockIterator &lhs, BlockSentinel)
        {
            return lhs.m_dataPtr->isEnd();
        }

        private:
        CoupledBlockBuckets<TBucketPolicy, TInputs...> *m_dataPtr;
        TCombinationsPolicy *m_policyPtr;
    };

    [[nodiscard]] BlockIterator begin()
    {
        return BlockIterator(&m_combinationsPolicy, &m_groupedData);
    }
    [[nodiscard]] BlockSentinel end() { return BlockSentinel{}; }
    using iterator = BlockIterator;

    private:
    TBucketPolicy m_bucketPolicy;
    CoupledBlockBuckets<TBucketPolicy, TInputs...> m_groupedData;
    TCombinationsPolicy m_combinationsPolicy;
};

template <
    template <typename...> class TCombinationsPolicy, typename TBucketPolicy, typename... TInputs>
auto makeBlockCombinations(
    const TBucketPolicy &t_bucketPolicy, const int t_minCatSize, const TInputs &...t_inputs
)
{
    using PolicyType = TCombinationsPolicy<std::vector<typename TInputs::const_iterator>...>;
    return BlockProducer<TBucketPolicy, PolicyType, TInputs...>(
        t_bucketPolicy, PolicyType{}, t_minCatSize, t_inputs...
    );
}

template <typename TSource, typename TCallable>
struct SourceWithCallable {
    // TODO: Change the whole wrapper, so it accepts the rvalue?
    using source   = TSource;
    using callable = TCallable;
    const source &ProvidedSource;
    callable ProvidedCallable;
    SourceWithCallable(const TSource &s, TCallable c) : ProvidedSource(s), ProvidedCallable(c) {}
    SourceWithCallable(TSource &&, TCallable) = delete;
};

template <typename TGrouping, typename TAssociated>
concept ValidAssociated = AssociatedCallableResultIsConvertibleToGrouping<
    typename TGrouping::source, typename TGrouping::callable, typename TAssociated::source,
    typename TAssociated::callable>;

template <
    typename TProducerType, template <typename...> typename TCombinationsPolicy,
    typename TBucketPolicy, typename TGrouping, typename... TAssociated>
    requires(ValidAssociated<TGrouping, TAssociated> && ...)
struct GroupedProducer {
    using GroupingCallableType = typename TGrouping::callable;
    using ViewsType            = std::tuple<BucketSortView<
        typename TGrouping::source, typename TGrouping::callable, typename TAssociated::source,
        typename TAssociated::callable>...>;

    template <std::size_t... Is>
    static TProducerType
    makeProducer(const TBucketPolicy &bucketPolicy, int minCatSize, const TGrouping &grouping, std::index_sequence<Is...>)
    {
        return makeBlockCombinations<TCombinationsPolicy>(
            bucketPolicy, minCatSize, (static_cast<void>(Is), grouping.ProvidedSource)...
        );
    }

    explicit GroupedProducer(
        const TBucketPolicy &t_bucketPolicy, const int t_minCatSize, const TGrouping &grouping,
        const TAssociated &...associated
    )
        // TODO: Inspect what is going on with the views on creating
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
    using AssociatedTupleType = typename ExtractSpanViewTypes<decltype(m_views)>::type;
    using ResultTupleType     = typename InterleaveWithTuple<
        typename TGrouping::source::value_type, AssociatedTupleType>::type;
    using ResultTupleTypeRef = typename InterleaveWithTuple<
        typename std::iterator_traits<typename TGrouping::source::const_iterator>::reference,
        AssociatedTupleType>::type;

    struct GroupingSentinel {
    };
    struct GroupedIterator {
        using iterator_category = std::input_iterator_tag;
        using difference_type   = std::ptrdiff_t;
        using value_type        = ResultTupleType;
        using pointer           = void;
        // TODO: Here is something bad with objects lifetime, the value_type is safer but may eat
        // more memory
        using reference = ResultTupleTypeRef;

        using BlockIteratorType    = typename TProducerType::iterator;
        using GroupingCallableType = typename TGrouping::callable;

        GroupedIterator() = default;
        GroupedIterator(
            ViewsType *viewsPtr, GroupingCallableType *callablePtr,
            TProducerType *combinationsProducerPtr
        )
            : m_viewsPtr(viewsPtr),
              m_callablePtr(callablePtr),
              m_combinationsProducerPtr(combinationsProducerPtr),
              m_blockIteratorPtr(combinationsProducerPtr->begin())
        {
        }

        GroupedIterator &operator++()
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

        auto operator*() const
        {
            // TODO: Check if std::forward_as_tuple works here
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

        friend bool operator==(const GroupedIterator &lhs, const GroupedIterator &rhs)
        {
            return lhs.m_blockIteratorPtr == rhs.m_blockIteratorPtr;
        }

        friend bool operator==(const GroupedIterator &lhs, GroupingSentinel)
        {
            return lhs.m_blockIteratorPtr == lhs.m_combinationsProducerPtr->end();
        }

        private:
        ViewsType *m_viewsPtr;
        GroupingCallableType *m_callablePtr;
        TProducerType *m_combinationsProducerPtr;
        // TODO: change name here and change the callable to value?
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
    const TBucketPolicy &t_bucketPolicy, const int t_minCatSize, const TGrouping &grouping,
    const TAssociated &...associated
)
{
    constexpr int N     = sizeof...(TAssociated);
    using TProducerType = decltype([&]<size_t... Is>(std::index_sequence<Is...>) {
        return makeBlockCombinations<TCombinationsPolicy>(
            t_bucketPolicy, t_minCatSize, (static_cast<void>(Is), grouping.ProvidedSource)...
        );
    }(std::make_index_sequence<N>{}));
    return GroupedProducer<
        TProducerType, TCombinationsPolicy, TBucketPolicy, TGrouping, TAssociated...>(
        t_bucketPolicy, t_minCatSize, grouping, associated...
    );
}

#endif  // PRODUCERS_H
