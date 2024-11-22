#include <format>
#include <iostream>
#include <nlohmann/json.hpp>
#include "chessmelon/piece.hpp"
#include "chessmelon/standard_pieces.h"

using json = nlohmann::json;

int main() {
  std::cout << "Hello world\n";
  std::size_t n = 10;
  for (std::size_t i = 0; i < n; ++i)
    std::cout << std::format("numberinbetween{}letters", i) << '\n';
  Piece p{'k'};
  p.move();
  
  json bishop = json::parse(melon::standard_pieces::B);
  std::cout << bishop << '\n';
}
