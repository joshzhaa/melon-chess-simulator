#include <format>
#include <print>

#include "melon/cli/text_io.hpp"
#include "melon/game.hpp"

using namespace melon;

int main() {
  Game game;
  std::println("{}", text_io::serialize(game.board(), true));
}
