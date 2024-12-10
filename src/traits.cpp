#include "melon/traits.hpp"

#include <array>
#include <cassert>
#include <cstddef>
#include <nlohmann/json.hpp>
#include <string>
#include <utility>

#include "melon/constants.hpp"
#include "melon/traits_json.hpp"
#include "melon/util.hpp"

namespace {

using namespace melon;
using json = nlohmann::json;  // NOLINT(misc-include-cleaner)

auto default_traits() noexcept -> std::array<Traits, constants::MAX_PIECES> {  // NOLINT(bugprone-exception-escape)
  std::array<Traits, constants::MAX_PIECES> traits{};                          // force default initialization for whole array
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

bool Traits::load_traits(byte id, std::string&& data) noexcept {  // NOLINT(bugprone-exception-escape)
  const json obj = json::parse(std::move(data), nullptr, false);
  if (obj.is_discarded()) return false;
  // potential race condition, doesn't matter for single threaded
  db()[id] = obj;
  return true;
}

// TODO: define more general way to load teams
// currently only supports standard chess rules
auto Team::db() noexcept -> std::array<Team, constants::MAX_TEAMS>& {
  static std::array<Team, constants::MAX_TEAMS> teams{
    Team{{.x = 0, .y = 0}},   // null team owns empty squares
    Team{{.x = 0, .y = 1}},   // white
    Team{{.x = 0, .y = -1}},  // black
  };
  return teams;
}

}  // namespace melon
