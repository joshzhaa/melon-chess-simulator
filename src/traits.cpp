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

using namespace melon;
using namespace std::literals;

auto default_traits() noexcept -> std::array<Traits, constants::MAX_PIECES> {
  std::array<Traits, constants::MAX_PIECES> traits{};  // force default initialization for whole array
  for (std::size_t piece_id = 0; piece_id < constants::STANDARD_TRAITS.size(); ++piece_id) {
    const json obj = json::parse(constants::STANDARD_TRAITS[piece_id], nullptr, false);
    assert(not obj.is_discarded());  // for STANDARD_TRAITS, assert failure can only be due to programmer error
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

bool Traits::load_traits(byte id, std::string&& data) noexcept {
  const json obj = json::parse(std::move(data), nullptr, false);
  if (obj.is_discarded()) return false;
  // potential race condition, doesn't matter for single threaded
  db()[id] = obj;
  return true;
}

}  // namespace melon
