//
// Created by Mateusz Mikiciuk on 15/12/2025.
//

#ifndef COMBINATIONSREWORK_H
#define COMBINATIONSREWORK_H

// TODO: concepts restraining TInputs and TCombinationsPolicy

#include <array>

template <typename Derived, typename... TInputs>
class CombinationsProducer {
public:
    void addOne()
    {
        static_cast<Derived*>(this)->addOneImpl();
    }
    void setData()
    {
        static_cast<Derived*>(this)->setDataImpl();
    }
protected:
    bool m_isEnd = false;
    std::array<int64_t, sizeof...(TInputs)> m_endIndexNumbers;
    std::array<int64_t, sizeof...(TInputs)> m_currentIndexNumbers;
};

template <typename... TInputs>
class FullCombinationsProducer : public CombinationsProducer<FullCombinationsProducer<TInputs...>, TInputs...> {
public:
    explicit FullCombinationsProducer(TInputs &...t_inputs)
    {
        auto tuple = std::make_tuple(t_inputs...);
    }
};

template <typename... TInputs>
class StrictlyUpperCombinationsProducer : public CombinationsProducer<StrictlyUpperCombinationsProducer<TInputs...>, TInputs...> {

};

// template <template <typename> class TCombinationsPolicy, typename... TInputs>
// auto makeCombinations(const TInputs &...t_inputs)
// {
//
// }

#endif //COMBINATIONSREWORK_H
