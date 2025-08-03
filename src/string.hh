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

    [[nodiscard]] constexpr pointer data() const noexcept { return m_data; }

    [[nodiscard]] constexpr size_type size() const noexcept { return m_size; }

  private:
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
