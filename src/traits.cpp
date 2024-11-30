#include "melon/traits.hpp"

#include <array>
#include <cassert>
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>
#include <utility>

#include "melon/constants.hpp"

using json = nlohmann::json;  // NOLINT(misc-include-cleaner)

namespace {

// define json strings for standard chess piece traits
char const* const B = R"(
{
  "id": 4,
  "moves": [{ "shape": "line", "vector": [[1, 1], [1, -1], [-1, -1], [-1, 1]] }],
  "attacks": [{ "shape": "line", "vector": [[1, 1], [1, -1], [-1, -1], [-1, 1]] }]
}
)";

char const* const P = R"(
{
  "id": 6,
  "moves": [{ "shape": "point", "vector": [[0, 1]] }],
  "attacks": [{ "shape": "point", "vector": [[1, 1], [-1, 1]] }],
  "actions": ["enpassant", "double_step"],
  "effects": ["enpassant", "promotion"]
}
)";

char const* const K = R"(
{
  "id": 1,
  "moves": [{ "shape": "point", "vector": [[0, 1], [1, -1], [0, -1], [-1, -1], [-1, 0], [-1, 1], [0, 1], [1, 1]] }],
  "attacks": [{ "shape": "point", "vector": [[0, 1], [1, -1], [0, -1], [-1, -1], [-1, 0], [-1, 1], [0, 1], [1, 1]] }],
  "actions": ["castle"],
  "effects": ["check", "checkmate"]
}
)";

// TODO: figure out how to construct Traits from json obj
melon::Traits make_traits(const json& json) {}

auto default_traits() noexcept -> std::array<melon::Traits, melon::constants::MAX_PIECES> {
  using melon::Traits;
  using melon::constants::MAX_PIECES;

  static const std::array strings = {B, P, K};

  std::array<Traits, MAX_PIECES> traits{};  // force default initialization for whole array
  for (const auto& string : strings) {
    json const obj = json::parse(string, nullptr, false);
    assert(not obj.is_discarded());  // assert failure can only be due to programmer error
    std::cout << obj << '\n';
    traits[obj["id"]] = make_traits(obj);
  }
  return traits;
}

}  // namespace

namespace melon {

auto Traits::db() noexcept -> std::array<Traits, constants::MAX_PIECES>& {
  static std::array<Traits, constants::MAX_PIECES> loaded_pieces{default_traits()};
  return loaded_pieces;
}

bool Traits::load_traits(unsigned char id, std::string&& data) noexcept {
  json const obj = json::parse(std::move(data), nullptr, false);
  if (obj.is_discarded()) return false;
  // potential race condition, doesn't matter for single threaded
  Traits::db()[id] = make_traits(obj);
  return true;
}

}  // namespace melon
