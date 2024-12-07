#include <format>
#include <nlohmann/json.hpp>
#include <print>

#include "melon/cli/text_io.hpp"
#include "melon/game.hpp"

using namespace melon;
using json = nlohmann::json;  // NOLINT(misc-include-cleaner)

int main() {
  Game game;
  std::println("{}", text_io::serialize(game.board(), true));
}
