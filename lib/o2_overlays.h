//
// Created by Mateusz Mikiciuk on 24/07/2025.
//

#ifndef O2OVERLAYS_H
#define O2OVERLAYS_H

#include <boost/interprocess/detail/named_proxy.hpp>
#include <iterator>
#include <memory>
#include <tuple>
#include "BucketPolicy.h"
#include "Framework/ASoA.h"

namespace o2::framework
{

template <typename Type>
struct myCallable {
    template <std::forward_iterator TIter>
    auto operator()(TIter const &it)
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
};

}  // namespace o2::framework

#endif  // O2OVERLAYS_H
