#pragma once

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

    constexpr pointer operator->() const noexcept { return m_ptr; }

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

    constexpr ~unique_ptr() { m_deleter(m_ptr); }

    constexpr pointer get() const { return m_ptr; }

  private:
    [[no_unique_address]] deleter m_deleter{};
    pointer m_ptr{nullptr};
};

template <typename T, typename... Args> constexpr unique_ptr<T> make_unique(Args... args) {
    return unique_ptr(new T{std::forward<Args>(args)...});
}

} // namespace nstd
