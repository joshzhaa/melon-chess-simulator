#include "melon/piece.hpp"

#include <cassert>
#include <cstddef>
#include <expected>
#include <optional>
#include <string_view>

#include "melon/math/matrix.hpp"
#include "melon/math/vector.hpp"
#include "melon/traits.hpp"
#include "melon/util.hpp"

namespace {

using namespace melon;

byte& mask_at(math::Matrix<byte>& mask, math::Vector<int> pos) {
  assert(mask.at(pos.y, pos.x));  // programmer must bounds check this
  return mask[ // return a reference to mask
    static_cast<std::size_t>(pos.y),  // xy == ji
    static_cast<std::size_t>(pos.x)
  ];
}

}  // namespace

namespace melon {

[[nodiscard]] auto Piece::matrix(
  MatrixType type,  // flag to enable a capture for each geometry
  Position pos
) const noexcept -> std::expected<math::Matrix<byte>, std::string_view> {
  using namespace std::literals;
  if (pos.xy.x < 0 or pos.xy.y < 0) return std::unexpected("Piece::matrix: expected nonnegative origin coordinates"sv);

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
        do {  // NOLINT(cppcoreguidelines-avoid-do-while), while requires violating DRY
          square += orientation;
          mask_at(mask, square) = True;
          piece = pos.board->at(square.y, square.x);
        } while (piece and piece->id() == 0);
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
