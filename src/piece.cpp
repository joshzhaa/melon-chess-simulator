#include "melon/piece.hpp"

#include <optional>

#include "melon/math/matrix.hpp"
#include "melon/math/vector.hpp"
#include "melon/traits.hpp"

namespace {

using namespace melon;
using namespace std::literals;

// bounds checked board indexing
// TODO: add stopping function
auto board_at(math::Vector<int> pos, const math::Matrix<Piece>& board) -> std::optional<Piece> {
  auto [m, n] = board.shape();
  auto [x, y] = pos;
  if (x < 0 or y < 0) return std::nullopt;
  // x and y are now guaranteed nonnegative
  auto xu = static_cast<std::size_t>(x);
  auto yu = static_cast<std::size_t>(y);
  if (xu > n or yu > m) return std::nullopt;
  return board[xu, yu];
}

byte& mask_at(math::Matrix<byte>& mask, math::Vector<int> pos) {
  assert(pos.x > 0 && pos.y > 0);  // user must check this is the case
  auto x = static_cast<std::size_t>(pos.x);
  auto y = static_cast<std::size_t>(pos.y);
  return mask[x, y];
}

auto apply_move_geometry(
  math::Matrix<byte>& mask,
  const math::Matrix<Piece>& board,
  math::Vector<int> origin,
  Shape shape,
  const math::Vector<int>& orientation
) noexcept -> math::Vector<int> {
  switch (shape) {
    case Shape::POINT: {
      origin = origin + orientation;
      auto piece = board_at(origin, board);
      if (piece && piece->id() == 0) mask_at(mask, origin) = True;
      break;
    }
    case Shape::RAY: {
      origin = origin + orientation;
      auto piece = board_at(origin, board);
      while (piece && piece->id() == 0) {
        mask_at(mask, origin) = True;
        origin = origin + orientation;
        piece = board_at(origin, board);
      }
      break;
    }
  }
  return origin;
}

}  // namespace

namespace melon {

auto Piece::move_matrix(
  math::Vector<int> origin,         // position of this piece
  const math::Matrix<Piece>& board  // board that includes this piece
) const noexcept -> std::expected<math::Matrix<byte>, std::string_view> {
  if (origin.x < 0 or origin.y < 0) return std::unexpected("move_matrix: expected nonnegative origin coordinates"sv);
  math::Matrix<byte> mask{board.shape(), False};
  for (auto [shape, orientation] : Traits::db()[id()].moves) {
    apply_move_geometry(mask, board, origin, shape, orientation);
  }
  return mask;
}

/* TODO: it might be possible to unify this function with move_matrix, probably with a different name
 * also need to do some testing
 * also, the behavior isn't correct right now
 */ 
auto Piece::attack_matrix(
  math::Vector<int> origin,         // position of this piece
  const math::Matrix<Piece>& board  // board that includes this piece
) const noexcept -> std::expected<math::Matrix<byte>, std::string_view> {
  if (origin.x < 0 or origin.y < 0) return std::unexpected("move_matrix: expected nonnegative origin coordinates"sv);
  math::Matrix<byte> mask{board.shape(), False};
  for (auto [shape, orientation] : Traits::db()[id()].attacks) {
    auto marker = apply_move_geometry(mask, board, origin, shape, orientation);
    if (board_at(marker, board)->id() != this->id()) mask_at(mask, marker) = True;
  }
  return mask;
}

}  // namespace melon
