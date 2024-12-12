#include <format>
#include <print>

#include "melon/cli/text_io.hpp"
#include "melon/game.hpp"

using namespace melon;

int main() {  // NOLINT(bugprone-exception-escape)
  Game game;
  std::println("{}", text_io::serialize(game.board(), true));
  game.touch({3, 1});
  game.touch({3, 2});
  std::println("{}", text_io::serialize(game.board(), true));
}
