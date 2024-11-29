#include "melon/traits.hpp"

#include <array>
#include <cassert>
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>
#include <utility>

#include "melon/constants.hpp"

using json = nlohmann::json;  // NOLINT(misc-include-cleaner)

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

melon::Traits make_traits(const json& json) {}

auto default_traits() noexcept -> std::array<melon::Traits, melon::constants::MAX_PIECES> {
  using namespace melon;
  std::array<Traits, constants::MAX_PIECES> traits_array;
  for (const auto& string : {B, P, K}) {
    json const obj = json::parse(string, nullptr, false);
    assert(not obj.is_discarded());  // assert failure can only be due to programmer error
    std::cout << obj << '\n';
  }
  return traits_array;
}

}  // namespace

namespace melon {

auto Traits::db() noexcept -> std::array<Traits, constants::MAX_PIECES>& {
  static std::array<Traits, constants::MAX_PIECES> loaded_pieces{default_traits()};
  return loaded_pieces;
}

// TODO: figure out how to construct Traits from json obj
bool Traits::load_traits(unsigned char id, std::string&& data) noexcept {
  json const obj = json::parse(std::move(data), nullptr, false);
  if (obj.is_discarded()) return false;
  // potential race condition, doesn't matter for single threaded
  Traits const traits;
  Traits::db()[id] = traits;  // don't move this, b/c aggregate struct
  return true;
}

}  // namespace melon
