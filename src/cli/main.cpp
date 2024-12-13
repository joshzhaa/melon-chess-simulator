#include <iostream>
#include <ostream>
#include <ranges>

#include "melon/cli/text_io.hpp"
#include "melon/game.hpp"

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

int main() {  // NOLINT(bugprone-exception-escape)
  Game game;
  std::cout << text_io::serialize(game.board(), true) << '\n';
  game.touch({3, 1});
  game.touch({3, 2});
  std::cout << text_io::serialize(game.board(), true) << '\n';
  game.touch({3, 1});
  game.touch({3, 2});

  const auto even = [](int i){ return i % 2 == 0; };
  const auto square = [](int i){ return i * i; };
  const auto ints = {1, 2, 3 ,4 ,5 ,6, 7, 8};
  for (int i : ints | std::views::filter(even) | std::views::transform(square)) {
    std::cout << i << '\n';
  }

  const std::vector<math::Vector<int>> inputs = {
    {4, 1},
    {4, 2},
    {5, 1},
    {5, 2},
    {6, 1},
    {6, 2},
  };
  game.play(inputs);
  std::cout << text_io::serialize(game.board(), true) << '\n';
}
