#include "melon/traits.hpp"

#include <array>
#include <cassert>
#include <nlohmann/json.hpp>
#include <string>
#include <utility>

#include "melon/constants.hpp"
#include "melon/traits_json.hpp"

using json = nlohmann::json;  // NOLINT(misc-include-cleaner)

namespace {

auto default_traits() noexcept -> std::array<melon::Traits, melon::constants::MAX_PIECES> {
  using melon::Traits, melon::constants::MAX_PIECES, melon::constants::STANDARD_TRAITS;

  std::array<Traits, MAX_PIECES> traits{};  // force default initialization for whole array
  for (std::size_t piece_id = 0; piece_id < STANDARD_TRAITS.size(); ++piece_id) {
    std::cout << piece_id << '\n';
    const json obj = json::parse(STANDARD_TRAITS[piece_id], nullptr, false);
    assert(not obj.is_discarded());  // assert failure can only be due to programmer error
    traits[piece_id] = obj;
  }
  return traits;
}

}  // namespace

namespace melon {

auto Traits::db() noexcept -> std::array<Traits, constants::MAX_PIECES>& {
  static std::array<Traits, constants::MAX_PIECES> loaded_pieces{default_traits()};
  return loaded_pieces;
}

bool Traits::load_traits(std::string&& data) noexcept {
  const json obj = json::parse(std::move(data), nullptr, false);
  if (obj.is_discarded()) return false;
  if (not obj.contains("id")) return false;
  // potential race condition, doesn't matter for single threaded
  db()[obj["id"]] = obj;
  return true;
}

}  // namespace melon
