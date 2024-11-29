#include "melon/piece.hpp"

#include "melon/math/matrix.hpp"
#include "melon/traits.hpp"

namespace melon {

auto Piece::attack(const math::Matrix<Piece>& board) const noexcept -> math::Matrix<bool> {
  auto [m, n] = board.shape();
  math::Matrix<bool> attack_matrix{m, n, false};
  const auto& traits = Traits::db()[id()];
  return attack_matrix;
}

}  // namespace melon
