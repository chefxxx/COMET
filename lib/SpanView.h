//
// Created by mshamrai on 1/3/26.
//

#ifndef COMET_SPANVIEW_H
#define COMET_SPANVIEW_H

template <typename TSpanType>
struct SpanView {
    using OriginalSpanIter = typename TSpanType::iterator;
    using SourceIterType   = typename TSpanType::element_type;
    using SourceValueType  = typename std::iterator_traits<SourceIterType>::value_type;

    using size_type       = std::size_t;
    using difference_type = std::ptrdiff_t;
    explicit SpanView(const TSpanType &s) : m_span(s){};

    using value_type      = SourceValueType;
    using reference       = const SourceValueType &;
    using const_reference = const SourceValueType &;

    struct SpanViewIterator {
        using iterator_category = std::bidirectional_iterator_tag;
        using difference_type   = std::ptrdiff_t;
        using value_type        = SourceValueType;
        using reference         = const value_type &;
        using pointer           = const value_type *;

        SpanViewIterator() = default;
        explicit SpanViewIterator(OriginalSpanIter spanIter) : m_spanIter(spanIter) {}

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

        SpanViewIterator &operator--()
        {
            --m_spanIter;
            return *this;
        }

        SourceIterType operator--(int)
        {
            SpanViewIterator copy = *this;
            --(*this);
            return copy;
        }
        SpanViewIterator &operator+=(difference_type n)
        {
            m_spanIter += n;
            return *this;
        }

        SpanViewIterator &operator-=(difference_type n)
        {
            m_spanIter -= n;
            return *this;
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

        reference operator[](difference_type n) const { return *m_spanIter[n]; }

        friend SpanViewIterator operator+(SpanViewIterator it, difference_type n)
        {
            return SpanViewIterator(it.m_spanIter + n);
        }
        friend SpanViewIterator operator+(difference_type n, SpanViewIterator it)
        {
            return SpanViewIterator(it.m_spanIter + n);
        }
        friend SpanViewIterator operator-(SpanViewIterator it, difference_type n)
        {
            return SpanViewIterator(it.m_spanIter - n);
        }
        friend difference_type operator-(const SpanViewIterator &a, const SpanViewIterator &b)
        {
            return a.m_spanIter - b.m_spanIter;
        }

        private:
        OriginalSpanIter m_spanIter;
    };

    using iterator       = SpanViewIterator;
    using const_iterator = iterator;

    [[nodiscard]] iterator begin() const { return iterator(m_span.begin()); }

    [[nodiscard]] iterator end() const { return iterator(m_span.end()); }

    [[nodiscard]] size_type size() const noexcept { return m_span.size(); }
    [[nodiscard]] bool empty() const noexcept { return m_span.empty(); }

    [[nodiscard]] reference operator[](size_type index) const { return *m_span[index]; }

    private:
    TSpanType m_span;
};

#endif  // COMET_SPANVIEW_H
