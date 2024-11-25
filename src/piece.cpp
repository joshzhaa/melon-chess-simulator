#include "melon/piece.hpp"

#include <format>
#include <iostream>

Piece::Piece(char id) : id_(id) { std::cout << std::format("piece id={} created\n", id); }

bool Piece::move() {
  std::cout << "moving!";
  return true;
}
