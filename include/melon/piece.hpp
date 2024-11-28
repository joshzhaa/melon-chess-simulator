#ifndef MELON_PIECE_H_
#define MELON_PIECE_H_

#include "melon/math/matrix.hpp"

// clang-format off
// unsigned char is so much more ergonomic than std::byte
namespace { using byte = unsigned char; }
// clang-format on

namespace melon {

/*
 * 0: K
 * 1: Q
 * 2: R
 * 3: B
 * 4: N
 * 5: P
 * 6: Empty
 */
class Piece {
  byte id_;     // imposes a max number of distinguishable pieces: 256
  byte team_;   // imposes a max number of distinguishable teams: 256
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
