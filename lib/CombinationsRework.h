//
// Created by Mateusz Mikiciuk on 15/12/2025.
//

#ifndef COMBINATIONS_REWORK_H
#define COMBINATIONS_REWORK_H

// TODO: concepts restraining TInputs and TCombinationsPolicy

#include <array>

template <typename Derived, typename... TInputs>
class CombinationsProducer
{
    using CombinationsType = std::tuple<typename TInputs::const_iterator...>;

    public:
    void addOne() { static_cast<Derived *>(this)->addOneDerived(); }
    void setData(const TInputs &...t_inputs)
    {
        static_cast<Derived *>(this)->setDataDerived(t_inputs...);
    }

    struct CombinationsIterator {
        using iterator_category = std::input_iterator_tag;
        using difference_type   = std::ptrdiff_t;
        using value_type        = std::tuple<typename TInputs::value_type...>;
        using pointer           = void;
        using reference         = std::tuple<
                    typename std::iterator_traits<typename TInputs::const_iterator>::reference...>;

        CombinationsType *mCombinationsPtr;
        CombinationsProducer *mProducerPtr;
        CombinationsIterator() = default;
        explicit CombinationsIterator(CombinationsType *state, CombinationsProducer *producer)
            : mCombinationsPtr(state), mProducerPtr(producer)
        {
        }
        CombinationsIterator(const CombinationsIterator &)            = default;
        CombinationsIterator &operator=(const CombinationsIterator &) = default;

        CombinationsIterator &operator++()
        {
            mProducerPtr->addOne();
            mCombinationsPtr = &mProducerPtr->m_current;
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
                *mCombinationsPtr
            );
        }

        friend bool operator==(const CombinationsIterator &lhs, const CombinationsIterator &rhs)
        {
            if (lhs.mProducerPtr->m_isEnd && rhs.mProducerPtr->m_isEnd)
                return true;
            return lhs.mProducerPtr == rhs.mProducerPtr &&
                   lhs.mCombinationsPtr == rhs.mCombinationsPtr;
        }
        friend bool operator!=(const CombinationsIterator &lhs, const CombinationsIterator &rhs)
        {
            return !(lhs == rhs);
        }
    };

    [[nodiscard]] CombinationsIterator begin() { return CombinationsIterator(&m_current, this); }
    [[nodiscard]] CombinationsIterator end() { return CombinationsIterator(&m_sentinel, this); }
    [[nodiscard]] bool isEnd() const { return m_isEnd; }

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

    template <size_t I, typename TInput>
    void setDataHelper(const TInput &t_input)
    {
        std::get<I>(m_current)   = t_input.begin();
        std::get<I>(m_sentinel)  = t_input.end();
        m_currentIndexNumbers[I] = 0;
        m_endIndexNumbers[I]     = std::distance(t_input.begin(), t_input.end());
    }

    bool m_isEnd = false;
    std::array<int64_t, sizeof...(TInputs)> m_endIndexNumbers;
    std::array<int64_t, sizeof...(TInputs)> m_currentIndexNumbers;
    CombinationsType m_sentinel;
    CombinationsType m_current;

    private:
    template <size_t I, size_t N>
    void addOneHelper(bool &t_wasModified)
    {
        if (t_wasModified) {
            constexpr auto ind           = N - I - 1;
            int64_t currentPointersIndex = ++m_currentIndexNumbers[ind];
            ++std::get<ind>(m_current);
            if (currentPointersIndex != m_endIndexNumbers[ind]) {
                static_cast<Derived *>(this)->template addOneImpl<I, N>();
                t_wasModified = false;
            }
        }
    }
};

template <typename... TInputs>
class FullCombinationsProducer
    : public CombinationsProducer<FullCombinationsProducer<TInputs...>, TInputs...>
{
    public:
    explicit FullCombinationsProducer(const TInputs &...t_inputs) { setDataDerived(t_inputs...); }

    void setDataDerived(const TInputs &...t_inputs)
    {
        [&]<std::size_t... Is>(const std::index_sequence<Is...> &) {
            ((this->template setDataHelper<Is>(t_inputs)), ...);
            this->m_isEnd = ((this->m_endIndexNumbers[Is] == 0) || ...);
        }(std::make_index_sequence<sizeof...(TInputs)>{});
    }

    void addOneDerived() { this->addOneBaseImpl(); }

    template <size_t I, size_t N>
    void addOneImpl()
    {
        [&]<std::size_t... Is>(const std::index_sequence<Is...> &) {
            (resetState<I, Is, N>(), ...);
        }(std::make_index_sequence<I>());
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
class StrictlyUpperCombinationsProducer
    : public CombinationsProducer<StrictlyUpperCombinationsProducer<TInputs...>, TInputs...>
{
};

#endif  // COMBINATIONS_REWORK_H
