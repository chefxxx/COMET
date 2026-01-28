#include <benchmark/benchmark.h>
#include <cmath>
#include <vector>

// Our Library Headers
#include "HeavyStructures.h"
#include "include/Combinations.h"
#include "include/Producers.h"

// Discreture Header
#include <discreture.hpp>

class StrictlyUpperComparisonFixture : public benchmark::Fixture
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
// TRIPLES: Strictly Upper (nCr where r=3)
// -----------------------------------------------------------------------------

BENCHMARK_DEFINE_F(StrictlyUpperComparisonFixture, Comet_StrictlyUpperTriples)
(benchmark::State& state)
{
    for (auto _ : state) {
        // Generates (i, j, k) where i < j < k
        auto combinations =
            makeCombinations<StrictlyUpperCombinationsPolicy>(this->data, this->data, this->data);
        for (const auto& [el0, el1, el2] : combinations) {
            benchmark::DoNotOptimize(std::sqrt(el0.globalIndex + el1.globalIndex + el2.globalIndex)
            );
        }
    }
    state.SetComplexityN(state.range(0));
}

BENCHMARK_DEFINE_F(StrictlyUpperComparisonFixture, Discreture_CombinationsTriples)
(benchmark::State& state)
{
    for (auto _ : state) {
        int n = static_cast<int>(this->data.size());
        // combinations(n, 3) generates indices for i < j < k
        auto combinations = discreture::combinations(n, 3);
        for (const auto& indices : combinations) {
            const auto& el0 = this->data[indices[0]];
            const auto& el1 = this->data[indices[1]];
            const auto& el2 = this->data[indices[2]];
            benchmark::DoNotOptimize(std::sqrt(el0.globalIndex + el1.globalIndex + el2.globalIndex)
            );
        }
    }
    state.SetComplexityN(state.range(0));
}

// -----------------------------------------------------------------------------
// PAIRS (Included for a complete comparison)
// -----------------------------------------------------------------------------

BENCHMARK_DEFINE_F(StrictlyUpperComparisonFixture, Comet_StrictlyUpperPairs)
(benchmark::State& state)
{
    for (auto _ : state) {
        auto combinations =
            makeCombinations<StrictlyUpperCombinationsPolicy>(this->data, this->data);
        for (const auto& [el0, el1] : combinations) {
            benchmark::DoNotOptimize(std::sqrt(el0.globalIndex + el1.globalIndex));
        }
    }
    state.SetComplexityN(state.range(0));
}

BENCHMARK_DEFINE_F(StrictlyUpperComparisonFixture, Discreture_CombinationsPairs)
(benchmark::State& state)
{
    for (auto _ : state) {
        int n             = static_cast<int>(this->data.size());
        auto combinations = discreture::combinations(n, 2);
        for (const auto& indices : combinations) {
            const auto& el0 = this->data[indices[0]];
            const auto& el1 = this->data[indices[1]];
            benchmark::DoNotOptimize(std::sqrt(el0.globalIndex + el1.globalIndex));
        }
    }
    state.SetComplexityN(state.range(0));
}

// -----------------------------------------------------------------------------
// REGISTRATION
// -----------------------------------------------------------------------------

BENCHMARK_REGISTER_F(StrictlyUpperComparisonFixture, Comet_StrictlyUpperPairs)
    ->RangeMultiplier(2)
    ->Range(1 << 4, 1 << 10)
    ->Complexity();
BENCHMARK_REGISTER_F(StrictlyUpperComparisonFixture, Discreture_CombinationsPairs)
    ->RangeMultiplier(2)
    ->Range(1 << 4, 1 << 10)
    ->Complexity();

BENCHMARK_REGISTER_F(StrictlyUpperComparisonFixture, Comet_StrictlyUpperTriples)
    ->RangeMultiplier(2)
    ->Range(1 << 4, 1 << 8)
    ->Complexity();
BENCHMARK_REGISTER_F(StrictlyUpperComparisonFixture, Discreture_CombinationsTriples)
    ->RangeMultiplier(2)
    ->Range(1 << 4, 1 << 8)
    ->Complexity();

BENCHMARK_MAIN();
