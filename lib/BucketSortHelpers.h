//
// Created by mshamrai on 12/28/25.
//

#ifndef COMET_BUCKETSORTHELPERS_H
#define COMET_BUCKETSORTHELPERS_H

#include <algorithm>
#include <numeric>
#include <vector>

template <typename BucketType, typename T, typename Callable>
std::vector<BucketType> getAvailableBuckets(const T& container, const Callable& callable)
{
    std::vector<BucketType> availableBuckets;
    availableBuckets.reserve(container.size());
    for (const auto& element : container) {
        availableBuckets.push_back(callable(element));
    }

    std::sort(availableBuckets.begin(), availableBuckets.end());
    availableBuckets.erase(
        std::unique(availableBuckets.begin(), availableBuckets.end()), availableBuckets.end()
    );
    return availableBuckets;
}

template <typename BucketType>
int getIndexFromBucket(const std::vector<BucketType>& availableBuckets, const BucketType& bucket)
{
    auto lowerBound = std::lower_bound(availableBuckets.begin(), availableBuckets.end(), bucket);

    return lowerBound == availableBuckets.end() || bucket < *lowerBound
               ? -1
               : std::distance(availableBuckets.begin(), lowerBound);
}

template <typename BucketType, typename T, typename Callable>
std::vector<size_t> getAmountsInsideAvailableBuckets(
    const T& container, const Callable& callable, const std::vector<BucketType>& availableBuckets
)
{
    std::vector<size_t> amounts(availableBuckets.size(), 0);
    for (const auto& element : container) {
        auto id = callable(element);

        int index = getIndexFromBucket(availableBuckets, id);
        if (index != -1)
            ++amounts[index];
    }
    return amounts;
}

inline std::vector<size_t> getOffsetsFromAmounts(const std::vector<size_t>& amounts)
{
    std::vector<size_t> offsets(amounts.size() + 1, 0);
    std::inclusive_scan(amounts.begin(), amounts.end(), offsets.begin() + 1);
    return offsets;
}

template <typename T>
concept HasSize = requires(const T& element) {
    {
        element.size()
    } -> std::integral;
};

template <typename BucketType, HasSize T, typename Callable>
auto getIteratorsSorted(
    const T& container, const Callable& callable, const std::vector<size_t>& offsets,
    const std::vector<BucketType>& availableBuckets, std::vector<size_t>& amounts
)
{
    // The last value in offsets tells how many elements will be placed in the sorted vector
    auto sizeOfSorted = offsets.back();
    std::vector<typename T::const_iterator> sorted(sizeOfSorted);
    for (auto iterator = container.begin(); sizeOfSorted > 0 && iterator != container.end();
         ++iterator) {
        auto id = getIndexFromBucket(availableBuckets, callable(*iterator));
        if (id != -1) {
            sorted[offsets[id + 1] - amounts[id]--] = iterator;
            --sizeOfSorted;
        }
    }

    return sorted;
}

#endif  // COMET_BUCKETSORTHELPERS_H
