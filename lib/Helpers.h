//
// Created by Mateusz Mikiciuk on 18/07/2025.
//

#ifndef HELPERS_H
#define HELPERS_H

#include <iterator>
#include "BucketPolicy.h"

struct BucketIndex {
    BucketIndex(size_t bucketIdx, size_t dataIdx) : mBucketIdx(bucketIdx), mDataIdx(dataIdx) {};

    size_t mBucketIdx;
    size_t mDataIdx;

    bool operator<(BucketIndex const &other) const {
        return std::tie(mBucketIdx, mDataIdx) < std::tie(other.mBucketIdx, other.mDataIdx);
    }
};

template<std::forward_iterator TIter, typename TBucketPolicy>
auto groupData(const TIter& start, const TIter& end, TBucketPolicy bucketPolicy, int minCatSize, int outsider) {

}

#endif //HELPERS_H
