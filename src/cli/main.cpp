#include <format>
#include <fstream>
#include <iostream>
#include <vector>

#include "melon/cli/text_io.hpp"
#include "melon/game.hpp"
#include "melon/math/vector.hpp"

using namespace melon;

// NOLINTBEGIN(*-named-parameter,  readability-convert-member-functions-to-static)
template <typename T>
struct LoudObject{
  LoudObject() { std::cout << "default ctor\n"; }
  LoudObject(const LoudObject&) { std::cout << "copy ctor\n"; }
  LoudObject& operator=(const LoudObject& other) {
    if (this == &other) return *this;
    std::cout << "copy assign\n";
    return *this;
  }
  LoudObject(LoudObject&&) noexcept { std::cout << "move ctor\n"; }
  LoudObject& operator=(LoudObject&&) noexcept {
    std::cout << "move assign\n";
    return *this;
  }
  ~LoudObject() { std::cout << "dtor\n"; }
  bool operator==(const LoudObject&) { return true; }
  T* begin() { return nullptr; }
  T* end() { return nullptr; }
};
// NOLINTEND(*-named-parameter, readability-convert-member-functions-to-static)

namespace {

void console_game(std::ostream& os, std::istream& is) {
  Game game;
  int x = 0;
  int y = 0;
  os << text_io::serialize(game.board(), true) << '\n';
  std::vector<math::Vector<int>> inputs;
  while (is >> x >> y) {
    os << "ply: " << game.ply() << '\n';
    os << "turn: " << static_cast<int>(game.turn()) << '\n';
    inputs.emplace_back(x, y);
    game.touch({.x=x, .y=y});
    if (game.mode() == Game::Mode::SELECT) {
      os << text_io::serialize(game.board(), true) << '\n';
    } else {
      text_io::print(game.move_mask());
    }
  }
  for (const auto [left, right] : inputs) {
    os << std::format("{} {}\n", left, right);
  }
  os << text_io::serialize(game.board(), false) << '\n';
}

} // namespace

int main() {
  // std::ifstream file{"castle.input"};
  // console_game(std::cout,  file);
  console_game(std::cout, std::cin);
}
