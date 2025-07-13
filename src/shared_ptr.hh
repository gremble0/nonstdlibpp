#pragma once

#include <atomic>
#include <cassert>
#include <cstdint>

namespace nstd {

// TODO(gremble0): deleter template, maybe some allocator stuff elsewhere too
class shared_count_base {
  public:
    constexpr shared_count_base() noexcept = default;

    constexpr void add_reference() noexcept {
        ++m_ref_count;
        ++m_weak_count;
    }

    constexpr void add_weak_reference() noexcept { ++m_weak_count; }

    constexpr void release() noexcept {
        if (--m_ref_count == 0) {
            delete this;
        }

        --m_weak_count;
    }

    constexpr void weak_release() noexcept { --m_weak_count; }

    [[nodiscard]] constexpr int64_t use_count() const noexcept { return m_ref_count; }

    shared_count_base(const shared_count_base &other) = delete;
    shared_count_base &operator=(const shared_count_base &other) = delete;

  private:
    // TODO(gremble0): maybe some smarter locking - here we have to lock and unlock twice when strong releasing
    std::atomic<int64_t> m_ref_count = 1;
    std::atomic<int64_t> m_weak_count = 1;
};

class shared_count {
  public:
    constexpr shared_count() noexcept = default;

    constexpr explicit shared_count(shared_count_base *count) noexcept : m_count(count) { assert(m_count); }

    constexpr shared_count(const shared_count &other) noexcept {
        if (other.m_count != nullptr) {
            m_count = other.m_count;
            m_count->add_reference();
        }
    }

    constexpr ~shared_count() noexcept {
        if (m_count) {
            m_count->release();
        }
    }

    shared_count &operator=(const shared_count &other) = delete;

    shared_count(shared_count &&other) = delete;

    shared_count &operator=(shared_count &&other) = delete;

    [[nodiscard]] constexpr int64_t use_count() const noexcept { return m_count == nullptr ? 0 : m_count->use_count(); }

  private:
    shared_count_base *m_count{nullptr};
};

template <typename T> class shared_ptr {
  public:
    using pointer = T *;

    template <typename U, typename... Args> friend constexpr shared_ptr<U> make_shared(Args... args);

    constexpr shared_ptr() noexcept = default;

    constexpr ~shared_ptr() noexcept = default;

    // TODO(gremble0): noexcept
    constexpr explicit shared_ptr(pointer ptr) : shared_ptr(ptr, shared_count{new shared_count_base}) {}

    constexpr shared_ptr(const shared_ptr &other) noexcept = default;

    // TODO(gremble0): these should not be deleted
    constexpr shared_ptr(shared_ptr &&other) noexcept = delete;
    constexpr shared_ptr &operator=(const shared_ptr &other) noexcept = delete;
    constexpr shared_ptr &operator=(shared_ptr &&other) noexcept = delete;

    constexpr pointer get() const { return m_ptr; }

    constexpr pointer operator->() const noexcept { return get(); }

    constexpr T &operator*() const noexcept { return *get(); }

    [[nodiscard]] constexpr int64_t use_count() const noexcept { return m_count.use_count(); }

  private:
    // Used by make shared
    constexpr explicit shared_ptr(pointer ptr, const shared_count &count) noexcept : m_ptr(ptr), m_count(count) {
        assert(m_ptr);
    }

    pointer m_ptr{nullptr};
    shared_count m_count{};
};

template <typename T, typename... Args> constexpr shared_ptr<T> make_shared(Args... args) {
    return shared_ptr(new T{std::forward<Args>(args)...}, shared_count{new shared_count_base});
}

} // namespace nstd
