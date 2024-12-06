#include <cstddef>
#include <format>
#include <iostream>
#include <nlohmann/json.hpp>

#include "melon/cli/text_io.hpp"
#include "melon/constants.hpp"
#include "melon/game.hpp"
#include "melon/traits.hpp"
#include "melon/traits_json.hpp"

using namespace melon;
using json = nlohmann::json;  // NOLINT(misc-include-cleaner)

namespace {

void list_pieces() {
  for (std::size_t piece_id = 0; piece_id < constants::STANDARD_TRAITS.size(); ++piece_id) {
    std::cout << std::format("piece_id={}", piece_id) << '\n';
    const json json = Traits::db()[piece_id];
    std::cout << json << '\n';
  }
}

}  // namespace 

int main() {
  Game game;
  std::cout << text_io::serialize(game.board(), true);
}
