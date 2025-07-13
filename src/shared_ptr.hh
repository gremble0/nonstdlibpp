#pragma once

#include <cassert>
#include <cstddef>

namespace nstd {

// TODO(gremble0): deleter template
struct control_block {
    size_t ref_count;
    size_t weak_count;
};

template <typename T> class shared_ptr {
  public:
    using pointer = T *;

    constexpr shared_ptr() noexcept = default;

    constexpr explicit shared_ptr(pointer ptr) : m_ptr(ptr), m_cb(1, 1) {}

    constexpr shared_ptr(const shared_ptr &other) = default;

    constexpr shared_ptr(pointer ptr, control_block *cb) : m_ptr(ptr), m_cb(cb) { assert(m_cb); }

    constexpr ~shared_ptr() noexcept {
        if (m_cb->ref_count <= 1) {
            delete m_ptr;
        } else {
            --m_cb->ref_count;
        }
    }

    constexpr pointer get() { return m_ptr; }

  private:
    pointer m_ptr;
    control_block m_cb;
};

} // namespace nstd
