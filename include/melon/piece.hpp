#ifndef MELON_PIECE_HPP
#define MELON_PIECE_HPP

#include "melon/byte.hpp"
#include "melon/math/matrix.hpp"
#include "melon/math/vector.hpp"

namespace melon {

/*
 * A Piece fundamentally is a pair (id, team), e.g. (Pawn, White).
 * Piece also tracks whether it has moved or not, for performance reasons (for K, P, R).
 * Piece's behaviors are stored in Traits, since each id shares the same behaviors
 */
class Piece {
  byte id_;  // imposes a max number of distinguishable pieces, see constants::MAX_PIECES
  byte team_;
  bool moved_{false};  // could be computed but would be inefficient to compute

public:
  explicit Piece(byte id, byte team) noexcept : id_{id}, team_{team} {}
  [[nodiscard]] byte id() const noexcept { return id_; }
  [[nodiscard]] byte team() const noexcept { return team_; }
  [[nodiscard]] bool moved() const noexcept { return moved_; }
  [[nodiscard]] bool empty() const noexcept { return id_ == 0; }
  void move() noexcept { moved_ = true; }
  void destruct() noexcept { *this = Piece{0, 0}; }

  enum class MatrixType : byte { ATTACK, MOVE };

  // defines the context that this Piece exists in
  struct Place {
    math::Vector<int> xy;              // this Piece is at (*board)[y, x]
    const math::Matrix<Piece>* board;  // Place is not user-facing, so nullptr only if programmer error
    const std::vector<std::tuple<math::Vector<int>, math::Vector<int>>>* move_history;  // for en_passant
  };

  /*
   * matrix of board squares this Piece can move to
   * derived from "moves" or "attacks" depending on MatrixType
   * moves and attacks mark squares as 1 (melon::True)
   * squares that this Piece can't move to are marked as 0 (melon::False)
   * special actions mark square with their corresponding melon::Action byte value
   */
  [[nodiscard]] auto matrix(
    MatrixType type,  // flag to enable a capture for each geometry
    Place place
  ) const noexcept -> math::Matrix<byte>;
};

}  // namespace melon

#endif  // MELON_PIECE_HPP
