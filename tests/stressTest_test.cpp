//
// Created by msh on 1/5/26.
//
#include <gtest/gtest.h>
#include <chrono>
#include <random>

#include "BucketPolicy.h"
#include "Combinations.h"
#include "GroupedCombinations.h"

struct GroupingElement {
    int Id;
    double value;
};
struct AssociatedElement1 {
    int groupId;
    double value;
};
struct AssociatedElement2 {
    int id;
    std::string value;
};

auto GroupingCall = [](const GroupingElement& e) {
    return e.Id;
};
auto Assoc1Call = [](const AssociatedElement1& e) {
    return e.groupId;
};
auto Assoc2Call = [](const AssociatedElement2& e) {
    return e.id;
};

class GroupedCombinationsStressTest : public ::testing::Test
{
    protected:
    std::vector<GroupingElement> groupingSource;
    std::vector<AssociatedElement1> associatedSource1;
    std::vector<AssociatedElement2> associatedSource2;

    const std::vector<double> buckets_edges = []() {
        std::vector<double> b;
        for (double i = 0.0; i <= 1.0; i += 0.01) b.push_back(i);
        return b;
    }();

    struct BucketCallable {
        auto operator()(const GroupingElement& e) const { return e.value; }
    };

    void SetUp() override
    {
        const int n_groups           = 100000;
        const int tracks_per_group   = 50;
        const int clusters_per_group = 20;

        std::mt19937 gen(42);
        std::uniform_real_distribution<double> val_dist(0.0, 1.0);
        std::uniform_int_distribution<int> id_dist(100, 1000000);

        groupingSource.reserve(n_groups);
        associatedSource1.reserve(n_groups * tracks_per_group);
        associatedSource2.reserve(n_groups * clusters_per_group);

        for (int i = 0; i < n_groups; ++i) {
            int currentId     = id_dist(gen);
            double currentVal = val_dist(gen);

            groupingSource.push_back({currentId, currentVal});

            for (int t = 0; t < tracks_per_group; ++t) {
                associatedSource1.push_back({currentId, val_dist(gen)});
            }

            for (int c = 0; c < clusters_per_group; ++c) {
                associatedSource2.push_back({currentId, "ClusterData_" + std::to_string(c)});
            }
        }

        for (int i = 0; i < 5000; ++i) {
            associatedSource1.push_back({-1, 0.0});
        }
    }
};

TEST_F(GroupedCombinationsStressTest, PerformanceAndScaleTest)
{
    auto groupingWrapper = SourceWithCallable{groupingSource, GroupingCall};
    auto assocWrapper1   = SourceWithCallable{associatedSource1, Assoc1Call};
    auto assocWrapper2   = SourceWithCallable{associatedSource2, Assoc2Call};

    const auto bucketPolicy = BucketPolicy(false, BucketCallable{}, buckets_edges);
    int minCatSize          = 1;

    // --- Шаг 1: Замер времени инициализации (Build CSR) ---
    auto start_build = std::chrono::high_resolution_clock::now();

    auto grouped = makeGroupedCombinations<StrictlyUpperCombinationsPolicy>(
        bucketPolicy, minCatSize, groupingWrapper, assocWrapper1, assocWrapper2
    );

    auto end_build = std::chrono::high_resolution_clock::now();
    auto build_ms =
        std::chrono::duration_cast<std::chrono::milliseconds>(end_build - start_build).count();

    std::cout << "[ PERF ] CSR Build time for 10k groups and 700k elements: " << build_ms << " ms"
              << std::endl;

    // --- Шаг 2: Замер времени итерации ---
    size_t combination_count      = 0;
    size_t total_tracks_processed = 0;

    auto start_iter = std::chrono::high_resolution_clock::now();

    for (auto [el0, span1, el1, span2] : grouped) {
        combination_count++;
        total_tracks_processed += span1.size();
        total_tracks_processed += span2.size();

        if (combination_count == 0)
            std::cout << el0.Id;
    }

    auto end_iter = std::chrono::high_resolution_clock::now();
    auto iter_ms =
        std::chrono::duration_cast<std::chrono::milliseconds>(end_iter - start_iter).count();

    std::cout << "[ PERF ] Iteration time: " << iter_ms << " ms" << std::endl;
    std::cout << "[ INFO ] Total combinations processed: " << combination_count << std::endl;
    std::cout << "[ INFO ] Total span elements touched: " << total_tracks_processed << std::endl;

    EXPECT_GT(combination_count, 500000);
    EXPECT_LT(build_ms, 500);
}
