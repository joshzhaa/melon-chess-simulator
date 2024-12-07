#include "melon/piece.hpp"

#include <cassert>
#include <cstddef>
#include <optional>

#include "melon/math/matrix.hpp"
#include "melon/math/vector.hpp"
#include "melon/traits.hpp"
#include "melon/util.hpp"

namespace {

using namespace melon;

byte& mask_at(math::Matrix<byte>& mask, math::Vector<int> pos) noexcept {
  assert(mask.at(pos.y, pos.x));  // programmer must bounds check this function
  return mask[ // return a reference to mask
    static_cast<std::size_t>(pos.y),  // xy == ji
    static_cast<std::size_t>(pos.x)
  ];
}

void assert_consistency(const Piece::Position& pos) noexcept {
#ifndef NDEBUG
  assert(pos.board);
  assert(pos.xy.x >= 0 and pos.xy.y >= 0);
  auto [m, n]= pos.board->shape();
  auto xu = static_cast<std::size_t>(pos.xy.x);
  auto yu = static_cast<std::size_t>(pos.xy.y);
  assert(xu < n and yu < m);
#endif
}

}  // namespace

namespace melon {

[[nodiscard]] auto Piece::matrix(
  MatrixType type,  // flag to enable a capture for each geometry
  Position pos
) const noexcept -> math::Matrix<byte> {
  using namespace std::literals;
  assert_consistency(pos);
  math::Matrix<byte> mask{pos.board->shape(), False};
  // Geometry iterator produces a temporary, must bind by value
  for (const auto [shape, orientation] : Traits::db()[id()].moves) {
    math::Vector<int> square{pos.xy};
    // handle each possible shape differently (only moves)
    switch (shape) {
      case Shape::POINT:
        square += orientation;
        if (  // if piece at square is empty (id == 0)
          auto piece = pos.board->at(square.y, square.x);
          piece and piece->id() == 0
        ) {
          mask_at(mask, pos.xy) = True;
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
      type == MatrixType::ATTACK and piece->team() != this->team()
    ) {
      mask_at(mask, square) = True;
    }
  }
  return mask;
}

}  // namespace melon
