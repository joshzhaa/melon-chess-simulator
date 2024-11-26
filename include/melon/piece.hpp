#ifndef MELON_PIECE_H_
#define MELON_PIECE_H_

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
  unsigned char id_;  // imposes a max number of distinguishable pieces: 256
  bool moved_;        // could be computed but would be inefficient to compute

public:
  explicit Piece(unsigned char id) noexcept;
  bool move() const noexcept;
  unsigned char id() const noexcept { return id_; }
};

}  // namespace melon
//
#endif  // MELON_PIECE_H_
