//
// Created by Mateusz Mikiciuk on 24/07/2025.
//

#ifndef O2OVERLAYS_H
#define O2OVERLAYS_H

#include <iterator>
#include <memory>
#include <tuple>
#include "BucketPolicy.h"
#include "Framework/ASoA.h"

namespace o2::framework
{

#ifndef O2_OVERLAY

template <typename Type>
struct myCallable {
    template <std::forward_iterator TIter>
    auto operator()(TIter const& it)
    {
        return soa::row_helpers::getColumnValue<typename Type::type, TIter, Type>(it);
    }
};

template <typename... Types>
struct ColumnBinningPolicy {
    using BucketType = BucketPolicy<myCallable<Types>...>;

    BucketType myBucket;

    ColumnBinningPolicy(
        std::array<std::vector<double>, sizeof...(Types)> bins, bool ignoreOverflows
    )
        : myBucket(std::make_tuple(myCallable<Types>()...), bins, ignoreOverflows)
    {
    }

    template <typename T>
    auto getBinningValues(T& rowIterator, uint64_t globalIndex = -1) const
    {
        if (globalIndex != -1) {
            rowIterator.setCursor(globalIndex);
        }
        return myBucket.getValues(rowIterator);
    }

    template <typename T>
    auto getBinningValues(
        typename T::iterator rowIterator, T& table, uint64_t globalIndex = -1
    ) const
    {
        return getBinningValues(rowIterator, globalIndex);
    }

    int getBin(std::tuple<typename Types::type...> const& data) const
    {
        auto indices = myBucket.getUpperIndicesForTuple(data);
        auto bucket  = myBucket.calculateBucketAtIndices(indices);
        return bucket;
    }
};

#endif

template <typename, typename...>
struct FlexibleBinningPolicy;

template <typename... Types, typename... TCallables>
struct FlexibleBinningPolicy<std::tuple<TCallables...>, Types...> {
};

}  // namespace o2::framework

#endif  // O2OVERLAYS_H
