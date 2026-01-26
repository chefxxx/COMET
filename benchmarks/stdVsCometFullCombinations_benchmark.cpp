//
// Created by Mateusz Mikiciuk on 20/01/2026.
//

#include <benchmark/benchmark.h>
#include <cmath>
#include <ranges>  // Required for std::views::cartesian_product
#include <vector>

// Our Library Headers
#include "Combinations.h"
#include "HeavyStructures.h"
#include "Producers.h"

class CartesianComparisonFixture : public benchmark::Fixture
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
// COMET: Cartesian Product (Full Pairs)
// -----------------------------------------------------------------------------
BENCHMARK_DEFINE_F(CartesianComparisonFixture, Comet_FullPairs)(benchmark::State& state)
{
    for (auto _ : state) {
        auto combinations = makeCombinations<FullCombinationsPolicy>(this->data, this->data);
        for (const auto& [el0, el1] : combinations) {
            benchmark::DoNotOptimize(std::sqrt(el0.globalIndex + el1.globalIndex));
        }
    }
    state.SetComplexityN(state.range(0));
}

// -----------------------------------------------------------------------------
// STD::RANGES: Cartesian Product (Full Pairs)
// -----------------------------------------------------------------------------
BENCHMARK_DEFINE_F(CartesianComparisonFixture, StdRanges_FullPairs)(benchmark::State& state)
{
    for (auto _ : state) {
        // C++23 std::views::cartesian_product
        auto cp = std::views::cartesian_product(this->data, this->data);
        for (auto&& [el0, el1] : cp) {
            benchmark::DoNotOptimize(std::sqrt(el0.globalIndex + el1.globalIndex));
        }
    }
    state.SetComplexityN(state.range(0));
}

// -----------------------------------------------------------------------------
// TRIPLES: Comet vs Std::Ranges
// -----------------------------------------------------------------------------

BENCHMARK_DEFINE_F(CartesianComparisonFixture, Comet_FullTriples)(benchmark::State& state)
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

BENCHMARK_DEFINE_F(CartesianComparisonFixture, StdRanges_FullTriples)(benchmark::State& state)
{
    for (auto _ : state) {
        auto cp = std::views::cartesian_product(this->data, this->data, this->data);
        for (auto&& [el0, el1, el2] : cp) {
            benchmark::DoNotOptimize(std::sqrt(el0.globalIndex + el1.globalIndex + el2.globalIndex)
            );
        }
    }
    state.SetComplexityN(state.range(0));
}

// -----------------------------------------------------------------------------
// REGISTRATION
// -----------------------------------------------------------------------------
BENCHMARK_REGISTER_F(CartesianComparisonFixture, Comet_FullPairs)
    ->RangeMultiplier(2)
    ->Range(1 << 4, 1 << 10)
    ->Complexity();
BENCHMARK_REGISTER_F(CartesianComparisonFixture, StdRanges_FullPairs)
    ->RangeMultiplier(2)
    ->Range(1 << 4, 1 << 10)
    ->Complexity();

BENCHMARK_REGISTER_F(CartesianComparisonFixture, Comet_FullTriples)
    ->RangeMultiplier(2)
    ->Range(1 << 4, 1 << 7)
    ->Complexity();
BENCHMARK_REGISTER_F(CartesianComparisonFixture, StdRanges_FullTriples)
    ->RangeMultiplier(2)
    ->Range(1 << 4, 1 << 7)
    ->Complexity();

BENCHMARK_MAIN();
