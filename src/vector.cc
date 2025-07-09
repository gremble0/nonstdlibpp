#include "vector.hh"
#include <utility>

namespace nstd {

template <typename T, typename Allocator>
template <typename... Args>
constexpr void vector<T, Allocator>::emplace_back(Args... args) {
    value_type x{std::forward(args...)};
    // TODO(gremble0): implement
}

} // namespace nstd
