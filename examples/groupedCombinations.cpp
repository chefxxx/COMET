//
// Created by Mateusz Mikiciuk on 19/01/2026.
//

#include <random>
#include <iostream>

#include "include/Producers.h"
#include "include/Combinations.h"
#include "include/BucketPolicy.h"

auto generateData(const int N)
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

struct Associated
{
    int id;
    double val;
};

auto generateAssociated(const int N)
{
    std::vector<Associated> associated;
    associated.reserve(N);
    std::mt19937 rng;
    std::uniform_int_distribution uniform_int_distribution(0, 10);
    std::uniform_real_distribution uniform_real_distribution(-1.0, 1.0);
    for (int i = 0; i < N; ++i) {
        associated.push_back({uniform_int_distribution(rng), uniform_real_distribution(rng)});
    }
    return associated;
}


int main() {
    const auto data = generateData(30);
    const auto associated = generateAssociated(100);

    auto getGroupedId   = [](const int& elem) { return elem % 10; };
    auto getId          = [](const Associated& elem) { return elem.id; };
    const auto grouping = SourceWithCallable(data, getGroupedId);
    const auto assoc    = SourceWithCallable(associated, getId);

    std::array buckets {0, 1};
    auto getBucket = [](const int& elem) { return elem & 1; };
    const auto bucket_policy = BucketPolicy(false, getBucket, buckets);

    auto groupedCombinations = makeGroupedCombinations<FullCombinationsPolicy>(bucket_policy, 1, grouping, assoc, assoc);
    std::cout << "FullGrouped producer:\n";
    for (const auto& [el0, sp0, el1, sp1]: groupedCombinations) {
        // Do something with the generated combinations ...
        for (const auto& [inner0, inner1] : makeCombinations<FullCombinationsPolicy>(sp0, sp1))
        {
            std::cout << "Generated inner combination: [";
            std::cout << "(" << inner0.id <<  ", " << inner0.val << ") and ";
            std::cout << "(" << inner1.id <<  ", " << inner1.val << ")]\n";
        }
    }

    auto groupedCombinationsUpper = makeGroupedCombinations<StrictlyUpperCombinationsPolicy>(bucket_policy, 1, grouping, assoc, assoc);
    std::cout << "StrictlyUpperGrouped producer:\n";
    for (const auto& [el0, sp0, el1, sp1]: groupedCombinationsUpper) {
        // Do something with the generated combinations ...
        for (const auto& [inner0, inner1] : makeCombinations<StrictlyUpperCombinationsPolicy>(sp0, sp1))
        {
            std::cout << "Generated inner combination: [";
            std::cout << "(" << inner0.id <<  ", " << inner0.val << ") and ";
            std::cout << "(" << inner1.id <<  ", " << inner1.val << ")]\n";
        }
    }
}
