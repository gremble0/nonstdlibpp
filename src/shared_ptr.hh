#pragma once

#include <atomic>
#include <cassert>
#include <cstddef>

namespace nstd {

// TODO(gremble0): deleter template, maybe some allocator stuff elsewhere too
class shared_count_base {
  public:
    constexpr void release() noexcept {
        --m_ref_count;
        --m_weak_count;
    }

    constexpr void weak_release() noexcept { --m_weak_count; }

  private:
    // TODO(gremble0): maybe some smarter locking - here we have to lock and unlock twice when strong releasing
    std::atomic<size_t> m_ref_count = 1;
    std::atomic<size_t> m_weak_count = 1;
};

class shared_count {
  private:
    shared_count_base *m_count;
};

template <typename T> class shared_ptr {
  public:
    using pointer = T *;

    constexpr shared_ptr() noexcept = default;

    constexpr explicit shared_ptr(pointer ptr) noexcept : m_ptr(ptr) {}

    constexpr shared_ptr(const shared_ptr &other) = default;

    constexpr ~shared_ptr() noexcept = default;

    constexpr pointer get() { return m_ptr; }

  private:
    pointer m_ptr;
    shared_count m_counter;
};

} // namespace nstd
