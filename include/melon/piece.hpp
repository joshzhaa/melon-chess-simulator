#ifndef MELON_PIECE_H_
#define MELON_PIECE_H_

#include <expected>
#include <string_view>

#include "melon/math/matrix.hpp"
#include "melon/math/vector.hpp"
#include "melon/util.hpp"

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

  [[nodiscard]] auto move_matrix(math::Vector<int> origin, const math::Matrix<Piece>& board) const noexcept
    -> std::expected<math::Matrix<byte>, std::string_view>;
  [[nodiscard]] auto attack_matrix(math::Vector<int> origin, const math::Matrix<Piece>& board) const noexcept
    -> std::expected<math::Matrix<byte>, std::string_view>;
};

}  // namespace melon

#endif  // MELON_PIECE_H_
