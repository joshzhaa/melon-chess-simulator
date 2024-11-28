#include "melon/piece.hpp"

namespace melon {

auto Piece::attack(const math::Matrix<Piece>& board) const noexcept -> math::Matrix<bool> {
  auto [m, n] = board.shape();
  math::Matrix<bool> attack_matrix{m, n, false};
  return attack_matrix;
}

}  // namespace melon
