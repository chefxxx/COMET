//
// Created by Mateusz Mikiciuk on 22/07/2025.
//

#ifndef COMBINATIONS_H
#define COMBINATIONS_H

#include <iostream>

struct FullCombinations {
    int P = 10;

    FullCombinations(int P_) : P(P_) {}

    void func(int xd)
    {
        for (int i = 1; i <= P; i++) std::cout << i << '\n';
    }
};

struct StrictlyUpperCombinations {
};

#endif  // COMBINATIONS_H
