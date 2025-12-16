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
    void addOne()
    {
        if (!m_isEnd) {
            const auto &underlying = static_cast<const Derived &>(*this);
            constexpr size_t N     = sizeof...(TInputs);
            bool wasModified       = true;
            [&]<std::size_t... Is>(const std::index_sequence<Is...> &) {
                ((static_cast<Derived *>(this)->template addOneImpl<Is, N>(wasModified)), ...);
            }(std::make_index_sequence<N>());
            m_isEnd = wasModified;
        }
    }
    void setData(const TInputs &...t_inputs)
    {
        static_cast<Derived *>(this)->setDataImpl(t_inputs...);
    }

    protected:
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
};

template <typename... TInputs>
class FullCombinationsProducer
    : public CombinationsProducer<FullCombinationsProducer<TInputs...>, TInputs...>
{
    public:
    explicit FullCombinationsProducer(const TInputs &...t_inputs)
    {
        this->setData(t_inputs...);
    }
    void setDataImpl(const TInputs &...t_inputs)
    {
        [&]<std::size_t... Is>(const std::index_sequence<Is...> &) {
            ((this->template setDataHelper<Is>(t_inputs)), ...);
            this->m_isEnd = ((this->m_endIndexNumbers[Is] == 0) || ...);
        }(std::make_index_sequence<sizeof...(TInputs)>{});
    }
    template <size_t I, size_t N>
    void addOneImpl(bool &t_wasModified)
    {
        if (t_wasModified) {
            constexpr auto ind           = N - I - 1;
            int64_t currentPointersIndex = ++this->m_currentIndexNumbers[ind];
            ++std::get<ind>(this->m_current);
            if (currentPointersIndex != this->m_endIndexNumbers[ind]) {
                [&]<std::size_t... Is>(const std::index_sequence<Is...> &) {
                    (resetState<I, Is, N>(), ...);
                }(std::make_index_sequence<I>());
                t_wasModified = false;
            }
        }
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
