//
// Created by mshamrai on 7/17/25.
//

#ifndef BUCKET_POLICY_H
#define BUCKET_POLICY_H

#include <algorithm>
#include <tuple>
#include <type_traits>

// Is this specific function callable with this specific element?
template <typename Func, typename TElement>
concept InvocableWith = std::invocable<Func, TElement>;

// Does the result of that function match the bucket's type?
template <typename Func, typename TElement, typename TBucket>
concept ReturnsBucketCompatibleType = requires(Func f, TElement e) {
    {
        f(e)
    } -> std::convertible_to<typename TBucket::value_type>;
};

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
        [&]<std::size_t... I>(std::index_sequence<I...>) {
            static_assert(
                (InvocableWith<std::tuple_element_t<I, CallablesTuple>, TElement> && ...),
                "ERROR: One of your callables cannot be called with the element you provided for "
                "bucketing."
            );

            static_assert(
                (ReturnsBucketCompatibleType<
                     std::tuple_element_t<I, CallablesTuple>, TElement,
                     std::tuple_element_t<I, BucketsTuple>> &&
                 ...),
                "ERROR: A callable returns a type that doesn't match its corresponding bucket."
            );
        }(std::make_index_sequence<N>{});

        auto values  = getValues(element);
        auto indices = getUpperIndicesForTuple(values);
        auto bucket  = calculateBucketAtIndices(indices);
        return bucket;
    }

    private:
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

    template <typename TElement>
    [[nodiscard]] auto getValues(const TElement &element) const
    {
        return [&]<std::size_t... I>(std::index_sequence<I...>) {
            return std::make_tuple(std::get<I>(m_callables)(element)...);
        }(std::make_index_sequence<N>{});
    }

    template <typename... Types>
    [[nodiscard]] auto getUpperIndicesForTuple(const std::tuple<Types...> &values) const
    {
        return [&]<std::size_t... I>(std::index_sequence<I...>) {
            return std::make_tuple(
                findIndex(std::get<I>(m_buckets), std::get<I>(values), m_ignoreOverflows)...
            );
        }(std::make_index_sequence<N>{});
    }

    template <typename... TIndices>
    [[nodiscard]] int calculateBucketAtIndices(const std::tuple<TIndices...> &indices) const
    {
        constexpr auto N = sizeof...(TIndices);
        auto indexSeq    = std::make_index_sequence<N - 1>();
        return m_ignoreOverflows && checkUnderOverflows(indices)
                   ? -1
                   : [&]<std::size_t... I>(std::index_sequence<I...>) {
                         return (
                             std::get<0>(indices) + ... +
                             (std::get<I + 1>(indices) *
                              (1 * ... * (std::get<I>(m_buckets).size() + 1)))
                         );
                     }(indexSeq);
    }

    template <typename... Types>
    bool checkUnderOverflows(const std::tuple<Types...> &arg) const
    {
        return [&arg]<std::size_t... I>(std::index_sequence<I...>) {
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
