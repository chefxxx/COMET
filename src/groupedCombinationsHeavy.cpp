//
// Created by mshamrai on 1/17/26.
//

#include "../benchmarks/helpers/HeavyStructures.h"
#include "../lib/BucketPolicy.h"
#include "../lib/Producers.h"
#include "../lib/Combinations.h"


void generate(const std::vector<HeavyGroupingStruct>& groupingData, const auto& groupingCallable,
    const std::vector<HeavyAssociatedFirstStruct>& associatedFirst, const auto& associatedFirsCallable,
    const std::vector<HeavyAssociatedSecondStruct>& associatedSecond, const auto& associatedSecondCallable);

int main(int, char**)
{
    std::vector<HeavyGroupingStruct> data;
    std::vector<HeavyAssociatedFirstStruct> associatedFirst;
    std::vector<HeavyAssociatedSecondStruct> associatedSecond;

    int seed = 42;
    int meanMultiplicity = 20;

    HeavyGroupingStructGenerator<int> generator = HeavyGroupingStructGenerator(seed, meanMultiplicity);
    HeavyAssociatedFirstStructGenerator firstAssociatedGenerator = HeavyAssociatedFirstStructGenerator(seed);
    HeavyAssociatedSecondStructGenerator secondAssociatedGenerator = HeavyAssociatedSecondStructGenerator();

    int N = 15000;
    data.reserve(N);
    associatedFirst.reserve(N * meanMultiplicity);
    associatedSecond.reserve(N * meanMultiplicity);
    for (int i = 0; i < N; ++i) {
        data.push_back(generator.generate(i));
        for (int j = 0; j < data[i].multiplicityFirst; ++j) {
            associatedFirst.push_back(firstAssociatedGenerator.generate(data[i].globalIndex, N * i + j));
        }
        for (int j = 0; j < data[i].multiplicitySecond; ++j) {
            associatedSecond.push_back(secondAssociatedGenerator.generate(data[i].globalIndex, N * i + j));
        }
    }


    generate(data, GetIdHeavyGroupingCallable{}, associatedFirst, GetIdHeavyAssociatedFirstStructCallable{},
        associatedSecond, GetIdHeavyAssociatedSecondStructCallable{});
}

void generate(const std::vector<HeavyGroupingStruct>& groupingData, const auto& groupingCallable,
    const std::vector<HeavyAssociatedFirstStruct>& associatedFirst, const auto& associatedFirstCallable,
    const std::vector<HeavyAssociatedSecondStruct>& associatedSecond, const auto& associatedSecondCallable)
{
    std::array<int, 2> buckets {-1, 20};
    BucketPolicy bp = BucketPolicy(false, groupingCallable, buckets);

    auto groupingWrapped = SourceWithCallable(groupingData, groupingCallable);
    auto associatedFirstWrapped = SourceWithCallable(associatedFirst, associatedFirstCallable);
    auto associatedSecondWrapped = SourceWithCallable(associatedSecond, associatedSecondCallable);

    auto groupedCombinations = makeGroupedCombinations<FullCombinationsPolicy>(bp, 1, groupingWrapped, associatedFirstWrapped, associatedSecondWrapped);
    for (const auto& [el0, as0, el1, as1]: groupedCombinations) {
        sqrt(el0.globalIndex + el1.globalIndex);
        auto combinations = makeCombinations<FullCombinationsPolicy>(as0, as1);
        for (const auto& [e0, e1]: combinations) {
            sqrt(e0.groupingIndex + e1.globalIndex);
        }
    }
}