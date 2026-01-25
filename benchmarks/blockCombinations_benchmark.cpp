//
// Created by mshamrai on 1/18/26.
//

#include <benchmark/benchmark.h>
#include <vector>

#include "BucketPolicy.h"
#include "Combinations.h"
#include "Producers.h"

#include "HeavyStructures.h"

class CombinationsFixtureHeavyweight : public benchmark::Fixture
{
    public:
    std::vector<HeavyGroupingStruct> data;
    int seed                           = 42;
    int meanMultiplicity               = 20;
    std::array<int, 10> firstDimension = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::array<int, 2> secondDimension = {0, 1};
    int minCatSize                     = 1;

    struct getFirstCallable {
        int operator()(const HeavyGroupingStruct& element) const
        {
            return element.globalIndex % 10;
        }
    };

    struct getSecondCallable {
        int operator()(const HeavyGroupingStruct& element) const { return element.globalIndex & 1; }
    };

    HeavyGroupingStructGenerator<int> generator =
        HeavyGroupingStructGenerator(seed, meanMultiplicity);
    void SetUp(const ::benchmark::State& state)
    {
        int N = static_cast<int>(state.range(0));
        data.reserve(N);
        for (int i = 0; i < N; ++i) data.push_back(generator.generate(i));
    }

    void TearDown(const ::benchmark::State&) { data.clear(); }
};

// -----------------------------------------------------------------------------
// SCENARIO 1: FULL PAIRS COMBINATIONS (N^2)
// -----------------------------------------------------------------------------
BENCHMARK_DEFINE_F(CombinationsFixtureHeavyweight, LibCombinationsFullPairs)
(benchmark::State& state)
{
    for (auto _ : state) {
        BucketPolicy bp = BucketPolicy(
            false, getFirstCallable{}, getSecondCallable{}, firstDimension, secondDimension
        );
        auto combinations =
            makeBlockCombinations<FullCombinationsPolicy>(bp, minCatSize, this->data, this->data);
        std::size_t count = 0;
        for (const auto& [el0, el1] : combinations) {
            benchmark::DoNotOptimize(sqrt(el0.globalIndex + el1.globalIndex));
        }
        benchmark::DoNotOptimize(count);
    }
    state.SetComplexityN(state.range(0));
}

// -----------------------------------------------------------------------------
// SCENARIO 2: FULL TRIPLES COMBINATIONS (N^3)
// -----------------------------------------------------------------------------
BENCHMARK_DEFINE_F(CombinationsFixtureHeavyweight, LibCombinationsFullTriples)
(benchmark::State& state)
{
    for (auto _ : state) {
        BucketPolicy bp = BucketPolicy(
            false, getFirstCallable{}, getSecondCallable{}, firstDimension, secondDimension
        );
        auto combinations = makeBlockCombinations<FullCombinationsPolicy>(
            bp, minCatSize, this->data, this->data, this->data
        );
        std::size_t count = 0;
        for (const auto& [el0, el1, el2] : combinations) {
            benchmark::DoNotOptimize(sqrt(el0.globalIndex + el1.globalIndex + el2.globalIndex));
        }
        benchmark::DoNotOptimize(count);
    }
    state.SetComplexityN(state.range(0));
}

// -----------------------------------------------------------------------------
// SCENARIO 3: STRICTLY UPPER PAIRS COMBINATIONS (N^2)
// -----------------------------------------------------------------------------
BENCHMARK_DEFINE_F(CombinationsFixtureHeavyweight, LibCombinationsStrictlyUpperPairs)
(benchmark::State& state)
{
    for (auto _ : state) {
        BucketPolicy bp = BucketPolicy(
            false, getFirstCallable{}, getSecondCallable{}, firstDimension, secondDimension
        );
        auto combinations = makeBlockCombinations<StrictlyUpperCombinationsPolicy>(
            bp, minCatSize, this->data, this->data
        );
        std::size_t count = 0;
        for (const auto& [el0, el1] : combinations) {
            benchmark::DoNotOptimize(sqrt(el0.globalIndex + el1.globalIndex));
        }
        benchmark::DoNotOptimize(count);
    }
    state.SetComplexityN(state.range(0));
}

// -----------------------------------------------------------------------------
// SCENARIO 3: STRICTLY UPPER TRIPLES COMBINATIONS (N^3)
// -----------------------------------------------------------------------------
BENCHMARK_DEFINE_F(CombinationsFixtureHeavyweight, LibCombinationsStrictlyUpperTriples)
(benchmark::State& state)
{
    for (auto _ : state) {
        BucketPolicy bp = BucketPolicy(
            false, getFirstCallable{}, getSecondCallable{}, firstDimension, secondDimension
        );
        auto combinations = makeBlockCombinations<StrictlyUpperCombinationsPolicy>(
            bp, minCatSize, this->data, this->data, this->data
        );
        std::size_t count = 0;
        for (const auto& [el0, el1, el2] : combinations) {
            benchmark::DoNotOptimize(sqrt(el0.globalIndex + el1.globalIndex + el2.globalIndex));
        }
        benchmark::DoNotOptimize(count);
    }
    state.SetComplexityN(state.range(0));
}

BENCHMARK_REGISTER_F(CombinationsFixtureHeavyweight, LibCombinationsFullPairs)
    ->RangeMultiplier(2)
    ->Range(1 << 4, 1 << 13)
    ->Complexity();
BENCHMARK_REGISTER_F(CombinationsFixtureHeavyweight, LibCombinationsFullTriples)
    ->RangeMultiplier(2)
    ->Range(1 << 4, 1 << 10)
    ->Complexity();
BENCHMARK_REGISTER_F(CombinationsFixtureHeavyweight, LibCombinationsStrictlyUpperPairs)
    ->RangeMultiplier(2)
    ->Range(1 << 4, 1 << 13)
    ->Complexity();
BENCHMARK_REGISTER_F(CombinationsFixtureHeavyweight, LibCombinationsStrictlyUpperTriples)
    ->RangeMultiplier(2)
    ->Range(1 << 4, 1 << 10)
    ->Complexity();
