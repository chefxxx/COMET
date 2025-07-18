//
// Created by Mateusz Mikiciuk on 18/07/2025.
//

#ifndef HELPERS_H
#define HELPERS_H

#include <iterator>
#include <map>
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
auto groupData(TIter start, TIter end, TBucketPolicy bucketPolicy, int outsider) {
    size_t dataIdx = 0;
    std::map<BucketIndex, std::vector<TIter>> buckets;
}

inline int findUpperIndex(std::vector<double> const& data, double value) {
    if (data.empty())
        return -1;
    return static_cast<int>(distance(data.begin(), std::upper_bound(data.begin(), data.end(), value)));
}

#endif //HELPERS_H
