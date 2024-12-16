#include "melon/piece.hpp"

#include <algorithm>
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

// @@@ Core Piece Logic @@@
[[nodiscard]] inline byte& mask_at(math::Matrix<byte>& mask, const math::Vector<int>& pos) noexcept {
  assert(mask.at(pos.y, pos.x));  // programmer must bounds check this function
  auto xu = static_cast<std::size_t>(pos.x);
  auto yu = static_cast<std::size_t>(pos.y);
  return mask[yu, xu];
}

inline void assert_consistency(const Piece::Place& pos) noexcept {
#ifndef NDEBUG
  assert(pos.board);
  assert(pos.move_history);
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
void rotate(math::Vector<int>& v, const math::Vector<int>& facing) noexcept {
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

[[nodiscard]] inline auto geometry(byte id, Piece::MatrixType type) noexcept -> const Geometry& {
  const Traits& traits = Traits::db()[id];
  // get geometry for attacking if attacks != moves (singalled by nonempty Geometry)
  return type == Piece::MatrixType::ATTACK and traits.attacks.size() > 0 ? traits.attacks : traits.moves;
}

[[nodiscard]] inline auto facing(byte team) noexcept -> const math::Vector<int>& { return Team::db()[team].facing; }

// modifies square for attack
void mark_point(math::Vector<int>& square, math::Matrix<byte>& mask, const math::Vector<int>& orientation, const Piece::Place& place) noexcept {
  square += orientation;
  if (  // if piece at square is empty (id == 0)
    auto piece = place.board->at(square.y, square.x);
    piece and piece->id() == 0
  ) {
    mask_at(mask, square) = True;
  }
}

// modifies square for attack
void mark_ray(math::Vector<int>& square, math::Matrix<byte>& mask, const math::Vector<int>& orientation, const Piece::Place& place) noexcept {
  std::optional<Piece> piece;
  do {
    mask_at(mask, square) = True;
    square += orientation;
    piece = place.board->at(square.y, square.x);
  } while (piece and piece->id() == 0);
  mask_at(mask, place.xy) = False;  // noop moves are not legal
}

inline void attack(const math::Vector<int>& square, math::Matrix<byte>& mask, byte team, const Piece::Place& place) noexcept {
  if (  // if can capture, mark one more square in the right direction
    auto piece = place.board->at(square.y, square.x);
    piece and piece->team() != team
  ) {
    mask_at(mask, square) = True;
  }
}

// @@@ Actions @@@
void en_passant(math::Matrix<byte>& mask, Piece::MatrixType type, const Piece::Place& place) noexcept {
  if (type == Piece::MatrixType::ATTACK) return;
  const auto& board = *place.board;
  const auto& move_history = *place.move_history;
  const auto pawn = board.at(place.xy.y, place.xy.x);

  auto up = facing(pawn->team());
  auto left = up;
  rotate(left, {.x = -1, .y = 0});
  auto right = up;
  rotate(right, {.x = 1, .y = 0});

  auto recent_double_step = [&board, &move_history](const math::Vector<int>& pos) {
    auto piece = board.at(pos.y, pos.x);
    if (not piece) return false;
    const auto& effects = Traits::db()[piece->id()].effects;
    // if has en_passant effect, then it's a "pawn"
    if (not std::ranges::contains(effects, Effect::EN_PASSANT)) return false;

    if (move_history.empty()) return false;
    const auto& [from, to] = move_history.back();
    if (to != pos) return false;
    auto displacement = to - from;
    auto square_dist = (displacement.x * displacement.x) + (displacement.y * displacement.y);  // probably no overflow
    return square_dist == 4;
  };

  if (recent_double_step(place.xy + left)) {
    mask_at(mask, place.xy + left + up) = static_cast<byte>(Action::EN_PASSANT);
  } else if (recent_double_step(place.xy + right)) {  // impossible for both ifs to trigger
    mask_at(mask, place.xy + right + up) = static_cast<byte>(Action::EN_PASSANT);
  }
}

void castle(math::Matrix<byte>& mask, Piece::MatrixType type, const Piece::Place& place) noexcept {
  // TODO: consider json action specification
  // something like condition, move
  // its difficult because en passant has a really complicated condition
  if (type == Piece::MatrixType::ATTACK) return;
  const auto& board = *place.board;
  const auto king = board.at(place.xy.y, place.xy.x);
  assert(king);
  if (king->moved()) return;
  auto find_rook = [&board, &place](const math::Vector<int>& offset) -> std::optional<math::Vector<int>> {
    math::Vector<int> origin = place.xy;  // copy
    auto piece = board.at(origin.y, origin.x);
    while (piece) {
      const auto& effects = Traits::db()[piece->id()].effects;
      // if piece has the CASTLE effect (i.e. is a rook)
      if (std::ranges::contains(effects, Effect::CASTLE)) return origin;
      origin += offset;
      piece = board.at(origin.y, origin.x);
    }
    return std::nullopt;
  };
  auto left = facing(king->team());  // initially up
  rotate(left, {.x = -1, .y = 0});   // now left
  auto left_rook = find_rook(left);

  auto right = facing(king->team());  // initially up
  rotate(right, {.x = 1, .y = 0});    // now right
  auto right_rook = find_rook(right);

  auto mask_if_castle = [&mask, &board, &place](const math::Vector<int>& rook_pos, const math::Vector<int>& offset) {
    auto rook = board.at(rook_pos.y, rook_pos.x);
    assert(rook);
    if (rook->moved()) return;
    const math::Vector<int> two_spaces = place.xy + offset + offset;
    const auto target = board.at(two_spaces.y, two_spaces.x);
    if (not target or not target->empty()) return;
    mask_at(mask, two_spaces) = static_cast<byte>(Action::CASTLE);
  };
  if (left_rook) mask_if_castle(*left_rook, left);
  if (right_rook) mask_if_castle(*right_rook, right);
}

void double_step(math::Matrix<byte>& mask, Piece::MatrixType type, const Piece::Place& place) noexcept {
  if (type == Piece::MatrixType::ATTACK) return;
  const auto& board = *place.board;
  const auto pawn = board.at(place.xy.y, place.xy.x);
  assert(pawn);
  if (pawn->moved()) return;
  // if both squares ahead are empty then double move is possible
  math::Vector<int> const one_ahead = place.xy + facing(pawn->team());
  if (auto piece = board.at(one_ahead.y, one_ahead.x); not piece or not piece->empty()) return;
  math::Vector<int> const two_ahead = one_ahead + facing(pawn->team());
  if (auto piece = board.at(two_ahead.y, two_ahead.x); not piece or not piece->empty()) return;
  // preconditions for double move are guaranteed satisfied
  mask_at(mask, two_ahead) = static_cast<byte>(Action::DOUBLE_STEP);
}

void exec_actions(math::Matrix<byte>& mask, Piece::MatrixType type, const Piece::Place& place) noexcept {
  const auto& piece = place.board->at(place.xy.y, place.xy.x);
  assert(piece);
  for (const auto& action : Traits::db()[piece->id()].actions) {
    switch (action) {
      case Action::EN_PASSANT:
        en_passant(mask, type, place);
        break;
      case Action::CASTLE:
        castle(mask, type, place);
        break;
      case Action::DOUBLE_STEP:
        double_step(mask, type, place);
        break;
    }
  }
}

}  // namespace

namespace melon {

[[nodiscard]] auto Piece::matrix(
  MatrixType type,  // flag to enable a capture for each geometry
  const Place& place
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
  exec_actions(mask, type, place);
  return mask;
}

}  // namespace melon
