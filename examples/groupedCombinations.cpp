//
// Created by Mateusz Mikiciuk on 19/01/2026.
//

#include <random>
#include <iostream>

#include "Producers.h"
#include "Combinations.h"
#include "BucketPolicy.h"

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
    auto getGroupedId = [](const int& elem) { return elem % 10; };
    auto getId = [](const Associated& elem) { return elem.id; };
    const auto grouping = SourceWithCallable(data, getGroupedId);
    const auto assoc = SourceWithCallable(associated, getId);
    auto getBucket = [](const int& elem) { return elem & 1; };
    std::array buckets {0, 1};
    const auto bucket_policy = BucketPolicy(false, getBucket, buckets);


    auto groupedCombinations = makeGroupedCombinations<FullCombinationsPolicy>(bucket_policy, 1, grouping, assoc, assoc);
    for (const auto& [el0, sp0, el1, sp1]: groupedCombinations) {
        // Do something with the generated combinations ...
        std::cout << "Generated combination: (" << el0 << ", " << el1 << ")\n";
        std::cout << "Associated data with the element: " << el0 << ")\n";
        for (const auto& el: sp0) {
            std::cout << "          " << el.id << " " << el.val << '\n';
        }
        std::cout << "Associated data with the element: " << el1 << ")\n";
        for (const auto& el: sp1) {
            std::cout << "          " << el.id << " " << el.val << '\n';
        }
    }
}
