#include <iostream>
#include <nlohmann/json.hpp>

#include "melon/piece.hpp"
#include "melon/standard_pieces.hpp"

using json = nlohmann::json;

int main() {
  std::vector<int> items = {0, 1, 2};
  // items.at(0);
  Piece p{'k'};
  p.move();

  json bishop = json::parse(melon::standard_pieces::B);
  std::cout << bishop << '\n';
}
