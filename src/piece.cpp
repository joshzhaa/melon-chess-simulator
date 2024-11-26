#include "melon/piece.hpp"

#include <format>
#include <iostream>

namespace melon {

Piece::Piece(unsigned char id) noexcept : id_(id) { std::cout << std::format("piece id={} created\n", id); }

bool Piece::move() const noexcept {
  std::cout << "moving!";
  return true;
}

}  // namespace melon
