#pragma once

#include <cstddef>
#include <memory>

namespace nstd {

template <typename T, typename Allocator = std::allocator<T>> class vector {
public:
  using size_type = std::size_t;
  using reference = T &;
  using const_reference = const T &;
  using pointer = T *;
  using const_pointer = const T *;

  vector() noexcept
      : m_data(Allocator(byte_size(default_capacity))),
        m_capacity(default_capacity), m_size(0) {}
  vector(size_type size) noexcept
      : m_data(Allocator(byte_size(default_capacity))), m_capacity(size),
        m_size(0) {}

  constexpr reference front() noexcept;
  constexpr const_reference front() const noexcept;

  constexpr reference back() noexcept;
  constexpr const_reference back() const noexcept;

  constexpr size_type size() const noexcept { return m_size; }

  constexpr const_pointer data() const noexcept { return m_data; }
  constexpr pointer data() noexcept { return m_data; }

private:
  // This will be the capacity of a default initialized/empty vector
  static constexpr size_type default_capacity = 8;
  // How much should the vector grow when it needs to resize?
  static constexpr size_type resize_factor = 2;

  static constexpr size_type byte_size(size_type s) { return s * sizeof(T); }

  T *m_data;
  size_type m_capacity;
  size_type m_size;
};

} // namespace nstd
