//
// Created by mshamrai on 1/17/26.
//

#include <random>

#include "../lib/BucketPolicy.h"
#include "../lib/Combinations.h"
#include "../lib/Producers.h"

struct AssociatedMock
{
    int globalIndex;
    int value;
};

void generate(const std::vector<int>& groupingData, const auto& groupingCallable,
    const std::vector<AssociatedMock>& associatedFirst, const auto& associatedFirsCallable,
    const std::vector<AssociatedMock>& associatedSecond, const auto& associatedSecondCallable);

auto getBucket = [](const int&) { return 1; };

auto getIdGrouping = [](const int& el) { return el; };

auto getIdAssociated = [](const AssociatedMock& as) { return as.globalIndex; };

int main(int, char**)
{
    int seed = 42;
    int meanMultiplicity = 20;

    std::mt19937 rng(seed);
    std::uniform_int_distribution uniform_int_distribution(-1000, 1000);
    std::poisson_distribution<int> poisson_distribution(meanMultiplicity);

    std::vector<int> data;
    std::vector<AssociatedMock> associatedFirst;
    std::vector<AssociatedMock> associatedSecond;


    int N = 10000;
    data.reserve(N);
    associatedFirst.reserve(N * meanMultiplicity);
    associatedSecond.reserve(N * meanMultiplicity);
    int multiplicity;
    for (int i = 0; i < N; ++i) {
        data.push_back(uniform_int_distribution(rng));
        multiplicity = poisson_distribution(rng);
        for (int j = 0; j < multiplicity; ++j) {
            associatedFirst.push_back(AssociatedMock(i, N + j));
        }
        multiplicity = poisson_distribution(rng);
        for (int j = 0; j < multiplicity; ++j) {
            associatedSecond.push_back(AssociatedMock(i, 2 * N + j));
        }
    }

    generate(data, getIdGrouping, associatedFirst, getIdAssociated, associatedSecond, getIdAssociated);
}

void generate(const std::vector<int>& groupingData, const auto& groupingCallable,
    const std::vector<AssociatedMock>& associatedFirst, const auto& associatedFirstCallable,
    const std::vector<AssociatedMock>& associatedSecond, const auto& associatedSecondCallable)
{
    std::array<int, 2> buckets {-1, 20};
    BucketPolicy bp = BucketPolicy(false, groupingCallable, buckets);

    auto groupingWrapped = SourceWithCallable(groupingData, groupingCallable);
    auto associatedFirstWrapped = SourceWithCallable(associatedFirst, associatedFirstCallable);
    auto associatedSecondWrapped = SourceWithCallable(associatedSecond, associatedSecondCallable);

    auto groupedCombinations = makeGroupedCombinations<FullCombinationsPolicy>(bp, 1, groupingWrapped, associatedFirstWrapped, associatedSecondWrapped);
    for (const auto& [el0, as0, el1, as1]: groupedCombinations) {
        sqrt(el0 + el1);
         auto combinations = makeCombinations<FullCombinationsPolicy>(as0, as1);
         for (const auto& [e0, e1]: combinations) {
             sqrt(e0.value + e1.value);
        }
    }
}