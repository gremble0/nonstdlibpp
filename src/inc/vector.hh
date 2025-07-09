#pragma once

#include <cassert>
#include <cstddef>
#include <format>
#include <initializer_list>
#include <memory>
#include <stdexcept>
#include <utility>

namespace nstd {

template <typename T, typename Allocator = std::allocator<T>> class vector {
  public:
    using size_type = std::size_t;
    using allocator_type = Allocator;
    using value_type = T;
    using reference = T &;
    using const_reference = const T &;
    using rvalue_reference = T &&;
    using pointer = T *;
    using const_pointer = const T *;

    struct iterator {
      public:
        reference operator*() const { return *m_ptr; }
        pointer operator->() const { return m_ptr; }

        iterator &operator++() {
            ++m_ptr;
            return *this;
        }

        iterator &operator++(int) {
            iterator ret = *this;
            ++m_ptr;
            return ret;
        }

        iterator &operator--() {
            --m_ptr;
            return *this;
        }

        iterator &operator--(int) {
            iterator ret = *this;
            --m_ptr;
            return ret;
        }

        bool operator==(const iterator &other) { return m_ptr == other.m_ptr; }
        bool operator!=(const iterator &other) { !((*this) == other); }

      private:
        pointer m_ptr;
    };

    constexpr vector() noexcept : m_data(nullptr), m_capacity(default_capacity), m_size(0) {}

    constexpr explicit vector(size_type size) noexcept : m_data(nullptr), m_capacity(size), m_size(0) {}

    constexpr vector(std::initializer_list<T> init)
        : m_data(m_allocator.allocate(init.size())), m_capacity(init.size() /* * 2? */), m_size(init.size()) {
        iterator it = begin();
        for (const auto &x : init) {
            *(it++) = x;
        }
    }

    constexpr explicit vector(const allocator_type &allocator) noexcept
        : m_data(nullptr), m_capacity(size), m_size(0), m_allocator(allocator) {}

    ~vector() {
        clear();
        if (m_data) {
            m_allocator.deallocate(m_data, m_capacity);
        }
    }

    // TODO(gremble0): rule of five - should be implemented
    vector(const vector &other) = delete;
    vector(vector &&other) = delete;
    vector &operator=(vector &other) = delete;
    vector &operator=(vector &&other) = delete;

    [[nodiscard]] constexpr iterator begin() const noexcept { return iterator{m_data}; }

    [[nodiscard]] constexpr iterator end() const noexcept { return iterator{m_data + m_size}; }

    // TODO(gremble0): iterator (begin())
    [[nodiscard]] constexpr iterator front() noexcept {
        assert(!empty());
        return begin();
    }

    [[nodiscard]] constexpr const_reference front() const noexcept {
        assert(!empty());
        return begin();
    };

    // TODO(gremble0): iterator (end() - 1)
    [[nodiscard]] constexpr iterator back() noexcept {
        assert(!empty());
        return end() - 1;
    };

    [[nodiscard]] constexpr const_reference back() const noexcept {
        assert(!empty());
        return end() - 1;
    }

    [[nodiscard]] constexpr size_type size() const noexcept { return m_size; }

    [[nodiscard]] constexpr size_type capacity() const noexcept { return m_capacity; }

    [[nodiscard]] constexpr const_pointer data() const noexcept { return m_data; }

    [[nodiscard]] constexpr pointer data() noexcept { return m_data; }

    [[nodiscard]] constexpr bool empty() const noexcept { return m_size == 0; }

    void clear() {
        if (empty()) {
            return;
        }
        // TODO(gremble0): implement
    }

    void reserve(size_type size) {
        if (size <= capacity()) {
            return;
        }
        // TODO(gremble0): implement
    }

    [[nodiscard]] constexpr reference operator[](size_type i) noexcept { return *(begin() + i); }

    [[nodiscard]] constexpr const_reference operator[](size_type i) const noexcept { return *(begin() + i); }

    [[nodiscard]] constexpr reference at(size_type i) {
        range_check(i);
        return (*this)[i];
    }

    [[nodiscard]] constexpr const_reference at(size_type i) const {
        range_check(i);
        return (*this)[i];
    }

    constexpr void push_back(const_reference x) { emplace_back(x); }

    constexpr void push_back(rvalue_reference x) { emplace_back(std::move(x)); }

    constexpr void pop_back() {
        assert(!empty());
        --m_size;
        std::destroy_at(end());
    }

    // Newer c++ versions seem to return a reference to the inserted element for emplace_back, but not for push_back?
    // This seems weird. Returning a reference to the inserted element is rarely useful anyways so we keep the API
    // constistent by making it void instead
    template <typename... Args> constexpr void emplace_back(Args... args) {
        value_type x{std::forward(args...)};
        // TODO(gremble0) implement
    }

  private:
    // This will be the capacity of a default initialized/empty vector
    static constexpr size_type default_capacity = 8;
    // How much should the vector grow when it needs to resize?
    static constexpr size_type growth_factor = 2;

    constexpr void range_check(size_type i) {
        if (i >= size()) {
            throw std::out_of_range(std::format("Index {} out of range for vector of size {}", i, m_size));
        }
    }

    allocator_type m_allocator;
    T *m_data;
    size_type m_capacity;
    size_type m_size;
};

} // namespace nstd
