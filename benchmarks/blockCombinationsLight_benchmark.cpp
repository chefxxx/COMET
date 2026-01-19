#include <vector>
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
    std::vector<int> data;

    std::array<int, 10> firstDimension = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::array<int, 2> secondDimension = {0, 1};
    int minCatSize                     = 1;

    struct getFirstCallable {
        int operator()(const int& element) const { return element % 10; }
    };

    struct getSecondCallable {
        int operator()(const int& element) const { return element & 1; }
    };

    void SetUp(const ::benchmark::State& state)
    {
        auto N = state.range(0);
        data.reserve(N);
        for (int i = 0; i < N; ++i) data.push_back(i);
    }

    void TearDown(const ::benchmark::State&) { data.clear(); }
};

// -----------------------------------------------------------------------------
// SCENARIO 1: FULL PAIRS COMBINATIONS LIGHT (N^2)
// -----------------------------------------------------------------------------
BENCHMARK_DEFINE_F(CombinationsFixtureLightweight, LibCombinationsFullPairs)
(benchmark::State& state)
{
    for (auto _ : state) {
        BucketPolicy bp = BucketPolicy(
            false, getFirstCallable{}, getSecondCallable{}, firstDimension, secondDimension
        );
        auto combinations =
            makeBlockCombinations<FullCombinationsPolicy>(bp, minCatSize, this->data, this->data);
        int64_t count = 0;
        for (const auto& [el0, el1] : combinations) {
            benchmark::DoNotOptimize(sqrt(el0 + el1));
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
        BucketPolicy bp = BucketPolicy(
            false, getFirstCallable{}, getSecondCallable{}, firstDimension, secondDimension
        );
        auto combinations = makeBlockCombinations<FullCombinationsPolicy>(
            bp, minCatSize, this->data, this->data, this->data
        );
        int64_t count = 0;
        for (const auto& [el0, el1, el2] : combinations) {
            benchmark::DoNotOptimize(sqrt(el0 + el1 + el2));
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
        BucketPolicy bp = BucketPolicy(
            false, getFirstCallable{}, getSecondCallable{}, firstDimension, secondDimension
        );
        auto combinations = makeBlockCombinations<StrictlyUpperCombinationsPolicy>(
            bp, minCatSize, this->data, this->data
        );
        int64_t count = 0;
        for (const auto& [el0, el1] : combinations) {
            benchmark::DoNotOptimize(sqrt(el0 + el1));
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
        BucketPolicy bp = BucketPolicy(
            false, getFirstCallable{}, getSecondCallable{}, firstDimension, secondDimension
        );
        auto combinations = makeBlockCombinations<StrictlyUpperCombinationsPolicy>(
            bp, minCatSize, this->data, this->data, this->data
        );
        int64_t count = 0;
        for (const auto& [el0, el1, el2] : combinations) {
            benchmark::DoNotOptimize(sqrt(el0 + el1 + el2));
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
    ->Range(1 << 4, 1 << 10)
    ->Complexity();
BENCHMARK_REGISTER_F(CombinationsFixtureLightweight, LibCombinationsStrictlyUpperPairs)
    ->RangeMultiplier(2)
    ->Range(1 << 4, 1 << 13)
    ->Complexity();
BENCHMARK_REGISTER_F(CombinationsFixtureLightweight, LibCombinationsStrictlyUpperTriples)
    ->RangeMultiplier(2)
    ->Range(1 << 4, 1 << 10)
    ->Complexity();
