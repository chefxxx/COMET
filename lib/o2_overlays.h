//
// Created by Mateusz Mikiciuk on 24/07/2025.
//

#ifndef O2OVERLAYS_H
#define O2OVERLAYS_H

#include "BucketPolicy.h"
#include <memory>

struct BinningToBucket {

};

template <typename... TTypes>
struct ColumnBinningPolicy {
    BinningToBucket binningToBucket;
    ColumnBinningPolicy(std::array<std::vector<double>, sizeof...(TTypes)> bins, bool ignoreOverflows) {
    }
};

#endif //O2OVERLAYS_H
