#include "melon/piece.hpp"

#include <array>
#include <cassert>
#include <cstddef>
#include <optional>

#include "melon/math/matrix.hpp"
#include "melon/math/vector.hpp"
#include "melon/traits.hpp"
#include "melon/util.hpp"

namespace {

using namespace melon;

[[nodiscard]] byte& mask_at(math::Matrix<byte>& mask, math::Vector<int> pos) noexcept {
  assert(mask.at(pos.y, pos.x));  // programmer must bounds check this function
  auto xu = static_cast<std::size_t>(pos.x);
  auto yu = static_cast<std::size_t>(pos.y);
  return mask[yu, xu];
}

void assert_consistency(const Piece::Place& pos) noexcept {
#ifndef NDEBUG
  assert(pos.board);
  assert(pos.xy.x >= 0 and pos.xy.y >= 0);
  auto [m, n] = pos.board->shape();
  auto xu = static_cast<std::size_t>(pos.xy.x);
  auto yu = static_cast<std::size_t>(pos.xy.y);
  assert(xu < n and yu < m);
#endif
}

/*
 * rotates a Vector v from "up" to "facing".
 * "facing" must be a unit vector.
 * uses a rotation matrix that transforms "up" to "facing"
 */
void rotate(math::Vector<int>& v, const math::Vector<int>& facing) {
  constexpr math::Vector<int> up{.x = 0, .y = 1};
  const auto [x1, y1] = up;
  const auto [x2, y2] = facing;
  const std::array<std::array<int, 2>, 2> matrix{
    {
     {(x1 * x2) + (y1 * y2), (-x1 * y2) + (x2 * y1)},
     {(x1 * y2) - (x2 * y1), (x1 * x2) + (y1 * y2)},
     }
  };
  // 2D only matrix multiplication
  v = {
    .x = (matrix[0][0] * v.x) + (matrix[0][1] * v.y),
    .y = (matrix[1][0] * v.x) + (matrix[1][1] * v.y),
  };
}

// whether Traits indicates that piece moves in same way it attacks
[[nodiscard]] inline bool same_attack_move(const Traits& traits) { return traits.attacks.size() == 0; }

}  // namespace

namespace melon {

[[nodiscard]] auto Piece::matrix(
  MatrixType type,  // flag to enable a capture for each geometry
  Place pos
) const noexcept -> math::Matrix<byte> {
  using namespace std::literals;
  assert_consistency(pos);

  math::Matrix<byte> mask{pos.board->shape(), False};
  const Traits& traits = Traits::db()[id()];
  const math::Vector facing = Team::db()[team()].facing;
  // get geometry for attacking if attacks != moves
  const Geometry& geometry = type == MatrixType::ATTACK and not same_attack_move(traits) ? traits.attacks : traits.moves;
  // Geometry iterator produces a temporary, must bind by value
  for (auto [shape, orientation] : geometry) {
    math::Vector<int> square{pos.xy};  // copy
    rotate(orientation, facing);
    // handle each possible shape differently (only moves)
    switch (shape) {
      case Shape::POINT:
        square += orientation;
        if (  // if piece at square is empty (id == 0)
          auto piece = pos.board->at(square.y, square.x);
          piece and piece->id() == 0
        ) {
          mask_at(mask, square) = True;
        }
        break;
      case Shape::RAY:
        std::optional<Piece> piece;
        do {  // NOLINT(cppcoreguidelines-avoid-do-while)
          mask_at(mask, square) = True;
          square += orientation;
          piece = pos.board->at(square.y, square.x);
        } while (piece and piece->id() == 0);
        mask_at(mask, pos.xy) = False;  // noop moves are not legal
        break;
    }
    if (  // if captures are enabled, mark one more square in the right direction
      auto piece = pos.board->at(square.y, square.x);
      type == MatrixType::ATTACK and piece and piece->team() != this->team()
    ) {
      mask_at(mask, square) = True;
    }
  }
  return mask;
}

}  // namespace melon
