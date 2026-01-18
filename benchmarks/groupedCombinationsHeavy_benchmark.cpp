//
// Created by mshamrai on 1/18/26.
//
#include <benchmark/benchmark.h>
#include <vector>

#include "BucketPolicy.h"
#include "Combinations.h"
#include "Producers.h"

#include "HeavyStructures.h"

class CombinationsFixtureHeavy : public benchmark::Fixture
{
    public:
    int seed             = 42;
    int meanMultiplicity = 20;

    struct GetBucketCallable {
        int operator()(const HeavyGroupingStruct& element) const { return 1; }
    };

    std::mt19937 rng = std::mt19937(seed);

    std::vector<HeavyGroupingStruct> groupingData;
    std::vector<HeavyAssociatedFirstStruct> associatedFirst;
    std::vector<HeavyAssociatedSecondStruct> associatedSecond;
    std::vector<HeavyAssociatedThirdStruct> associatedThird;

    HeavyGroupingStructGenerator<int, int, int> generator =
        HeavyGroupingStructGenerator(seed, meanMultiplicity, meanMultiplicity, meanMultiplicity);
    HeavyAssociatedFirstStructGenerator firstAssociatedGenerator =
        HeavyAssociatedFirstStructGenerator(seed);
    HeavyAssociatedSecondStructGenerator secondAssociatedGenerator =
        HeavyAssociatedSecondStructGenerator();
    HeavyAssociatedThirdStructGenerator thirdAssociatedGenerator =
        HeavyAssociatedThirdStructGenerator();

    std::uniform_int_distribution<int> uniform_int_distribution =
        std::uniform_int_distribution(-1000, 1000);

    std::array<int, 2> buckets = {0, 1};
    int minCatSize             = 1;

    void SetUp(const ::benchmark::State& state)
    {
        // N elements
        auto N = state.range(0);
        groupingData.reserve(N);
        associatedFirst.reserve(meanMultiplicity * N);
        associatedSecond.reserve(meanMultiplicity * N);
        for (int i = 0; i < N; ++i) {
            groupingData.push_back(generator.generate(i));
            for (int j = 0; j < groupingData[i].multiplicityFirst; ++j) {
                associatedFirst.push_back(
                    firstAssociatedGenerator.generate(i, uniform_int_distribution(rng))
                );
            }
            for (int j = 0; j < groupingData[i].multiplicitySecond; ++j) {
                associatedSecond.push_back(
                    secondAssociatedGenerator.generate(i, uniform_int_distribution(rng))
                );
            }
            for (int j = 0; j < groupingData[i].multiplicityThird; ++j) {
                associatedThird.push_back(
                    thirdAssociatedGenerator.generate(i, uniform_int_distribution(rng))
                );
            }
        }
        // 20*N associated elements
    }

    void TearDown(const ::benchmark::State&) { groupingData.clear(); }
};

// -----------------------------------------------------------------------------
// SCENARIO 1: FULL PAIRS COMBINATIONS LIGHT (N^2 Log(N))
// -----------------------------------------------------------------------------
BENCHMARK_DEFINE_F(CombinationsFixtureHeavy, LibCombinationsFullPairs)
(benchmark::State& state)
{
    for (auto _ : state) {
        BucketPolicy bp = BucketPolicy(false, GetBucketCallable{}, buckets);

        auto grouping = SourceWithCallable(groupingData, GetIdHeavyGroupingCallable{});
        auto associatedFirstWrapper =
            SourceWithCallable(associatedFirst, GetIdHeavyAssociatedFirstStructCallable{});
        auto associatedSecondWrapper =
            SourceWithCallable(associatedSecond, GetIdHeavyAssociatedSecondStructCallable{});

        auto combinations = makeGroupedCombinations<FullCombinationsPolicy>(
            bp, minCatSize, grouping, associatedFirstWrapper, associatedSecondWrapper
        );
        size_t count = 0;
        for (const auto& [el0, as0, el1, as1] : combinations) {
            benchmark::DoNotOptimize(sqrt(el0.globalIndex + el1.globalIndex));
            benchmark::DoNotOptimize(count += as0.size());
            benchmark::DoNotOptimize(count += as1.size());
        }
        benchmark::DoNotOptimize(count);
    }
    state.SetComplexityN(state.range(0));
}

// -----------------------------------------------------------------------------
// SCENARIO 2: FULL PAIRS COMBINATIONS LIGHT (N^3 Log(N))
// -----------------------------------------------------------------------------
BENCHMARK_DEFINE_F(CombinationsFixtureHeavy, LibCombinationsFullTriples)
(benchmark::State& state)
{
    for (auto _ : state) {
        BucketPolicy bp = BucketPolicy(false, GetBucketCallable{}, buckets);

        auto grouping = SourceWithCallable(groupingData, GetIdHeavyGroupingCallable{});
        auto associatedFirstWrapper =
            SourceWithCallable(associatedFirst, GetIdHeavyAssociatedFirstStructCallable{});
        auto associatedSecondWrapper =
            SourceWithCallable(associatedSecond, GetIdHeavyAssociatedSecondStructCallable{});
        auto associatedThirdWrapper =
            SourceWithCallable(associatedThird, GetIdHeavyAssociatedThirdStructCallable{});

        auto combinations = makeGroupedCombinations<FullCombinationsPolicy>(
            bp, minCatSize, grouping, associatedFirstWrapper, associatedSecondWrapper,
            associatedThirdWrapper
        );
        int64_t count = 0;
        for (const auto& [el0, as0, el1, as1, el2, as2] : combinations) {
            benchmark::DoNotOptimize(sqrt(el0.globalIndex + el1.globalIndex + el2.globalIndex));
            benchmark::DoNotOptimize(count += as0.size());
            benchmark::DoNotOptimize(count += as1.size());
            benchmark::DoNotOptimize(count += as2.size());
        }
        benchmark::DoNotOptimize(count);
    }
    state.SetComplexityN(state.range(0));
}

// -----------------------------------------------------------------------------
// SCENARIO 3: STRICTLY UPPER PAIRS COMBINATIONS LIGHT (N^2 Log(N))
// -----------------------------------------------------------------------------
BENCHMARK_DEFINE_F(CombinationsFixtureHeavy, LibCombinationsStrictlyUpperPairs)
(benchmark::State& state)
{
    for (auto _ : state) {
        BucketPolicy bp = BucketPolicy(false, GetBucketCallable{}, buckets);

        auto grouping = SourceWithCallable(groupingData, GetIdHeavyGroupingCallable{});
        auto associatedFirstWrapper =
            SourceWithCallable(associatedFirst, GetIdHeavyAssociatedFirstStructCallable{});
        auto associatedSecondWrapper =
            SourceWithCallable(associatedSecond, GetIdHeavyAssociatedSecondStructCallable{});

        auto combinations = makeGroupedCombinations<StrictlyUpperCombinationsPolicy>(
            bp, minCatSize, grouping, associatedFirstWrapper, associatedSecondWrapper
        );
        size_t count = 0;
        for (const auto& [el0, as0, el1, as1] : combinations) {
            benchmark::DoNotOptimize(sqrt(el0.globalIndex + el1.globalIndex));
            benchmark::DoNotOptimize(count += as0.size());
            benchmark::DoNotOptimize(count += as1.size());
        }
        benchmark::DoNotOptimize(count);
    }
    state.SetComplexityN(state.range(0));
}

// -----------------------------------------------------------------------------
// SCENARIO 3: STRICTLY UPPER PAIRS COMBINATIONS LIGHT (N^3 Log(N))
// -----------------------------------------------------------------------------
BENCHMARK_DEFINE_F(CombinationsFixtureHeavy, LibCombinationsStrictlyUpperTriples)
(benchmark::State& state)
{
    for (auto _ : state) {
        BucketPolicy bp = BucketPolicy(false, GetBucketCallable{}, buckets);

        auto grouping = SourceWithCallable(groupingData, GetIdHeavyGroupingCallable{});
        auto associatedFirstWrapper =
            SourceWithCallable(associatedFirst, GetIdHeavyAssociatedFirstStructCallable{});
        auto associatedSecondWrapper =
            SourceWithCallable(associatedSecond, GetIdHeavyAssociatedSecondStructCallable{});
        auto associatedThirdWrapper =
            SourceWithCallable(associatedThird, GetIdHeavyAssociatedThirdStructCallable{});

        auto combinations = makeGroupedCombinations<StrictlyUpperCombinationsPolicy>(
            bp, minCatSize, grouping, associatedFirstWrapper, associatedSecondWrapper,
            associatedThirdWrapper
        );
        int64_t count = 0;
        for (const auto& [el0, as0, el1, as1, el2, as2] : combinations) {
            benchmark::DoNotOptimize(sqrt(el0.globalIndex + el1.globalIndex + el2.globalIndex));
            benchmark::DoNotOptimize(count += as0.size());
            benchmark::DoNotOptimize(count += as1.size());
            benchmark::DoNotOptimize(count += as2.size());
        }
        benchmark::DoNotOptimize(count);
    }
    state.SetComplexityN(state.range(0));
}

BENCHMARK_REGISTER_F(CombinationsFixtureHeavy, LibCombinationsFullPairs)
    ->RangeMultiplier(2)
    ->Range(1 << 4, 1 << 13)
    ->Complexity();
BENCHMARK_REGISTER_F(CombinationsFixtureHeavy, LibCombinationsFullTriples)
    ->RangeMultiplier(2)
    ->Range(1 << 4, 1 << 9)
    ->Complexity();
BENCHMARK_REGISTER_F(CombinationsFixtureHeavy, LibCombinationsStrictlyUpperPairs)
    ->RangeMultiplier(2)
    ->Range(1 << 4, 1 << 13)
    ->Complexity();
BENCHMARK_REGISTER_F(CombinationsFixtureHeavy, LibCombinationsStrictlyUpperTriples)
    ->RangeMultiplier(2)
    ->Range(1 << 4, 1 << 9)
    ->Complexity();
