//
// Created by Mateusz Mikiciuk on 19/01/2026.
//

#include <iostream>
#include <deque>
#include <vector>

#include "Combinations.h"
#include "Producers.h"

int main() {
    // containers that satisfy random access iterator concept
    const std::vector s1 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    const std::array  s2 = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};
    const std::deque  s3 = {"mat", "pat", "cat", "fat", "rat", "pat"};

    constexpr int maxNeighbours = 5;

    auto combinationsProducer = makeCombinations<FullCombinationsPolicy>(s1, s2, s3);
    std::cout << "FullCombinations producer:\n";
    for (const auto& [elem1, elem2, elem3] : combinationsProducer) {
        // Do something with the created combination ...
        std::cout << "Produced tuple: (" << elem1 << ", " << elem2 << ", " << elem3 << ")\n";
    }

    auto strictlyUpperCombinationsProducer = makeCombinations<StrictlyUpperCombinationsPolicy>(s1, s2, s3);
    std::cout << "StrictlyUpper producer:\n";
    for (const auto& [elem1, elem2, elem3] : strictlyUpperCombinationsProducer) {
        // Do something with the created combination ...
        std::cout << "Produced tuple: (" << elem1 << ", " << elem2 << ", " << elem3 << ")\n";
    }

    auto sameKindFull = makeSameKindCombinations<SameTypeFullCombinationsPolicy>(maxNeighbours, s1, s1);
    std::cout << "SameKindFull producer:\n";
    for (const auto& [elem1, elem2] : sameKindFull) {
        // Do something with the created combination ...
        std::cout << "Produced tuple: (" << elem1 << ", " << elem2 << ")\n";
    }

    auto sameKindStrictlyUpper = makeSameKindCombinations<SameTypeStrictlyUpperCombinationsPolicy>(maxNeighbours, s2, s2);
    for (const auto& [elem1, elem2] : sameKindStrictlyUpper) {
        // Do something with the created combination ...
        std::cout << "Produced tuple: (" << elem1 << ", " << elem2 << ")\n";
    }
}