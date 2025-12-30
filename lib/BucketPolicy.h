//
// Created by mshamrai on 7/17/25.
//

#ifndef BUCKET_POLICY_H
#define BUCKET_POLICY_H

#include <algorithm>
#include <array>
#include <tuple>
#include <type_traits>
#include <vector>

template <typename T1, typename T2>
concept is_less_comparable = requires(T1 t, T2 u) {
    { t < u } -> std::same_as<bool>;
};

// template <typename TElement, typename... TCallables>
// concept is_function_callable_on_element = (std::invocable<TCallables, TElement> && ...);
//
// template <typename TElement, typename... TCallables>
// concept is_function_bucketable_on_doubles =
//     is_function_callable_on_element<TElement, TCallables...> &&
//     (std::is_convertible_v<std::invoke_result_t<TCallables, TElement>, double> && ...);

template <typename TBucketPolicy, typename TIter>
concept is_bucket_policy = requires(TBucketPolicy policy, TIter iter) {
    { policy.getBucket(*iter) } -> std::same_as<int>;
    { policy.getMaximalBucketCount() } -> std::same_as<int>;
    policy.getValues(*iter);
};

template <typename TContainer, typename T2>
requires is_less_comparable<typename TContainer::value_type, T2>
int findIndex(TContainer const &data, T2 const &value, const bool ignoreOverflows)
{
    const auto tmp =
        static_cast<int>(std::distance(data.begin(), std::upper_bound(data.begin(), data.end(), value)));
    return ignoreOverflows && (tmp == 0 || tmp == static_cast<int>(data.size())) ? -1 : tmp;
}

// define type traits to filter arrays
// general template to filter all types, excluding arrays - inherits from std::false_type
template <typename T>
struct is_double_array : std::false_type {};

// template specialization for std::array<double, N> - inherits from std::true_type
template <std::size_t N>
struct is_double_array<std::array<double, N>> : std::true_type {};

template <typename... TMixedArgs>
struct BucketPolicy final {
    private:
    template <bool Condition, typename T>
    using filter_t = std::conditional_t<Condition, std::tuple<std::decay_t<T>>, std::tuple<>>;

    using CallablesTuple = decltype(std::tuple_cat(
        std::declval<filter_t<!is_double_array<std::decay_t<TMixedArgs>>::value, TMixedArgs>>()...
    ));

    using ArraysTuple = decltype(std::tuple_cat(
        std::declval<filter_t<is_double_array<std::decay_t<TMixedArgs>>::value, TMixedArgs>>()...
    ));

    public:

    explicit BucketPolicy(const bool t_ignoreOverflows, TMixedArgs &&...t_mixedArgs)
    :   m_callables(extract_by_predicate<false>(std::forward<TMixedArgs>(t_mixedArgs)...)),
        m_bucketsRanges(extract_by_predicate<true>(std::forward<TMixedArgs>(t_mixedArgs)...)),
        m_ignoreOverflows(t_ignoreOverflows)
    {}

    template <typename TElement>
        // requires is_function_bucketable_on_doubles<TElement, TCallables...>
    [[nodiscard]] int getBucket(TElement const &element) const
    {
        auto values  = getValues(element);
        auto indices = getUpperIndicesForTuple(values);
        auto bucket  = calculateBucketAtIndices(indices);
        return bucket;
    }

    private:

    template <bool SelectArrays, typename... Ts>
    static auto extract_by_predicate(Ts &&...args)
    {
        return std::tuple_cat(
            conditional_make_tuple<SelectArrays>(std::forward<Ts>(args))...
        );
    }

    template <bool SelectArrays, typename T>
    static auto conditional_make_tuple(T &&arg)
    {
        if constexpr (is_double_array<std::decay_t<T>>::value == SelectArrays) {
            return std::make_tuple(std::forward<T>(arg));
        }
        else {
            return std::tuple{};
        }
    }

    public:

    [[nodiscard]] int getMaximalBucketCount() const
    {
        return [&]<std::size_t... I>(std::index_sequence<I...>) {
            return (1 * ... * (std::get<I>(m_bucketsRanges).size() + 1));
        }(std::make_index_sequence<std::tuple_size_v<CallablesTuple>>{});
    }

    template <typename TElement>
    [[nodiscard]] auto getValues(const TElement &element) const
    {
        return [&]<std::size_t... I>(std::index_sequence<I...>) {
            return std::make_tuple(std::get<I>(m_callables)(element)...);
        }(std::make_index_sequence<std::tuple_size_v<CallablesTuple>>{});
    }

    template <typename... Types>
    [[nodiscard]] auto getUpperIndicesForTuple(const std::tuple<Types...> &values) const
    {
        return [&]<std::size_t... I>(std::index_sequence<I...>) {
            return std::make_tuple(
                findIndex(std::get<I>(m_bucketsRanges), std::get<I>(values), m_ignoreOverflows)...
            );
        }(std::make_index_sequence<std::tuple_size_v<CallablesTuple>>{});
    }

    template <typename... TIndices>
    [[nodiscard]] int calculateBucketAtIndices(const std::tuple<TIndices...> &indices) const
    {
        constexpr auto N = sizeof...(TIndices);
        auto indexSeq    = std::make_index_sequence<N - 1>();
        return m_ignoreOverflows && checkUnderOverflows(indices)
                   ? -1
                   : [&]<size_t... I>(std::index_sequence<I...>) {
                         return (
                             std::get<0>(indices) + ... +
                             (std::get<I + 1>(indices) *
                              (1 * ... * (std::get<I>(m_bucketsRanges).size() + 1)))
                         );
                     }(indexSeq);
    }

    template <typename... Types>
    bool checkUnderOverflows(const std::tuple<Types...> &arg) const
    {
        return [&]<size_t... I>(std::index_sequence<I...>) {
            return ((std::get<I>(arg) == -1) || ...);
        }(std::make_index_sequence<sizeof...(Types)>{});
    }

    CallablesTuple m_callables;
    ArraysTuple m_bucketsRanges;
    bool m_ignoreOverflows;
};


template <typename... Args>
BucketPolicy(bool, Args&&...) -> BucketPolicy<Args...>;

#endif  // BUCKET_POLICY_H
