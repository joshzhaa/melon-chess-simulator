#include "melon/piece.hpp"

#include <optional>

#include "melon/math/matrix.hpp"
#include "melon/math/vector.hpp"
#include "melon/traits.hpp"

namespace {

using namespace melon;
using namespace std::literals;

enum class OutOfBounds {};

// bounds checked board indexing
// TODO: add stopping function
auto board_at(math::Vector<int> pos, const math::Matrix<Piece>& board) -> std::optional<Piece> {
  auto [m, n] = board.shape();
  auto [x, y] = pos;
  if (x < 0 or y < 0) return std::nullopt;
  auto ux = static_cast<std::size_t>(x);
  auto uy = static_cast<std::size_t>(y);
  if (ux > n or uy > m) return std::nullopt;
  return board[ux, uy];
}

}  // namespace

namespace melon {

auto Piece::move_matrix(math::Vector<int> origin, const math::Matrix<Piece>& board) const noexcept
  -> std::expected<math::Matrix<byte>, std::string_view> {
  if (origin.x < 0 or origin.y < 0) return std::unexpected("move_matrix: expected nonnegative origin coordinates");
  auto [m, n] = board.shape();
  math::Matrix<byte> mask{m, n, 0};  // initial entirely false matrix
  const auto& geometry = Traits::db()[id()].moves;
  for (auto [shape, orientation] : geometry) {
    math::Vector<int> marker = origin;
    switch (shape) {
      case Shape::POINT:
        marker + orientation;
        break;
      case Shape::RAY:
        break;
    }
  }
}

auto Piece::attack_matrix(math::Vector<int> origin, const math::Matrix<Piece>& board) const noexcept
  -> std::expected<math::Matrix<byte>, std::string_view> {
  auto [m, n] = board.shape();
  math::Matrix<byte> attack_matrix{m, n, 0};
  const auto& traits = Traits::db()[id()];
  return attack_matrix;
}

}  // namespace melon
