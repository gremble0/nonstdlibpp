#pragma once

#include <cstddef>
#include <utility>

namespace nstd {

template <typename T> struct default_delete {
    constexpr default_delete() noexcept = default;

    constexpr void operator()(T *ptr) const { delete ptr; }
};

template <typename T, typename Deleter = default_delete<T>> class unique_ptr {
  public:
    using pointer = T *;
    using deleter = Deleter;

    constexpr unique_ptr() noexcept = default;

    constexpr explicit unique_ptr(pointer ptr) noexcept : m_ptr(ptr) {}

    unique_ptr(const unique_ptr &other) = delete;

    unique_ptr &operator=(const unique_ptr &other) = delete;

    constexpr unique_ptr(unique_ptr &&other) noexcept {
        reset(other.get());
        other.m_ptr = nullptr;
    }

    constexpr unique_ptr &operator=(unique_ptr &&other) noexcept {
        reset(other.get());
        other.m_ptr = nullptr;

        return *this;
    }

    constexpr ~unique_ptr() { m_deleter(m_ptr); }

    [[nodiscard]] constexpr pointer get() const { return m_ptr; }

    [[nodiscard]] constexpr deleter get_deleter() const { return m_deleter; }

    constexpr pointer operator->() const noexcept { return get(); }

    [[nodiscard]] constexpr explicit operator bool() const noexcept { return get() != nullptr; }

    constexpr void reset(pointer ptr) noexcept {
        // In case deleter throws, do the deletion last. STL has a contract where custom deleters should not throw, but
        // it still seems to handle this possibility. This seems to be by design so we will do the same.
        pointer old_ptr = m_ptr;
        m_ptr = ptr;
        m_deleter(old_ptr);
    }

    constexpr pointer release() noexcept {
        pointer ptr = m_ptr;
        m_ptr = nullptr;
        return ptr;
    }

    constexpr void swap(unique_ptr &other) noexcept {
        m_deleter = std::exchange(m_deleter, other.get_deleter());
        m_ptr = std::exchange(m_ptr, other.get());
    }

  private:
    [[no_unique_address]] deleter m_deleter{};
    pointer m_ptr{nullptr};
};

template <typename T, typename... Args> constexpr unique_ptr<T> make_unique(Args... args) {
    return unique_ptr(new T{std::forward<Args>(args)...});
}

// The parameters have separate templated types because its necessary for types that may have some relation with
// eachother, but that are not the same. E.g. comparing two unrelated types won't work anyways, but comparing two
// classes related by inheritance will.
template <typename T, typename TDeleter, typename U, typename UDeleter>
[[nodiscard]] inline constexpr bool operator==(const unique_ptr<T, TDeleter> &a, const unique_ptr<U, UDeleter> &b) {
    return a.get() == b.get();
}

template <typename T, typename TDeleter>
[[nodiscard]] inline constexpr bool operator==(const unique_ptr<T, TDeleter> &a, std::nullptr_t) {
    return a.get() == nullptr;
}

template <typename T, typename TDeleter>
[[nodiscard]] inline constexpr bool operator==(std::nullptr_t, const unique_ptr<T, TDeleter> &a) {
    return a.get() == nullptr;
}

} // namespace nstd
