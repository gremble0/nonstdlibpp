#pragma once

#include "normal_iterator.hh"
#include <memory>

namespace nstd {

template <typename CharT, typename Allocator = std::allocator<CharT>> class basic_string {
  public:
    using allocator_type = Allocator;
    using size_type = Allocator::size_type;
    using pointer = CharT *;
    using iterator = normal_iterator<pointer, basic_string>;

    constexpr basic_string() noexcept = default;

    constexpr basic_string(size_type sz, CharT c) noexcept : m_data(m_allocator.allocate(sz)), m_size(sz) {
        memset(m_data, c, sz);
    }

    [[nodiscard]] constexpr pointer data() const noexcept { return m_data; }

    [[nodiscard]] constexpr size_type size() const noexcept { return m_size; }

  private:
    static constexpr void memset(pointer dest, CharT c, size_type sz) {
        for (size_type i = 0; i < sz; ++i) {
            dest[i] = c;
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
