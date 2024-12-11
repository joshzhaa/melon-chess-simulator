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
  void move() noexcept { moved_ = true; }

  enum class MatrixType : byte { ATTACK, MOVE };

  // this Piece is at (*board)[y, x]
  struct Place {
    math::Vector<int> xy;
    const math::Matrix<Piece>* board;  // Place is not user-facing, so nullptr only if programmer error
  };

  [[nodiscard]] auto matrix(
    MatrixType type,  // flag to enable a capture for each geometry
    Place place
  ) const noexcept -> math::Matrix<byte>;
};

}  // namespace melon

#endif  // MELON_PIECE_HPP
