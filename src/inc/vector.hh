#pragma once

#include <cassert>
#include <cstddef>
#include <memory>

namespace nstd {

template <typename T, typename Allocator = std::allocator<T>> class vector {
public:
  using size_type = std::size_t;
  using allocator_type = Allocator;
  using reference = T &;
  using const_reference = const T &;
  using pointer = T *;
  using const_pointer = const T *;

  vector() noexcept
      : m_data(nullptr), m_capacity(default_capacity), m_size(0),
        m_allocator() {}

  explicit vector(size_type size) noexcept
      : m_data(nullptr), m_capacity(size), m_size(0), m_allocator() {}

  explicit vector(const allocator_type &allocator) noexcept
      : m_data(nullptr), m_capacity(size), m_size(0), m_allocator(allocator) {}

  ~vector() {
    clear();
    if (m_data) {
      m_allocator.deallocate(m_data, m_capacity);
    }
  }

  // TODO(gremble0): rule of five
  vector(const vector &other) = delete;
  vector(vector &&other) = delete;
  vector &operator=(vector &other) = delete;
  vector &operator=(vector &&other) = delete;

  // TODO(gremble0): iterator (begin())
  [[nodiscard]] constexpr reference front() noexcept {
    assert(!empty());
    return *m_data;
  }

  [[nodiscard]] constexpr const_reference front() const noexcept {
    assert(!empty());
    return *m_data;
  };

  // TODO(gremble0): iterator (end() - 1)
  [[nodiscard]] constexpr reference back() noexcept {
    assert(!empty());
    return m_data[m_size - 1];
  };

  [[nodiscard]] constexpr const_reference back() const noexcept {
    assert(!empty());
    return m_data[m_size - 1];
  }

  [[nodiscard]] constexpr size_type size() const noexcept { return m_size; }

  [[nodiscard]] constexpr const_pointer data() const noexcept { return m_data; }

  [[nodiscard]] constexpr pointer data() noexcept { return m_data; }

  [[nodiscard]] constexpr bool empty() const noexcept { return m_size == 0; }

  void clear() {
    // TODO(gremble0): implement
  }

private:
  // This will be the capacity of a default initialized/empty vector
  static constexpr size_type default_capacity = 8;
  // How much should the vector grow when it needs to resize?
  static constexpr size_type growth_factor = 2;

  T *m_data;
  size_type m_capacity;
  size_type m_size;
  allocator_type m_allocator;
};

} // namespace nstd
