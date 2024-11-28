#include "melon/traits.hpp"

#include <array>
#include <cassert>
#include <iostream>
#include <nlohmann/json.hpp>

#include "melon/constants.hpp"

using json = nlohmann::json;

// define json strings for standard chess piece traits
namespace {

char const* const B = R"(
{
  "moves": [{ "shape": "line", "vector": [[1, 1], [1, -1], [-1, -1], [-1, 1]] }],
  "attacks": [{ "shape": "line", "vector": [[1, 1], [1, -1], [-1, -1], [-1, 1]] }]
}
)";

char const* const P = R"(
{
  "moves": [{ "shape": "point", "vector": [[0, 1]] }],
  "attacks": [{ "shape": "point", "vector": [[1, 1], [-1, 1]] }],
  "actions": ["enpassant", "double_move"],
  "effects": ["enpassant", "promotion"]
}
)";

char const* const K = R"(
{
  "moves": [{ "shape": "point", "vector": [[0, 1], [1, -1], [0, -1], [-1, -1], [-1, 0], [-1, 1], [0, 1], [1, 1]]}],
  "attacks": [{ "shape": "point", "vector": [[0, 1], [1, -1], [0, -1], [-1, -1], [-1, 0], [-1, 1], [0, 1], [1, 1]]}],
  "actions": ["castle"],
  "effects": ["check", "checkmate"]
}
)";

auto default_traits() -> std::array<melon::Traits, melon::constants::MAX_PIECES> {
  using namespace melon;
  std::array<Traits, constants::MAX_PIECES> traits_array;
  for (const auto string : {B, P, K}) {
    json obj = json::parse(string, nullptr, false);
    assert(not obj.is_discarded());  // assert failure can only be due to programmer error
    std::cout << obj << '\n';
  }
  return traits_array;
}

}  // namespace

namespace melon {

auto Traits::db() -> std::array<Traits, constants::MAX_PIECES>& {
  static std::array<Traits, constants::MAX_PIECES> loaded_pieces{default_traits()};
  return loaded_pieces;
}

// TODO: figure out how to construct Traits from json obj
bool Traits::load_traits(unsigned char id, std::string&& data) {
  json obj = json::parse(std::move(data), nullptr, false);
  if (obj.is_discarded()) return false;
  Traits traits;
  Traits::db()[id] = std::move(traits);  // NRVO doesn't apply so move is probably correct
  return true;
}

}  // namespace melon
