#include "melon/piece.hpp"

#include <array>
#include <cassert>
#include <cstddef>
#include <optional>

#include "melon/byte.hpp"
#include "melon/math/matrix.hpp"
#include "melon/math/vector.hpp"
#include "melon/traits.hpp"

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

[[nodiscard]] inline auto geometry(byte id, Piece::MatrixType type) -> const Geometry& {
  const Traits& traits = Traits::db()[id];
  // get geometry for attacking if attacks != moves (singalled by nonempty Geometry)
  return type == Piece::MatrixType::ATTACK and traits.attacks.size() > 0 ? traits.attacks : traits.moves;
}

[[nodiscard]] inline auto facing(byte team) -> const math::Vector<int>& { return Team::db()[team].facing; }

// modifies square for attack
void mark_point(math::Vector<int>& square, math::Matrix<byte>& mask, const math::Vector<int>& orientation, const Piece::Place& place) {
  square += orientation;
  if (  // if piece at square is empty (id == 0)
    auto piece = place.board->at(square.y, square.x);
    piece and piece->id() == 0
  ) {
    mask_at(mask, square) = True;
  }
}

// modifies square for attack
void mark_ray(math::Vector<int>& square, math::Matrix<byte>& mask, const math::Vector<int>& orientation, const Piece::Place& place) {
  std::optional<Piece> piece;
  do {
    mask_at(mask, square) = True;
    square += orientation;
    piece = place.board->at(square.y, square.x);
  } while (piece and piece->id() == 0);
  mask_at(mask, place.xy) = False;  // noop moves are not legal
}

void attack(const math::Vector<int>& square, math::Matrix<byte>& mask, byte team, const Piece::Place& place) {
  if (  // if can capture, mark one more square in the right direction
    auto piece = place.board->at(square.y, square.x);
    piece and piece->team() != team
  ) {
    mask_at(mask, square) = True;
  }
}

}  // namespace

namespace melon {

[[nodiscard]] auto Piece::matrix(
  MatrixType type,  // flag to enable a capture for each geometry
  Place place
) const noexcept -> math::Matrix<byte> {
  using namespace std::literals;
  assert_consistency(place);

  math::Matrix<byte> mask{place.board->shape(), False};
  // Geometry iterator produces a temporary, must bind by value
  for (auto [shape, orientation] : geometry(id(), type)) {
    math::Vector<int> square{place.xy};  // copy
    rotate(orientation, facing(team()));
    // handle only moves for each shape
    switch (shape) {
      case Shape::POINT:
        mark_point(square, mask, orientation, place);
        break;
      case Shape::RAY:
        mark_ray(square, mask, orientation, place);
        break;
    }
    if (type == MatrixType::ATTACK) attack(square, mask, team(), place);
  }
  return mask;
}

}  // namespace melon
