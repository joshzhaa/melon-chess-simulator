#ifndef MELON_CONSTANTS_H_
#define MELON_CONSTANTS_H_

#include <cstddef>

namespace melon::constants {

template <typename T>
consteval std::size_t maxof() {
  return (1 << (sizeof(T) * 8)) - 1;  // NOLINT
}

constexpr std::size_t MAX_PIECES = maxof<unsigned char>();
constexpr std::size_t MAX_TEAMS = maxof<unsigned char>();

}  // namespace melon::constants

#endif
