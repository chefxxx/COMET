//
// Created by mshamrai on 7/17/25.
//

#ifndef BUCKET_POLICY_H
#define BUCKET_POLICY_H

#include <algorithm>
#include <tuple>
#include <type_traits>

template <typename Container, typename T2>
int findIndex(Container const &data, T2 const &value, const bool ignoreOverflows)
{
    const auto tmp = static_cast<int>(
        std::distance(data.begin(), std::upper_bound(data.begin(), data.end(), value))
    );
    return ignoreOverflows && (tmp == 0 || tmp == static_cast<int>(data.size())) ? -1 : tmp;
}

template <typename... AllArgs>
struct BucketPolicy final {
    explicit BucketPolicy(const bool t_ignoreOverflows, AllArgs &&...t_args)
        : m_callables(
              extract_from_tuple<0>(std::make_index_sequence<N>{}, std::forward_as_tuple(t_args...))
          ),
          m_buckets(
              extract_from_tuple<N>(std::make_index_sequence<N>{}, std::forward_as_tuple(t_args...))
          ),
          m_ignoreOverflows(t_ignoreOverflows)
    {
    }

    template <typename TElement>
    [[nodiscard]] int getBucket(TElement const &element) const
    {
        auto values  = getValues(element);
        auto indices = getUpperIndicesForTuple(values);
        auto bucket  = calculateBucketAtIndices(indices);
        return bucket;
    }

    private:
    // Note: I assumed that user passes equal number of bins and callables.
    // TODO: Later we will provide concepts restricting types of those.
    static_assert(sizeof...(AllArgs) % 2 == 0);
    static constexpr auto N = sizeof...(AllArgs) / 2;
    using FullTuple         = std::tuple<AllArgs...>;

    template <std::size_t Offset, typename Seq>
    struct MapTuple;
    template <std::size_t Offset, std::size_t... I>
    struct MapTuple<Offset, std::index_sequence<I...>> {
        using type = std::tuple<std::decay_t<std::tuple_element_t<Offset + I, FullTuple>>...>;
    };

    using CallablesTuple = typename MapTuple<0, std::make_index_sequence<N>>::type;
    using BucketsTuple   = typename MapTuple<N, std::make_index_sequence<N>>::type;

    template <std::size_t Offset, std::size_t... I, typename TFullType>
    static auto extract_from_tuple(std::index_sequence<I...>, TFullType &&t_full)
    {
        return std::make_tuple(std::get<Offset + I>(std::forward<TFullType>(t_full))...);
    }

    // TODO: ghost function
    [[nodiscard]] constexpr int getMaximalBucketCount() const
    {
        return [&]<std::size_t... I>(std::index_sequence<I...>) {
            return (1 * ... * (std::get<I>(m_buckets).size() + 1));
        }(std::make_index_sequence<N>{});
    }

    template <typename TElement>
    [[nodiscard]] auto getValues(TElement const &element) const
    {
        return [&]<std::size_t... I>(std::index_sequence<I...>) {
            return std::make_tuple(std::get<I>(m_callables)(element)...);
        }(std::make_index_sequence<N>{});
    }

    template <typename... Types>
    [[nodiscard]] auto getUpperIndicesForTuple(std::tuple<Types...> const &values) const
    {
        return [&]<std::size_t... I>(std::index_sequence<I...>) {
            return std::make_tuple(
                findIndex(std::get<I>(m_buckets), std::get<I>(values), m_ignoreOverflows)...
            );
        }(std::make_index_sequence<N>{});
    }

    template <typename... TIndices>
    [[nodiscard]] int calculateBucketAtIndices(std::tuple<TIndices...> const &indices) const
    {
        constexpr auto N = sizeof...(TIndices);
        auto indexSeq    = std::make_index_sequence<N - 1>();
        return m_ignoreOverflows && checkUnderOverflows(indices)
                   ? -1
                   : [&]<size_t... I>(std::index_sequence<I...>) {
                         return (
                             std::get<0>(indices) + ... +
                             (std::get<I + 1>(indices) *
                              (1 * ... * (std::get<I>(m_buckets).size() + 1)))
                         );
                     }(indexSeq);
    }

    template <typename... Types>
    bool checkUnderOverflows(std::tuple<Types...> arg) const
    {
        return [&arg]<size_t... I>(std::index_sequence<I...>) {
            return ((std::get<I>(arg) == -1) || ...);
        }(std::make_index_sequence<sizeof...(Types)>{});
    }

    CallablesTuple m_callables;
    BucketsTuple m_buckets;
    bool m_ignoreOverflows;
};

// Deduction guide for the compiler
template <typename... Args>
BucketPolicy(bool, Args &&...) -> BucketPolicy<Args...>;

#endif  // BUCKET_POLICY_H
