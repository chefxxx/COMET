#include <benchmark/benchmark.h>
#include <cmath>
#include <vector>

// Our Library Headers
#include "Combinations.h"
#include "HeavyStructures.h"
#include "Producers.h"

// Discreture Header
#include <../include/discreture.hpp>

class CombinationsComparisonFixture : public benchmark::Fixture
{
    public:
    std::vector<HeavyGroupingStruct> data;
    int seed             = 42;
    int meanMultiplicity = 20;

    HeavyGroupingStructGenerator<int> generator =
        HeavyGroupingStructGenerator(seed, meanMultiplicity);

    void SetUp(const ::benchmark::State& state)
    {
        int N = state.range(0);
        data.clear();
        data.reserve(N);
        for (int i = 0; i < N; ++i) data.push_back(generator.generate(i));
    }

    void TearDown(const ::benchmark::State&) { data.clear(); }
};

// -----------------------------------------------------------------------------
// PAIRS (N x N)
// -----------------------------------------------------------------------------

BENCHMARK_DEFINE_F(CombinationsComparisonFixture, Lib_FullPairs)(benchmark::State& state)
{
    for (auto _ : state) {
        auto combinations = makeCombinations<FullCombinationsPolicy>(this->data, this->data);
        for (const auto& [el0, el1] : combinations) {
            benchmark::DoNotOptimize(std::sqrt(el0.globalIndex + el1.globalIndex));
        }
    }
    state.SetComplexityN(state.range(0));
}

BENCHMARK_DEFINE_F(CombinationsComparisonFixture, Discreture_IntervalPairs)(benchmark::State& state)
{
    for (auto _ : state) {
        auto n     = this->data.size();
        auto range = discreture::integer_interval(n);

        for (auto i : range) {
            for (auto j : range) {
                const auto& el0 = this->data[i];
                const auto& el1 = this->data[j];
                benchmark::DoNotOptimize(std::sqrt(el0.globalIndex + el1.globalIndex));
            }
        }
    }
    state.SetComplexityN(state.range(0));
}

// -----------------------------------------------------------------------------
// TRIPLES (N x N x N)
// -----------------------------------------------------------------------------

BENCHMARK_DEFINE_F(CombinationsComparisonFixture, Lib_FullTriples)(benchmark::State& state)
{
    for (auto _ : state) {
        auto combinations =
            makeCombinations<FullCombinationsPolicy>(this->data, this->data, this->data);
        for (const auto& [el0, el1, el2] : combinations) {
            benchmark::DoNotOptimize(std::sqrt(el0.globalIndex + el1.globalIndex + el2.globalIndex)
            );
        }
    }
    state.SetComplexityN(state.range(0));
}

BENCHMARK_DEFINE_F(CombinationsComparisonFixture, Discreture_IntervalTriples)
(benchmark::State& state)
{
    for (auto _ : state) {
        auto n     = this->data.size();
        auto range = discreture::integer_interval(n);

        for (auto i : range) {
            for (auto j : range) {
                for (auto k : range) {
                    const auto& el0 = this->data[i];
                    const auto& el1 = this->data[j];
                    const auto& el2 = this->data[k];
                    benchmark::DoNotOptimize(
                        std::sqrt(el0.globalIndex + el1.globalIndex + el2.globalIndex)
                    );
                }
            }
        }
    }
    state.SetComplexityN(state.range(0));
}

// -----------------------------------------------------------------------------
// REGISTRATION
// -----------------------------------------------------------------------------

BENCHMARK_REGISTER_F(CombinationsComparisonFixture, Lib_FullPairs)
    ->RangeMultiplier(2)
    ->Range(1 << 4, 1 << 11)
    ->Complexity();
BENCHMARK_REGISTER_F(CombinationsComparisonFixture, Discreture_IntervalPairs)
    ->RangeMultiplier(2)
    ->Range(1 << 4, 1 << 11)
    ->Complexity();

BENCHMARK_REGISTER_F(CombinationsComparisonFixture, Lib_FullTriples)
    ->RangeMultiplier(2)
    ->Range(1 << 4, 1 << 8)
    ->Complexity();
BENCHMARK_REGISTER_F(CombinationsComparisonFixture, Discreture_IntervalTriples)
    ->RangeMultiplier(2)
    ->Range(1 << 4, 1 << 8)
    ->Complexity();

BENCHMARK_MAIN();
