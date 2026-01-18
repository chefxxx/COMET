//
// Created by mshamrai on 1/18/26.
//

#include <benchmark/benchmark.h>
#include <vector>

#include "BucketPolicy.h"
#include "Combinations.h"
#include "Producers.h"

#include "HeavyStructures.h"

class CombinationsFixtureLightweight : public benchmark::Fixture
{
    public:
    struct AssociatedMock {
        int globalIndex;
        int value;
    };
    int seed             = 42;
    int meanMultiplicity = 20;

    struct GetGroupingCallable {
        int operator()(const int& element) const { return element; }
    };

    struct GetAssociatedCallable {
        int operator()(const AssociatedMock& element) const { return element.globalIndex; }
    };

    struct GetBucketCallable {
        int operator()(const int& element) const { return 1; }
    };

    std::vector<int> groupingData;
    std::vector<AssociatedMock> associatedData;
    std::mt19937 rng = std::mt19937(seed);
    std::uniform_int_distribution<int> uniform_int_distribution =
        std::uniform_int_distribution(-1000, 1000);
    std::poisson_distribution<int> poisson_distribution =
        std::poisson_distribution(meanMultiplicity);

    std::array<int, 2> buckets = {0, 1};
    int minCatSize             = 1;

    void SetUp(const ::benchmark::State& state)
    {
        // N elements
        auto N = state.range(0);
        groupingData.reserve(N);
        associatedData.reserve(meanMultiplicity * N);
        for (int i = 0; i < N; ++i) {
            groupingData.push_back(i);
            int multiplicity = poisson_distribution(rng);
            for (int j = 0; j < multiplicity; ++j) {
                associatedData.push_back(AssociatedMock(i, uniform_int_distribution(rng)));
            }
        }
        // 20*N associated elements
    }

    void TearDown(const ::benchmark::State&) { groupingData.clear(); }
};

// -----------------------------------------------------------------------------
// SCENARIO 1: FULL PAIRS COMBINATIONS LIGHT (N^2)
// -----------------------------------------------------------------------------
BENCHMARK_DEFINE_F(CombinationsFixtureLightweight, LibCombinationsFullPairs)
(benchmark::State& state)
{
    for (auto _ : state) {
        BucketPolicy bp = BucketPolicy(false, GetBucketCallable{}, buckets);

        auto grouping         = SourceWithCallable(groupingData, GetGroupingCallable{});
        auto associatedFirst  = SourceWithCallable(associatedData, GetAssociatedCallable{});
        auto associatedSecond = SourceWithCallable(associatedData, GetAssociatedCallable{});

        auto combinations = makeGroupedCombinations<FullCombinationsPolicy>(
            bp, minCatSize, grouping, associatedFirst, associatedSecond
        );
        size_t count = 0;
        for (const auto& [el0, as0, el1, as1] : combinations) {
            benchmark::DoNotOptimize(sqrt(el0 + el1));
            benchmark::DoNotOptimize(count += as0.size());
            benchmark::DoNotOptimize(count += as1.size());
        }
        benchmark::DoNotOptimize(count);
    }
    state.SetComplexityN(state.range(0));
}

// -----------------------------------------------------------------------------
// SCENARIO 2: FULL PAIRS COMBINATIONS LIGHT (N^3)
// -----------------------------------------------------------------------------
BENCHMARK_DEFINE_F(CombinationsFixtureLightweight, LibCombinationsFullTriples)
(benchmark::State& state)
{
    for (auto _ : state) {
        BucketPolicy bp = BucketPolicy(false, GetBucketCallable{}, buckets);

        auto grouping         = SourceWithCallable(groupingData, GetGroupingCallable{});
        auto associatedFirst  = SourceWithCallable(associatedData, GetAssociatedCallable{});
        auto associatedSecond = SourceWithCallable(associatedData, GetAssociatedCallable{});
        auto associatedThird  = SourceWithCallable(associatedData, GetAssociatedCallable{});

        auto combinations = makeGroupedCombinations<FullCombinationsPolicy>(
            bp, minCatSize, grouping, associatedFirst, associatedSecond, associatedThird
        );
        int64_t count = 0;
        for (const auto& [el0, as0, el1, as1, el2, as2] : combinations) {
            benchmark::DoNotOptimize(sqrt(el0 + el1 + el2));
            benchmark::DoNotOptimize(count += as0.size());
            benchmark::DoNotOptimize(count += as1.size());
            benchmark::DoNotOptimize(count += as2.size());
        }
        benchmark::DoNotOptimize(count);
    }
    state.SetComplexityN(state.range(0));
}

// -----------------------------------------------------------------------------
// SCENARIO 3: STRICTLY UPPER PAIRS COMBINATIONS LIGHT (N^2)
// -----------------------------------------------------------------------------
BENCHMARK_DEFINE_F(CombinationsFixtureLightweight, LibCombinationsStrictlyUpperPairs)
(benchmark::State& state)
{
    for (auto _ : state) {
        BucketPolicy bp = BucketPolicy(false, GetBucketCallable{}, buckets);

        auto grouping         = SourceWithCallable(groupingData, GetGroupingCallable{});
        auto associatedFirst  = SourceWithCallable(associatedData, GetAssociatedCallable{});
        auto associatedSecond = SourceWithCallable(associatedData, GetAssociatedCallable{});

        auto combinations = makeGroupedCombinations<StrictlyUpperCombinationsPolicy>(
            bp, minCatSize, grouping, associatedFirst, associatedSecond
        );
        int64_t count = 0;
        for (const auto& [el0, as0, el1, as1] : combinations) {
            benchmark::DoNotOptimize(sqrt(el0 + el1));
            benchmark::DoNotOptimize(count += as0.size());
            benchmark::DoNotOptimize(count += as1.size());
        }
        benchmark::DoNotOptimize(count);
    }
    state.SetComplexityN(state.range(0));
}

// -----------------------------------------------------------------------------
// SCENARIO 3: STRICTLY UPPER PAIRS COMBINATIONS LIGHT (N^3)
// -----------------------------------------------------------------------------
BENCHMARK_DEFINE_F(CombinationsFixtureLightweight, LibCombinationsStrictlyUpperTriples)
(benchmark::State& state)
{
    for (auto _ : state) {
        BucketPolicy bp = BucketPolicy(false, GetBucketCallable{}, buckets);

        auto grouping         = SourceWithCallable(groupingData, GetGroupingCallable{});
        auto associatedFirst  = SourceWithCallable(associatedData, GetAssociatedCallable{});
        auto associatedSecond = SourceWithCallable(associatedData, GetAssociatedCallable{});
        auto associatedThird  = SourceWithCallable(associatedData, GetAssociatedCallable{});

        auto combinations = makeGroupedCombinations<StrictlyUpperCombinationsPolicy>(
            bp, minCatSize, grouping, associatedFirst, associatedSecond, associatedThird
        );
        int64_t count = 0;
        for (const auto& [el0, as0, el1, as1, el2, as2] : combinations) {
            benchmark::DoNotOptimize(sqrt(el0 + el1 + el2));
            benchmark::DoNotOptimize(count += as0.size());
            benchmark::DoNotOptimize(count += as1.size());
            benchmark::DoNotOptimize(count += as2.size());
        }
        benchmark::DoNotOptimize(count);
    }
    state.SetComplexityN(state.range(0));
}

BENCHMARK_REGISTER_F(CombinationsFixtureLightweight, LibCombinationsFullPairs)
    ->RangeMultiplier(2)
    ->Range(1 << 4, 1 << 13)
    ->Complexity();
BENCHMARK_REGISTER_F(CombinationsFixtureLightweight, LibCombinationsFullTriples)
    ->RangeMultiplier(2)
    ->Range(1 << 4, 1 << 9)
    ->Complexity();
BENCHMARK_REGISTER_F(CombinationsFixtureLightweight, LibCombinationsStrictlyUpperPairs)
    ->RangeMultiplier(2)
    ->Range(1 << 4, 1 << 13)
    ->Complexity();
BENCHMARK_REGISTER_F(CombinationsFixtureLightweight, LibCombinationsStrictlyUpperTriples)
    ->RangeMultiplier(2)
    ->Range(1 << 4, 1 << 9)
    ->Complexity();
