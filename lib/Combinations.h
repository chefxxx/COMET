//
// Created by Mateusz Mikiciuk on 22/07/2025.
//

#ifndef COMBINATIONS_H
#define COMBINATIONS_H

#include "Helpers.h"

template <typename... Ts>
struct CombinationsPolicyBase {
    public:
    private:
        std::tuple<typename Ts::iterator...> currentState;
};

template <typename... Ts>
struct FullCombinationsPolicy : CombinationsPolicyBase<Ts...> {
};

template <typename... Ts>
struct StrictlyUpperCombinationsPolicy : CombinationsPolicyBase<Ts...> {
};

#endif  // COMBINATIONS_H
