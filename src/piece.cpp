#include "melon/piece.hpp"

#include "melon/math/matrix.hpp"
#include "melon/math/vector.hpp"
#include "melon/traits.hpp"

namespace melon {

auto Piece::move_matrix(math::Vector<int> origin, const math::Matrix<Piece>& board) const noexcept -> math::Matrix<byte> {}

auto Piece::attack_matrix(math::Vector<int> origin, const math::Matrix<Piece>& board) const noexcept -> math::Matrix<byte> {
  auto [m, n] = board.shape();
  math::Matrix<byte> attack_matrix{m, n, 0};
  const auto& traits = Traits::db()[id()];
  return attack_matrix;
}

}  // namespace melon
