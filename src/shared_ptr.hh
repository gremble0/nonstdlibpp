#pragma once

#include <atomic>
#include <cassert>
#include <cstddef>

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

    shared_count_base(const shared_count_base &other) = delete;
    shared_count_base &operator=(const shared_count_base &other) = delete;

  private:
    // TODO(gremble0): maybe some smarter locking - here we have to lock and unlock twice when strong releasing
    std::atomic<size_t> m_ref_count = 1;
    std::atomic<size_t> m_weak_count = 1;
};

class shared_count {
  public:
    constexpr shared_count() noexcept = default;

    shared_count(const shared_count &other) {
        if (other.m_count == nullptr) {
            m_count = new shared_count_base;
        } else {
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

  private:
    shared_count_base *m_count{nullptr};
};

template <typename T> class shared_ptr {
  public:
    using pointer = T *;

    constexpr shared_ptr() noexcept = default;

    constexpr ~shared_ptr() noexcept = default;

    constexpr explicit shared_ptr(pointer ptr) noexcept : m_ptr(ptr) {}

    constexpr shared_ptr(const shared_ptr &other) noexcept = default;

    // TODO(gremble0): these should not be deleted
    constexpr shared_ptr(shared_ptr &&other) noexcept = delete;
    constexpr shared_ptr &operator=(const shared_ptr &other) noexcept = delete;
    constexpr shared_ptr &operator=(shared_ptr &&other) noexcept = delete;

    constexpr pointer get() { return m_ptr; }

  private:
    pointer m_ptr{nullptr};
    shared_count m_counter{};
};

} // namespace nstd
