#pragma once

#include "normal_iterator.hh"

#include <format>
#include <memory>
#include <utility>

namespace nstd {

template <typename CharT, typename Allocator = std::allocator<CharT>> class basic_string {
  public:
    using allocator_type = Allocator;
    using size_type = Allocator::size_type;
    using const_reference = const CharT &;
    using reference = CharT &;
    using iterator = normal_iterator<CharT *, basic_string>;
    using const_iterator = normal_iterator<const CharT *, basic_string>;

    constexpr basic_string() noexcept = default;

    constexpr basic_string(size_type sz, CharT c) noexcept : m_size(sz) {
        if (sz > 0) {
            m_data = m_allocator.allocate(sz + 1);
            memset(m_data, c, sz);
            m_data[sz] = CharT{};
        }
    }

    // NOLINTNEXTLINE(google-explicit-constructor, hicpp-explicit-conversions) This is how its like in the STL
    constexpr basic_string(const CharT *src) noexcept : m_size(strlen(src)) { copy(src); }

    constexpr basic_string(const basic_string &other) noexcept : m_allocator(other.m_allocator), m_size(other.size()) {
        copy(other.m_data);
    }

    constexpr basic_string(basic_string &&other) noexcept
        : m_allocator(std::exchange(other.m_allocator, allocator_type{})), m_data(std::exchange(other.m_data, nullptr)),
          m_size(other.size(), 0) {}

    constexpr ~basic_string() noexcept { m_allocator.deallocate(m_data, m_size); }

    [[nodiscard]] constexpr const CharT *data() const noexcept { return m_data; }

    [[nodiscard]] constexpr CharT *data() noexcept { return m_data; }

    // c_str does not have mutable overload
    [[nodiscard]] constexpr const CharT *c_str() const noexcept { return m_data; }

    [[nodiscard]] constexpr size_type size() const noexcept { return m_size; }

    [[nodiscard]] constexpr reference at(size_type i) {
        range_check(i);
        return m_data[i];
    }

    [[nodiscard]] constexpr iterator begin() { return iterator(m_data); }

    [[nodiscard]] constexpr const_iterator begin() const { return const_iterator(m_data); }

    [[nodiscard]] constexpr const_iterator cbegin() const { return const_iterator(m_data); }

    [[nodiscard]] constexpr iterator end() { return iterator(m_data + m_size); }

    [[nodiscard]] constexpr const_iterator end() const { return const_iterator(m_data + m_size); }

    [[nodiscard]] constexpr const_iterator cend() const { return const_iterator(m_data + m_size); }

    [[nodiscard]] constexpr reference operator[](size_type i) noexcept { return m_data[i]; }

    [[nodiscard]] constexpr const_reference operator[](size_type i) const noexcept { return m_data[i]; }

    [[nodiscard]] constexpr bool operator==(const basic_string &other) const noexcept {
        if (size() != other.size()) {
            return false;
        }

        for (size_type i = 0; i < m_size; ++i) {
            if (m_data[i] != other.m_data[i]) {
                return false;
            }
        }

        return true;
    }

  private:
    constexpr void range_check(size_type i) const {
        if (i >= size()) {
            throw std::out_of_range(std::format("Index {} out of range for string of size {}", i, m_size));
        }
    }

    // NOTE: unsafe
    static constexpr void memset(CharT *dest, CharT c, size_type sz) {
        for (size_type i = 0; i < sz; ++i) {
            dest[i] = c;
        }
    }

    // NOTE: unsafe
    static constexpr void memcpy(CharT *dest, const CharT *src, size_type sz) {
        for (size_type i = 0; i < sz; ++i) {
            dest[i] = src[i];
        }
    }

    // NOTE: unsafe
    static constexpr size_type strlen(const CharT *s) {
        size_type len = 0;
        while (s[len] != CharT{}) {
            ++len;
        }
        return len;
    }

    constexpr void copy(const CharT *s) {
        if (m_size > 0) {
            m_data = m_allocator.allocate(m_size + 1);
            memcpy(m_data, s, m_size);
            m_data[m_size] = CharT{};
        }
    }

    [[no_unique_address]] allocator_type m_allocator;
    CharT *m_data{nullptr};
    size_type m_size{0};
};

using string = basic_string<char>;
using wstring = basic_string<wchar_t>;
using u8string = basic_string<char8_t>;
using u16string = basic_string<char16_t>;
using u32string = basic_string<char32_t>;

} // namespace nstd
