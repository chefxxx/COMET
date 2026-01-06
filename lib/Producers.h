//
// Created by Mateusz Mikiciuk on 23/12/2025.
//

#ifndef PRODUCERS_H
#define PRODUCERS_H

#include <tuple>
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

template <template <typename...> class TCombinationsPolicy, typename... TInputs>
auto makeSameTypeCombinations(int categoryNeighbours, const TInputs &...t_inputs)
{
    using PolicyType = TCombinationsPolicy<TInputs...>;
    return CombinationsProducer<PolicyType>(PolicyType(categoryNeighbours, t_inputs...));
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

#endif  // PRODUCERS_H
