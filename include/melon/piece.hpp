#ifndef MELON_PIECE_H_
#define MELON_PIECE_H_

#include "melon/math/matrix.hpp"

// clang-format off
// unsigned char is so much more ergonomic than std::byte
namespace { using byte = unsigned char; }  // TODO: move this into some kind of utility header
// clang-format on

namespace melon {

/*
 * A Piece fundamentally is a pair (id, team), e.g. (Pawn, White).
 * Piece also tracks whether it has moved or not, for performance reasons (for K, P, R).
 * Piece's behaviors are stored in Traits, since each id shares the same behaviors
 */
class Piece {
  byte id_;  // imposes a max number of distinguishable pieces, see constants.hpp
  byte team_;
  bool moved_;  // could be computed but would be inefficient to compute

public:
  explicit Piece(byte id, byte team) noexcept : id_{id}, team_{team}, moved_{false} {}
  byte id() const noexcept { return id_; }
  byte team() const noexcept { return team_; }
  bool moved() const noexcept { return moved_; }
  void move() noexcept { moved_ = true; }

  /*
   * returns an "attack matrix" (a matrix of same shape as board that highlights allowed moves)
   * this shows the result of the "moves" and "attacks" fields of the piece json
   */
  auto attack(const math::Matrix<Piece>& board) const noexcept -> math::Matrix<bool>;
};

}  // namespace melon
//
#endif  // MELON_PIECE_H_
