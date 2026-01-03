//
// Created by mshamrai on 1/3/26.
//

#ifndef COMET_SPANVIEW_H
#define COMET_SPANVIEW_H
#include <span>

template <typename TSource>
struct SpanView {
    public:
    using SourceIterType      = typename TSource::iterator;
    using SourceConstIterType = typename TSource::const_iterator;
    using SourceValueType     = typename TSource::value_type;

    using InnerSpanType   = typename std::span<const SourceConstIterType>;
    using size_type       = std::size_t;
    using difference_type = std::ptrdiff_t;
    explicit SpanView(InnerSpanType s) : m_span(s){};

    using value_type      = SourceValueType;
    using reference       = const SourceValueType &;
    using const_reference = const SourceValueType &;

    struct SpanViewIterator {
        using iterator_category = std::forward_iterator_tag;
        using difference_type   = std::ptrdiff_t;
        using value_type        = SourceValueType;
        using reference         = const value_type &;
        using pointer           = const value_type *;

        SpanViewIterator() = default;
        SpanViewIterator(InnerSpanType spanIterPtr) : m_spanIter(spanIterPtr) {}

        SpanViewIterator &operator++()
        {
            ++m_spanIter;
            return *this;
        }

        SpanViewIterator operator++(int)
        {
            SpanViewIterator copy = *this;
            ++(*this);
            return copy;
        }

        reference operator*() const { return **m_spanIter; }

        pointer operator->() const { return &(**m_spanIter); }

        friend bool operator==(const SpanViewIterator &lhs, const SpanViewIterator &rhs)
        {
            return lhs.m_spanIter == rhs.m_spanIter;
        }

        friend bool operator!=(const SpanViewIterator &lhs, const SpanViewIterator &rhs)
        {
            return lhs.m_spanIter != rhs.m_spanIter;
        }

        private:
        InnerSpanType m_spanIter;
    };

    using iterator       = SpanViewIterator;
    using const_iterator = iterator;

    [[nodiscard]] iterator begin() const { return iterator(m_span.begin()); }

    [[nodiscard]] iterator end() const { return iterator(m_span.end()); }

    [[nodiscard]] size_type size() const noexcept { return m_span.size(); }
    [[nodiscard]] bool empty() const noexcept { return m_span.empty(); }

    [[nodiscard]] reference operator[](size_type index) const { return *m_span[index]; }

    private:
    InnerSpanType m_span;
};

#endif  // COMET_SPANVIEW_H
