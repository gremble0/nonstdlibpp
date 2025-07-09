#include "vector.hh"

#include <memory>

namespace nstd {

template <typename T, typename Allocator> void vector<T, Allocator>::reserve(size_type size) {
    if (size <= capacity()) {
        return;
    }

    auto *new_data = m_allocator.allocate(size);

    std::uninitialized_move(begin(), end(), new_data);
    std::destroy(begin(), end());
    if (m_data) {
        m_allocator.deallocate(m_data, capacity());
    }

    m_data = new_data;
    m_capacity = size;
}

} // namespace nstd
