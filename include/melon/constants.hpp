#ifndef MELON_CONSTANTS_HPP_
#define MELON_CONSTANTS_HPP_

#include <cstddef>
#include <string_view>

#include "byte.hpp"

namespace melon::constants {

template <typename T>
consteval std::size_t maxof() {
  // 8U is not a magic number, everyone knows it's the number of bits in a byte
  return (1U << (sizeof(T) * 8U)) - 1;  // NOLINT(*-magic-numbers)
}

constexpr std::size_t MAX_PIECES = maxof<byte>();
constexpr std::size_t MAX_TEAMS = maxof<byte>();

// json strings for standard chess piece traits
constexpr std::string_view EMPTY_TRAITS = R"(
{
  "moves": [],
  "attacks": [],
  "actions": [],
  "effects": []
}
)";

constexpr std::string_view KING_TRAITS = R"(
{
  "moves": [
    { "orientation": [ 1, 0 ], "shape": "point" },
    { "orientation": [ 1, -1 ], "shape": "point" },
    { "orientation": [ 0, -1 ], "shape": "point" },
    { "orientation": [ -1, -1 ], "shape": "point" },
    { "orientation": [ -1, 0 ], "shape": "point" },
    { "orientation": [ -1, 1 ], "shape": "point" },
    { "orientation": [ 0, 1 ], "shape": "point" },
    { "orientation": [ 1, 1 ], "shape": "point" }
  ],
  "attacks": [],
  "actions": ["castle"],
  "effects": ["check", "checkmate"]
}
)";

constexpr std::string_view QUEEN_TRAITS = R"(
{
  "moves": [
    { "orientation": [ 1, 0 ], "shape": "ray" },
    { "orientation": [ 1, -1 ], "shape": "ray" },
    { "orientation": [ 0, -1 ], "shape": "ray" },
    { "orientation": [ -1, -1 ], "shape": "ray" },
    { "orientation": [ -1, 0 ], "shape": "ray" },
    { "orientation": [ -1, 1 ], "shape": "ray" },
    { "orientation": [ 0, 1 ], "shape": "ray" },
    { "orientation": [ 1, 1 ], "shape": "ray" }
  ],
  "attacks": [],
  "actions": [],
  "effects": []
}
)";

constexpr std::string_view ROOK_TRAITS = R"(
{
  "moves": [
    { "orientation": [ 1, 0 ], "shape": "ray" },
    { "orientation": [ 0, -1 ], "shape": "ray" },
    { "orientation": [ -1, 0 ], "shape": "ray" },
    { "orientation": [ 0, 1 ], "shape": "ray" }
  ],
  "attacks": [],
  "actions": [],
  "effects": ["castle"]
}
)";

constexpr std::string_view BISHOP_TRAITS = R"(
{
  "moves": [
    { "orientation": [ 1, 1 ], "shape": "ray" },
    { "orientation": [ 1, -1 ], "shape": "ray" },
    { "orientation": [ -1, -1 ], "shape": "ray" },
    { "orientation": [ -1, 1 ], "shape": "ray" }
  ],
  "attacks": [],
  "actions": [],
  "effects": []
}
)";

constexpr std::string_view KNIGHT_TRAITS = R"(
{
  "moves": [
    { "orientation": [ 1, 2 ], "shape": "point" },
    { "orientation": [ 2, 1 ], "shape": "point" },
    { "orientation": [ 2, -1 ], "shape": "point" },
    { "orientation": [ 1, -2 ], "shape": "point" },
    { "orientation": [ -1, -2 ], "shape": "point" },
    { "orientation": [ -2, -1 ], "shape": "point" },
    { "orientation": [ -2, 1 ], "shape": "point" },
    { "orientation": [ -1, -2 ], "shape": "point" }
  ],
  "attacks": [],
  "actions": [],
  "effects": []
}
)";

constexpr std::string_view PAWN_TRAITS = R"(
{
  "moves": [
    { "orientation": [ 0, 1 ], "shape": "point" }
  ],
  "attacks": [
    { "orientation": [ 1, 1 ], "shape": "point" },
    { "orientation": [ -1, 1 ], "shape": "point" }
  ],
  "actions": ["en_passant", "double_step"],
  "effects": ["en_passant", "promotion"]
}
)";

// for some reason clangd@18.1.3 believes this line won't compile
constexpr std::array STANDARD_TRAITS = {EMPTY_TRAITS, KING_TRAITS, QUEEN_TRAITS, ROOK_TRAITS, BISHOP_TRAITS, KNIGHT_TRAITS, PAWN_TRAITS};
// constexpr std::string_view STANDARD_TRAITS[] = {EMPTY_TRAITS, KING_TRAITS, QUEEN_TRAITS, ROOK_TRAITS, BISHOP_TRAITS, KNIGHT_TRAITS, PAWN_TRAITS};

}  // namespace melon::constants

#endif
