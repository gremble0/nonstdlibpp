#pragma once

#include <iterator>

template <typename Iterator, typename Container> struct normal_iterator {
  public:
    // Needed as traits for several STL functions
    using value_type = typename std::iterator_traits<Iterator>::value_type;
    using reference = typename std::iterator_traits<Iterator>::reference;
    using pointer = typename std::iterator_traits<Iterator>::pointer;
    using difference_type = typename std::iterator_traits<Iterator>::difference_type;
    using iterator_category = typename std::iterator_traits<Iterator>::iterator_category;

    constexpr explicit normal_iterator(pointer ptr) : m_ptr(ptr) {}

    constexpr reference operator*() const noexcept { return *m_ptr; }

    constexpr pointer operator->() const noexcept { return m_ptr; }

    constexpr normal_iterator &operator++() noexcept {
        ++m_ptr;
        return *this;
    }

    constexpr normal_iterator &operator++(int) noexcept {
        normal_iterator &ret = *this;
        ++m_ptr;
        return ret;
    }

    constexpr normal_iterator &operator--() noexcept {
        --m_ptr;
        return *this;
    }

    constexpr normal_iterator &operator--(int) noexcept {
        normal_iterator &ret = *this;
        --m_ptr;
        return ret;
    }

    constexpr reference operator[](difference_type n) const noexcept { return m_ptr[n]; }

    constexpr normal_iterator &operator-=(difference_type n) noexcept {
        m_ptr -= n;
        return *this;
    }

    constexpr normal_iterator &operator+=(difference_type n) noexcept {
        m_ptr += n;
        return *this;
    }

    constexpr normal_iterator operator-(difference_type n) const noexcept { return normal_iterator{m_ptr - n}; }

    constexpr normal_iterator operator+(difference_type n) const noexcept { return normal_iterator{m_ptr + n}; }

    constexpr bool operator==(const normal_iterator &other) const noexcept { return m_ptr == other.m_ptr; }

    constexpr bool operator!=(const normal_iterator &other) const noexcept { return !((*this) == other); }

  private:
    pointer m_ptr;
};
