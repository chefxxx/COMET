//
// Created by mshamrai on 1/15/26.
//

#include <array>
#include <iostream>
#include <ostream>
#include <random>

#include "../lib/Combinations.h"
#include "../lib/Producers.h"
#include "../lib/BucketPolicy.h"

std::vector<int> generateData(int N)
{
    std::mt19937 rng;
    std::uniform_int_distribution uniform_int_distribution(0, 100);
    std::vector<int> data;
    data.reserve(N);
    for (int i = 0; i < N; ++i) {
        data.push_back(uniform_int_distribution(rng));
    }
    return data;
}


int main(int argc, char** argv)
{
    auto data = generateData(10);

    std::cout << "Simple full combinations:" << std::endl;
    std::cout << "---------------------------------------------------------------" << std::endl;

    auto combination = makeCombinations<FullCombinationsPolicy>(data, data, data, data);
    for (const auto& [el0, el1, el2, el3]: combination) {
        std::cout << el0 << " " << el1 << " " << el2 << " " << el3 << std::endl;
    }

    std::cout << "Simple StrictlyUpper combinations:" << std::endl;
    std::cout << "---------------------------------------------------------------" << std::endl;

    auto strictlyUpperCombinations = makeCombinations<FullCombinationsPolicy>(data, data, data, data);
    for (const auto& [el0, el1, el2, el3]: strictlyUpperCombinations) {
        std::cout << el0 << " " << el1 << " " << el2 << " " << el3 << std::endl;
    }

    std::cout << "Windowed (5) Full combinations:" << std::endl;
    std::cout << "---------------------------------------------------------------" << std::endl;

    data = generateData(100);
    auto sameKindCombination = makeSameKindCombinations<SameTypeFullCombinationsPolicy>(5, data, data, data, data);
    for (const auto& [el0, el1, el2, el3]: sameKindCombination) {
        std::cout << el0 << " " << el1 << " " << el2 << " " << el3 << std::endl;
    }

    std::cout << "Windowed (5) StrictlyUpper combinations:" << std::endl;
    std::cout << "---------------------------------------------------------------" << std::endl;

    auto strictlyUpperSameTypeCombinations = makeSameKindCombinations<SameTypeStrictlyUpperCombinationsPolicy>(5, data, data, data, data);
    for (const auto& [el0, el1, el2, el3]: strictlyUpperSameTypeCombinations) {
        std::cout << el0 << " " << el1 << " " << el2 << " " << el3 << std::endl;
    }

    std::cout << "Block full combinations (Only div 10 same are combined):" << std::endl;
    std::cout << "---------------------------------------------------------------" << std::endl;

    auto callable = [](const int& a) { return a%10; };
    std::array boundaries = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    auto bp = BucketPolicy(false, callable, boundaries);

    data = generateData(30);

    auto blockCombinations = makeBlockCombinations<FullCombinationsPolicy>(bp, 1, data, data);
    for (const auto& [el0, el1]: blockCombinations) {
        std::cout << el0 << " " << el1 << std::endl;
    }

    std::cout << "Block StrictlyUpper combinations (Only div 10 same are combined):" << std::endl;
    std::cout << "---------------------------------------------------------------" << std::endl;
    auto blockStrictlyUpperCombinations = makeBlockCombinations<StrictlyUpperCombinationsPolicy>(bp, 1, data, data);
    for (const auto& [el0, el1]: blockStrictlyUpperCombinations) {
        std::cout << el0 << " " << el1 << std::endl;
    }

    std::cout << "Block Grouped combinations (Only div 10 same are combined):" << std::endl;
    std::cout << "---------------------------------------------------------------" << std::endl;

    struct Associated
    {
        int id;
        double val;
    };

    std::vector<Associated> associated;
    associated.reserve(100);
    std::mt19937 rng;
    std::uniform_int_distribution uniform_int_distribution(0, 10);
    std::uniform_real_distribution<double> uniform_real_distribution(-1.0, 1.0);

    for (int i = 0; i < 100; ++i) {
        associated.push_back({uniform_int_distribution(rng), uniform_real_distribution(rng)});
    }
////////////////////////////////////////////////////////////////////////////////////////////////////////
///
///
////////////////////////////////////////////////////////////////////////////////////////////////////////
    auto getGroupedId = [](const int& elem) { return elem % 10; };
    auto getId = [](const Associated& elem) { return elem.id; };

    auto grouping = SourceWithCallable(data, getGroupedId);
    auto assoc = SourceWithCallable(associated, getId);

    auto getBucket = [](const int& elem) { return elem & 1; };

    std::array buckets {0, 1};
    auto nbp = BucketPolicy(false, getBucket, buckets);

    auto groupedCombinations = makeGroupedCombinations<FullCombinationsPolicy>(nbp, 1, grouping, assoc, assoc);

    for (const auto& [el0, sp0, el1, sp1]: groupedCombinations) {
        std::cout << el0 << ":" << el1 << std::endl;
        std::cout << el0 << std::endl;
        for (const auto& el: sp0) {
            std::cout << "          " << el.id << " " << el.val << std::endl;
        }
        std::cout << el1 << std::endl;
        for (const auto& el: sp1) {
            std::cout << "          " << el.id << " " << el.val << std::endl;
        }
    }
}