//
// Created by mshamrai on 1/18/26.
//
#include <benchmark/benchmark.h>
#include <vector>

#include "include/Combinations.h"
#include "include/Producers.h"

#include "HeavyStructures.h"

class CombinationsFixtureHeavyweight : public benchmark::Fixture
{
    public:
    std::vector<HeavyGroupingStruct> data;
    int seed             = 42;
    int meanMultiplicity = 20;
    int windowSize       = 10;

    HeavyGroupingStructGenerator<int> generator =
        HeavyGroupingStructGenerator(seed, meanMultiplicity);
    void SetUp(const ::benchmark::State &state)
    {
        int N = static_cast<int>(state.range(0));
        data.reserve(N);
        for (int i = 0; i < N; ++i) data.push_back(generator.generate(i));
    }

    void TearDown(const ::benchmark::State &) { data.clear(); }
};

// -----------------------------------------------------------------------------
// SCENARIO 1: STRICTLY UPPER PAIRS COMBINATIONS (N^2)
// -----------------------------------------------------------------------------
BENCHMARK_DEFINE_F(CombinationsFixtureHeavyweight, LoopCombinationsStrictlyUpperPairs)
(benchmark::State &state)
{
    for (auto _ : state) {
        std::size_t count = 0;
        auto size         = this->data.size();
        for (std::size_t i = 0; i < size; ++i) {
            for (std::size_t j = i + 1; j < size && j < i + windowSize; ++j) {
                auto [el0, el1] = std::forward_as_tuple(this->data[i], this->data[j]);
                benchmark::DoNotOptimize(sqrt(el0.globalIndex + el1.globalIndex));
                count++;
            }
        }
        benchmark::DoNotOptimize(count);
    }
    state.SetComplexityN(state.range(0));
}

BENCHMARK_DEFINE_F(CombinationsFixtureHeavyweight, LibCombinationsStrictlyUpperPairs)
(benchmark::State &state)
{
    for (auto _ : state) {
        auto combinations = makeSameKindCombinations<SameTypeStrictlyUpperCombinationsPolicy>(
            windowSize, this->data, this->data
        );
        std::size_t count = 0;
        for (const auto &[el0, el1] : combinations) {
            benchmark::DoNotOptimize(sqrt(el0.globalIndex + el1.globalIndex));
        }
        benchmark::DoNotOptimize(count);
    }
    state.SetComplexityN(state.range(0));
}

// -----------------------------------------------------------------------------
// SCENARIO 2: STRICTLY TRIPLES COMBINATIONS (N^3)
// -----------------------------------------------------------------------------
BENCHMARK_DEFINE_F(CombinationsFixtureHeavyweight, LoopCombinationsStrictlyUpperTriples)
(benchmark::State &state)
{
    for (auto _ : state) {
        std::size_t count = 0;
        auto size         = this->data.size();
        for (std::size_t i = 0; i < size; ++i) {
            for (std::size_t j = i + 1; j < size && j < i + windowSize; ++j) {
                for (std::size_t k = j + 1; k < size && k < i + windowSize; ++k) {
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

BENCHMARK_DEFINE_F(CombinationsFixtureHeavyweight, LibCombinationsStrictlyUpperTriples)
(benchmark::State &state)
{
    for (auto _ : state) {
        auto combinations = makeSameKindCombinations<SameTypeStrictlyUpperCombinationsPolicy>(
            windowSize, this->data, this->data, this->data
        );
        std::size_t count = 0;
        for (const auto &[el0, el1, el2] : combinations) {
            benchmark::DoNotOptimize(sqrt(el0.globalIndex + el1.globalIndex + el2.globalIndex));
        }
        benchmark::DoNotOptimize(count);
    }
    state.SetComplexityN(state.range(0));
}

BENCHMARK_REGISTER_F(CombinationsFixtureHeavyweight, LoopCombinationsStrictlyUpperPairs)
    ->RangeMultiplier(2)
    ->Range(1 << 4, 1 << 13)
    ->Complexity();
BENCHMARK_REGISTER_F(CombinationsFixtureHeavyweight, LibCombinationsStrictlyUpperPairs)
    ->RangeMultiplier(2)
    ->Range(1 << 4, 1 << 13)
    ->Complexity();
BENCHMARK_REGISTER_F(CombinationsFixtureHeavyweight, LoopCombinationsStrictlyUpperTriples)
    ->RangeMultiplier(2)
    ->Range(1 << 4, 1 << 10)
    ->Complexity();
BENCHMARK_REGISTER_F(CombinationsFixtureHeavyweight, LibCombinationsStrictlyUpperTriples)
    ->RangeMultiplier(2)
    ->Range(1 << 4, 1 << 10)
    ->Complexity();
