#pragma once

#include "normal_iterator.hh"

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <format>
#include <initializer_list>
#include <memory>
#include <stdexcept>
#include <utility>

// STL marks several destructor calling functions noexcept, even though they internally calls destructors that are not
// necessarily noexcept ? We do the same here, so just use noexcept for these functions to stay consistent with the STL

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
    using iterator = normal_iterator<pointer, vector>;
    using const_iterator = normal_iterator<const_pointer, vector>;

    constexpr vector() = default;

    constexpr explicit vector(size_type size) noexcept : m_capacity(size) {}

    constexpr vector(std::initializer_list<T> init)
        : m_data(init.size() > 0 ? m_allocator.allocate(init.size()) : nullptr), m_capacity(init.size()),
          m_size(init.size()) {
        range_copy(init.begin(), init.end(), begin());
    }

    constexpr explicit vector(const allocator_type &allocator) noexcept : m_capacity(size), m_allocator(allocator) {}

    constexpr ~vector() noexcept {
        clear();
        if (m_data) {
            m_allocator.deallocate(m_data, m_capacity);
        }
    }

    vector(const vector &other)
        : m_allocator(other.get_allocator()), m_data(nullptr), m_capacity(other.capacity()), m_size(other.size()) {
        reserve(other.size());
        range_copy(other.begin(), other.end(), begin());
    }

    vector(vector &&other) noexcept
        : m_allocator(std::move(other.get_allocator())), m_data(other.data()), m_capacity(other.capacity()),
          m_size(other.size()) {
        other.m_data = nullptr;
        other.m_capacity = 0;
        other.m_size = 0;
    }

    // TODO(gremble0): rule of five - should be implemented
    vector &operator=(vector &other) = delete;
    vector &operator=(vector &&other) = delete;

    [[nodiscard]] constexpr bool operator==(const vector &other) const {
        return (size() == other.size()) && std::equal(begin(), end(), other.begin(), other.end());
    }

    [[nodiscard]] constexpr bool operator!=(const vector &other) { return !(*this == other); }

    [[nodiscard]] constexpr iterator begin() noexcept { return iterator{m_data}; }

    [[nodiscard]] constexpr const_iterator begin() const noexcept { return const_iterator{m_data}; }

    [[nodiscard]] constexpr const_iterator cbegin() const noexcept { return const_iterator{m_data}; }

    [[nodiscard]] constexpr iterator end() noexcept { return iterator{m_data + m_size}; }

    [[nodiscard]] constexpr const_iterator end() const noexcept { return const_iterator{m_data + m_size}; }

    [[nodiscard]] constexpr const_iterator cend() const noexcept { return const_iterator{m_data + m_size}; }

    [[nodiscard]] constexpr iterator front() noexcept {
        assert(!empty());
        return begin();
    }

    [[nodiscard]] constexpr const_reference front() const noexcept {
        assert(!empty());
        return begin();
    };

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

    [[nodiscard]] constexpr allocator_type get_allocator() const noexcept { return m_allocator; }

    [[nodiscard]] constexpr bool empty() const noexcept { return m_size == 0; }

    constexpr void clear() noexcept {
        std::destroy(begin(), end());
        m_size = 0;
    }

    void reserve(size_type size) {
        auto *new_data = m_allocator.allocate(size);

        if (m_data) {
            // Move data if we have any
            std::uninitialized_move(begin(), end(), new_data);
            std::destroy(begin(), end());
            m_allocator.deallocate(m_data, capacity());
        }

        m_data = new_data;
        m_capacity = size;
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

    // TODO(gremble0): STL has constexpr push/emplace_back. We cant do this right now because reserve() is not constexpr
    void push_back(const_reference x) { emplace_back(x); }

    void push_back(rvalue_reference x) { emplace_back(std::move(x)); }

    constexpr void pop_back() noexcept {
        assert(!empty());
        --m_size;
        std::destroy_at(end());
    }

    // Newer c++ versions seem to return a reference to the inserted element for emplace_back, but not for push_back?
    // This seems weird. Returning a reference to the inserted element is rarely useful anyways so we keep the API
    // constistent by making it void instead
    template <typename... Args> void emplace_back(Args &&...args) {
        if (m_size >= m_capacity) {
            reserve(m_capacity * s_growth_factor);
        }

        std::construct_at(&*end(), std::forward<Args>(args)...);
        ++m_size;
    }

  private:
    // This will be the capacity of a default initialized/empty vector
    static constexpr size_type s_default_capacity = 8;
    // How much should the vector grow when it needs to resize?
    static constexpr size_type s_growth_factor = 2;

    constexpr void range_check(size_type i) {
        if (i >= size()) {
            throw std::out_of_range(std::format("Index {} out of range for vector of size {}", i, m_size));
        }
    }

    template <typename FromIterator, typename ToIterator>
    constexpr void range_copy(FromIterator from_start, FromIterator from_end, ToIterator to_start) {
        try {
            // uninitialized_copy is constexpr in c++26. With c++23 this constructor is not really constexpr, but I will
            // leave it like this for the future.
            std::uninitialized_copy(from_start, from_end, to_start);
        } catch (...) {
            // We cannot know the type of what is thrown here since it can depend on user defined types. We just have to
            // catch everything and clean up before finishing
            if (m_data) {
                m_allocator.deallocate(m_data, m_capacity);
            }
            throw;
        }
    }

    [[no_unique_address]] allocator_type m_allocator;
    pointer m_data{nullptr};
    size_type m_capacity{0};
    size_type m_size{0};
};

} // namespace nstd
