//
// Created by mshamrai on 7/17/25.
//

#ifndef BUCKETPOLICY_H
#define BUCKETPOLICY_H

#include <type_traits>

template <typename... TCallables>
struct BucketPolicy {
    BucketPolicy(std::tuple<TCallables...> callables, std::array<std::vector<double>, sizeof...(TCallables)> bucketsRanges) {}

private:
    std::tuple<TCallables...> mCallables;
    std::array<std::vector<double>, sizeof...(TCallables)> mBucketsRanges;

    auto getBucket();
    auto getBucketRangeVal();
};

/* Defines element on which lambda funcion can be called */
//template <typename TElement>
//concept is_element = requires (TElement element) {
//
//};

template <typename TCallable, typename TElement>
concept is_callable = requires (TCallable callable, TElement element) {

};


#endif //BUCKETPOLICY_H


