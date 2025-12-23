//
// Created by Mateusz Mikiciuk on 23/12/2025.
//

#ifndef COMBINATIONS_H
#define COMBINATIONS_H

#include <iterator>
#include <tuple>

/**
 * @brief This class just serves as an iterable wrapper around policies.
 * @tparam TCombinationsPolicy policy object that defines behaviour of the iterator
 */
template <typename TCombinationsPolicy>
struct CombinationsProducer {
    // This constructor is used in makeCombinations func
    explicit CombinationsProducer(TCombinationsPolicy policy) : m_policy(std::move(policy)) {}

    struct CombinationsSentinel {
    };

    struct CombinationsIterator {
        using iterator_category = std::input_iterator_tag;
        using difference_type   = std::ptrdiff_t;
        using value_type        = typename TCombinationsPolicy::CombinationsValue;
        using pointer           = void;
        using reference         = typename TCombinationsPolicy::CombinationsReference;

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

template <typename Derived, typename... TInputs>
struct CombinationsPolicyBase {
    using CombinationsType  = std::tuple<typename TInputs::const_iterator...>;
    using CombinationsValue = std::tuple<typename TInputs::value_type...>;
    using CombinationsReference =
        std::tuple<typename std::iterator_traits<typename TInputs::const_iterator>::reference...>;

    // interface functions for CombinationsProducer
    [[nodiscard]] bool isEnd() const { return m_isEnd; }
    [[nodiscard]] CombinationsType &current() { return m_current; }

    protected:
    void addOneBaseImpl()
    {
        if (!m_isEnd) {
            constexpr size_t N = sizeof...(TInputs);
            bool wasModified   = true;
            [&]<std::size_t... Is>(const std::index_sequence<Is...> &) {
                (addOneHelper<Is, N>(wasModified), ...);
            }(std::make_index_sequence<N>());
            m_isEnd = wasModified;
        }
    }

    void setDataBaseImpl(const TInputs &...t_inputs)
    {
        [&]<std::size_t... Is>(const std::index_sequence<Is...> &) {
            ((setDataHelper<Is>(t_inputs)), ...);
            this->m_isEnd = ((this->m_endIndexNumbers[Is] == 0) || ...);
        }(std::make_index_sequence<sizeof...(TInputs)>{});
    }

    bool m_isEnd = false;
    std::array<int64_t, sizeof...(TInputs)> m_endIndexNumbers;
    std::array<int64_t, sizeof...(TInputs)> m_currentIndexNumbers;
    CombinationsType m_sentinel;
    CombinationsType m_current;

    private:
    template <size_t I, typename TInput>
    void setDataHelper(const TInput &t_input)
    {
        std::get<I>(m_current)   = t_input.begin();
        std::get<I>(m_sentinel)  = t_input.end();
        m_currentIndexNumbers[I] = 0;
        m_endIndexNumbers[I]     = std::distance(t_input.begin(), t_input.end());
    }

    template <size_t I, size_t N>
    void addOneHelper(bool &t_wasModified)
    {
        if (t_wasModified) {
            constexpr auto ind           = N - I - 1;
            int64_t currentPointersIndex = ++m_currentIndexNumbers[ind];
            ++std::get<ind>(m_current);
            if (currentPointersIndex != m_endIndexNumbers[ind]) {
                static_cast<Derived *>(this)->template addOneImpl<I, N>(t_wasModified);
            }
        }
    }
};

template <typename... TInputs>
class FullCombinationsPolicy
    : public CombinationsPolicyBase<FullCombinationsPolicy<TInputs...>, TInputs...>
{
    public:
    explicit FullCombinationsPolicy(const TInputs &...t_inputs) { setData(t_inputs...); }

    void setData(const TInputs &...t_inputs) { this->setDataBaseImpl(t_inputs...); }
    void addOne() { this->addOneBaseImpl(); }

    template <size_t I, size_t N>
    void addOneImpl(bool &t_wasModified)
    {
        [&]<std::size_t... Is>(const std::index_sequence<Is...> &) {
            (resetState<I, Is, N>(), ...);
        }(std::make_index_sequence<I>());
        t_wasModified = false;
    }

    private:
    // N - number of data sources
    // I - which position from the right side is considered
    // J - loop iterator, which pointer is set to 0 from the N - I to right position
    template <size_t I, size_t J, size_t N>
    void resetState()
    {
        constexpr auto ind = N - I + J;
        std::get<ind>(this->m_current) -= this->m_currentIndexNumbers[ind];
        this->m_currentIndexNumbers[ind] = 0;
    }
};

template <typename... TInputs>
class StrictlyUpperCombinationsPolicy
    : public CombinationsPolicyBase<StrictlyUpperCombinationsPolicy<TInputs...>, TInputs...>
{
    public:
    explicit StrictlyUpperCombinationsPolicy(const TInputs &...t_inputs) { setData(t_inputs...); }

    void setData(const TInputs &...t_inputs)
    {
        this->setDataBaseImpl(t_inputs...);
        // Set ranges here have the same logic as a loop into ... with setting new
        // pointers for right side positions.
        if (!this->m_isEnd) {
            constexpr auto N = sizeof...(t_inputs);
            bool shouldEnd   = true;
            setRanges<N - 1, N>(shouldEnd);
            this->m_isEnd = !shouldEnd;
        }
    }

    void addOne() { this->addOneBaseImpl(); }

    template <size_t I, size_t N>
    void addOneImpl(bool &t_wasModified)
    {
        bool wasChanged = true;
        setRanges<I, N>(wasChanged);
        t_wasModified = !wasChanged;
    }

    private:
    template <size_t I, size_t N>
    void setRanges(bool &t_condition)
    {
        [&]<std::size_t... Is>(const std::index_sequence<Is...> &) {
            ((resetState<I, Is, N>(t_condition)), ...);
        }(std::make_index_sequence<I>());
    }

    // Here there are changes from the Full version. The pointers must be set not to 0, but to
    // Position + 1 from the nearest left pointer.
    // N - number of data sources
    // I - which position from the right side is considered
    // J - loop iterator, which pointer is set to 0 from the N - I to right position
    template <size_t I, size_t J, size_t N>
    void resetState(bool &t_condition)
    {
        if (t_condition) {
            constexpr auto ind = N - I + J;
            int64_t tmpInd     = this->m_currentIndexNumbers[ind - 1] + 1;
            if (tmpInd < this->m_endIndexNumbers[ind]) {
                std::get<ind>(this->m_current) += tmpInd - this->m_currentIndexNumbers[ind];
                this->m_currentIndexNumbers[ind] = tmpInd;
            } else {
                t_condition = false;
            }
        }
    }
};

template <template <typename...> class TCombinationsPolicy, typename... TInputs>
auto makeCombinations(const TInputs &...t_inputs)
{
    using PolicyType = TCombinationsPolicy<TInputs...>;
    return CombinationsProducer<PolicyType>(PolicyType(t_inputs...));
}

#endif  // COMBINATIONS_H
