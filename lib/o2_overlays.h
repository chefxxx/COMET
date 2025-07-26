//
// Created by Mateusz Mikiciuk on 24/07/2025.
//

#ifndef O2OVERLAYS_H
#define O2OVERLAYS_H

#include "Framework/ASoA.h"
#include "BucketPolicy.h"
#include <boost/interprocess/detail/named_proxy.hpp>
#include <memory>
#include <tuple>

namespace o2::framework
{


template <typename... TTypes>
struct ColumnBinningPolicy {

    
    ColumnBinningPolicy(std::array<std::vector<double>, sizeof...(TTypes)> bins, bool ignoreOverflows) {
        
    }

    template <typename TType>
    struct Foo {
        template <typename TIter>
        double operator()(TIter const &it){
            return soa::row_helpers::getColumnValue<typename TType::type, TIter, TType>(it);
        }
    };

    std::tuple<Foo<TTypes>...> mTuple;
};


}

#endif //O2OVERLAYS_H
