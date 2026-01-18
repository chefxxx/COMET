//
// Created by mshamrai on 1/18/26.
//
#include <benchmark/benchmark.h>
#include <vector>

#include "Combinations.h"
#include "Producers.h"

#include "HeavyStructures.h"

class CombinationsFixtureLightweight : public benchmark::Fixture
{
    public:
    std::vector<double> data;
    int windowSize = 10;

    void SetUp(const ::benchmark::State &state)
    {
        auto N = state.range(0);
        data.reserve(N);
        for (int i = 0; i < N; ++i) data.push_back(i);
    }

    void TearDown(const ::benchmark::State &) { data.clear(); }
};

// -----------------------------------------------------------------------------
// SCENARIO 1: FULL PAIRS COMBINATIONS LIGHT (N * N)
// -----------------------------------------------------------------------------
BENCHMARK_DEFINE_F(CombinationsFixtureLightweight, LoopCombinationsFullPairs)
(benchmark::State &state)
{
    for (auto _ : state) {
        int64_t count = 0;
        auto size     = this->data.size();
        for (int i = 0; i < size; ++i) {
            for (int j = i; j < size && j < i + windowSize; ++j) {
                auto [el0, el1] = std::forward_as_tuple(this->data[i], this->data[j]);
                benchmark::DoNotOptimize(sqrt(el0 + el1));
                count++;
            }
        }
        benchmark::DoNotOptimize(count);
    }
    state.SetComplexityN(state.range(0));
}

BENCHMARK_DEFINE_F(CombinationsFixtureLightweight, LibCombinationsFullPairs)
(benchmark::State &state)
{
    for (auto _ : state) {
        auto combinations = makeSameKindCombinations<SameTypeFullCombinationsPolicy>(
            windowSize, this->data, this->data
        );
        int64_t count = 0;
        for (const auto &[el0, el1] : combinations) {
            benchmark::DoNotOptimize(sqrt(el0 + el1));
        }
        benchmark::DoNotOptimize(count);
    }
    state.SetComplexityN(state.range(0));
}

// -----------------------------------------------------------------------------
// SCENARIO 2: FULL PAIRS COMBINATIONS LIGHT (N * N * N)
// -----------------------------------------------------------------------------
BENCHMARK_DEFINE_F(CombinationsFixtureLightweight, LoopCombinationsFullTriples)
(benchmark::State &state)
{
    for (auto _ : state) {
        int64_t count = 0;
        auto size     = this->data.size();
        for (int i = 0; i < size; ++i) {
            for (int j = i; j < size && j < i + windowSize; ++j) {
                for (int k = i; k < size && k < i + windowSize; ++k) {
                    auto [el0, el1, el2] =
                        std::forward_as_tuple(this->data[i], this->data[j], this->data[k]);
                    benchmark::DoNotOptimize(sqrt(el0 + el1 + el2));
                    count++;
                }
            }
        }
        benchmark::DoNotOptimize(count);
    }
    state.SetComplexityN(state.range(0));
}

BENCHMARK_DEFINE_F(CombinationsFixtureLightweight, LibCombinationsFullTriples)
(benchmark::State &state)
{
    for (auto _ : state) {
        auto combinations = makeSameKindCombinations<SameTypeFullCombinationsPolicy>(
            windowSize, this->data, this->data, this->data
        );
        int64_t count = 0;
        for (const auto &[el0, el1, el2] : combinations) {
            benchmark::DoNotOptimize(sqrt(el0 + el1 + el2));
        }
        benchmark::DoNotOptimize(count);
    }
    state.SetComplexityN(state.range(0));
}

BENCHMARK_REGISTER_F(CombinationsFixtureLightweight, LoopCombinationsFullPairs)
    ->RangeMultiplier(2)
    ->Range(1 << 4, 1 << 13)
    ->Complexity();
BENCHMARK_REGISTER_F(CombinationsFixtureLightweight, LibCombinationsFullPairs)
    ->RangeMultiplier(2)
    ->Range(1 << 4, 1 << 13)
    ->Complexity();
BENCHMARK_REGISTER_F(CombinationsFixtureLightweight, LoopCombinationsFullTriples)
    ->RangeMultiplier(2)
    ->Range(1 << 4, 1 << 10)
    ->Complexity();
BENCHMARK_REGISTER_F(CombinationsFixtureLightweight, LibCombinationsFullTriples)
    ->RangeMultiplier(2)
    ->Range(1 << 4, 1 << 10)
    ->Complexity();
