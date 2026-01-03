//
// Created by Mateusz Mikiciuk on 29/12/2025.
//

#ifndef HELPERS_H
#define HELPERS_H

#include <map>
#include <tuple>

// TODO: concepts for TBucketPolicy

template <typename... Types, typename Func>
auto tupleTransform(const std::tuple<Types...> &t_tuple, Func &&t_fn)
{
    return std::apply(
        [&]<typename... InnerTypes>(InnerTypes &&...elem) {
            return std::make_tuple(t_fn(std::forward<InnerTypes>(elem))...);
        },
        t_tuple
    );
}

// sort of std::map<> specialization type
template <typename TBucketPolicy, typename TInput>
struct SingleBlockBuckets {
    using underlying_iterator_type = typename TInput::const_iterator;
    // TODO: consider std::unordered_map<> here, requires further tests
    using container_type = std::map<int, std::vector<underlying_iterator_type>>;
    using iterator       = typename container_type::const_iterator;
    using value_type     = typename container_type::value_type;

    SingleBlockBuckets() = default;
    explicit SingleBlockBuckets(
        const TBucketPolicy &t_bucketPolicy, const TInput &t_input, const int t_minCatSize
    )
    {
        createSingle(t_bucketPolicy, t_input, t_minCatSize);
    }

    // sync api
    iterator begin() const { return m_buckets.begin(); }
    iterator end() const { return m_buckets.end(); }
    iterator erase(iterator t_it) { return m_buckets.erase(t_it); }
    value_type at(int t_bucketNum) const { return m_buckets.at(t_bucketNum); }
    [[nodiscard]] bool contains(int t_bucketNum) const { return m_buckets.contains(t_bucketNum); }

    private:
    container_type m_buckets;

    // new version of groupData() function
    void createSingle(
        const TBucketPolicy &t_bucketPolicy, const TInput &t_input, const size_t t_minCatSize
    )
    {
        for (auto it = t_input.begin(); it != t_input.end(); ++it) {
            const int bucketNumber = t_bucketPolicy.getBucket(*it);
            m_buckets[bucketNumber].push_back(it);
        }

        // We need this second loop,
        // bc in the one above we do not know when we hit last element in bucket.
        for (auto it = m_buckets.begin(); it != m_buckets.end();) {
            if (it->second.size() < t_minCatSize) {
                it = m_buckets.erase(it);
            } else {
                ++it;
            }
        }
    }
};

template <typename TBucketPolicy, typename... TInputs>
struct CoupledBlockBuckets {
    CoupledBlockBuckets() = default;
    explicit CoupledBlockBuckets(
        const TBucketPolicy &t_bucketPolicy, const int t_minCatSize, const TInputs &...t_inputs
    )
    {
        m_data = tupleTransform(std::forward_as_tuple(t_inputs...), [&](auto &&t_input) {
            return SingleBlockBuckets(t_bucketPolicy, t_input, t_minCatSize);
        });
        syncBuckets();  // syncBuckets() has to be called before initialization of iterators!
        m_current = tupleTransform(m_data, [&](auto &&t_single) {
            return t_single.begin();
        });
        m_end     = tupleTransform(m_data, [&](auto &&t_single) {
            return t_single.end();
        });
    }

    void nextIterators()
    {
        std::apply(
            [](auto &...t_iters) {
                (++t_iters, ...);
            },
            m_current
        );
    }

    [[nodiscard]] bool isEnd() const
    {
        return std::get<0>(m_current) ==
               std::get<0>(m_end);  // if one have finished, then all have finished
    }

    [[nodiscard]] auto currentBuckets() const
    {
        return std::apply(
            [](auto const &...iters) {
                return std::forward_as_tuple(iters->second...);
            },
            m_current
        );
    }

    private:
    std::tuple<SingleBlockBuckets<TBucketPolicy, TInputs>...> m_data;
    std::tuple<typename SingleBlockBuckets<TBucketPolicy, TInputs>::iterator...> m_current;
    std::tuple<typename SingleBlockBuckets<TBucketPolicy, TInputs>::iterator...> m_end;

    void syncHelper(auto &firstData, const auto &comparedData)
    {
        for (auto it = firstData.begin(); it != firstData.end();) {
            if (!comparedData.contains(it->first)) {
                it = firstData.erase(it);
            } else {
                ++it;
            }
        }
    }

    void syncBuckets()
    {
        constexpr size_t N = sizeof...(TInputs);
        auto &firstData    = std::get<0>(m_data);
        [&]<std::size_t... Is>(const std::index_sequence<Is...> &) {
            (syncHelper(firstData, std::get<Is>(m_data)), ...);
            (syncHelper(std::get<Is>(m_data), firstData), ...);
        }(std::make_index_sequence<N>());
    }
};

#endif  // HELPERS_H
