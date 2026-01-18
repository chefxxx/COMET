//
// Created by mshamrai on 1/12/26.
//

#include <benchmark/benchmark.h>
#include <vector>

#include "Combinations.h"
#include "Producers.h"

#include "HeavyStructures.h"

class CombinationsFixtureHeavyweight : public benchmark::Fixture
{
    public:
    std::vector<HeavyGroupingStruct> data;
    int seed             = 42;
    int meanMultiplicity = 20;

    HeavyGroupingStructGenerator<int> generator =
        HeavyGroupingStructGenerator(seed, meanMultiplicity);
    void SetUp(const ::benchmark::State &state)
    {
        int N = state.range(0);
        data.reserve(N);
        for (int i = 0; i < N; ++i) data.push_back(generator.generate(i));
    }

    void TearDown(const ::benchmark::State &) { data.clear(); }
};

// -----------------------------------------------------------------------------
// SCENARIO 1: FULL PAIRS COMBINATIONS LIGHT (N * N)
// -----------------------------------------------------------------------------
BENCHMARK_DEFINE_F(CombinationsFixtureHeavyweight, LoopCombinationsFullPairs)
(benchmark::State &state)
{
    for (auto _ : state) {
        int64_t count = 0;
        auto size     = this->data.size();
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                auto [el0, el1] = std::forward_as_tuple(this->data[i], this->data[j]);
                benchmark::DoNotOptimize(sqrt(el0.globalIndex + el1.globalIndex));
                count++;
            }
        }
        benchmark::DoNotOptimize(count);
    }
    state.SetComplexityN(state.range(0));
}

BENCHMARK_DEFINE_F(CombinationsFixtureHeavyweight, LibCombinationsFullPairs)
(benchmark::State &state)
{
    for (auto _ : state) {
        auto combinations = makeCombinations<FullCombinationsPolicy>(this->data, this->data);
        int64_t count     = 0;
        for (const auto &[el0, el1] : combinations) {
            benchmark::DoNotOptimize(sqrt(el0.globalIndex + el1.globalIndex));
        }
        benchmark::DoNotOptimize(count);
    }
    state.SetComplexityN(state.range(0));
}

// -----------------------------------------------------------------------------
// SCENARIO 2: FULL PAIRS COMBINATIONS LIGHT (N * N * N)
// -----------------------------------------------------------------------------
BENCHMARK_DEFINE_F(CombinationsFixtureHeavyweight, LoopCombinationsFullTriples)
(benchmark::State &state)
{
    for (auto _ : state) {
        int64_t count = 0;
        auto size     = this->data.size();
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                for (int k = 0; k < size; ++k) {
                    auto [el0, el1, el2] =
                        std::forward_as_tuple(this->data[i], this->data[j], this->data[k]);
                    benchmark::DoNotOptimize(
                        sqrt(el0.globalIndex + el1.globalIndex + el2.globalIndex)
                    );
                    count++;
                }
            }
        }
        benchmark::DoNotOptimize(count);
    }
    state.SetComplexityN(state.range(0));
}

BENCHMARK_DEFINE_F(CombinationsFixtureHeavyweight, LibCombinationsFullTriples)
(benchmark::State &state)
{
    for (auto _ : state) {
        auto combinations =
            makeCombinations<FullCombinationsPolicy>(this->data, this->data, this->data);
        int64_t count = 0;
        for (const auto &[el0, el1, el2] : combinations) {
            benchmark::DoNotOptimize(sqrt(el0.globalIndex + el1.globalIndex + el2.globalIndex));
        }
        benchmark::DoNotOptimize(count);
    }
    state.SetComplexityN(state.range(0));
}

BENCHMARK_REGISTER_F(CombinationsFixtureHeavyweight, LoopCombinationsFullPairs)
    ->RangeMultiplier(2)
    ->Range(1 << 4, 1 << 13)
    ->Complexity();
BENCHMARK_REGISTER_F(CombinationsFixtureHeavyweight, LibCombinationsFullPairs)
    ->RangeMultiplier(2)
    ->Range(1 << 4, 1 << 13)
    ->Complexity();
BENCHMARK_REGISTER_F(CombinationsFixtureHeavyweight, LoopCombinationsFullTriples)
    ->RangeMultiplier(2)
    ->Range(1 << 4, 1 << 10)
    ->Complexity();
BENCHMARK_REGISTER_F(CombinationsFixtureHeavyweight, LibCombinationsFullTriples)
    ->RangeMultiplier(2)
    ->Range(1 << 4, 1 << 10)
    ->Complexity();
