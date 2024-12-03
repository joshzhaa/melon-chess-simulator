#ifndef MELON_TEXT_IO_HPP_
#define MELON_TEXT_IO_HPP_

#include <string>

#include "melon/piece.hpp"

namespace melon::text_io {

std::string icon(const Piece& piece);  // unicode chess icon for piece (id, team)
std::string text(const Piece& piece);  // ascii text representation of piece (id, team)
// text representation of board
std::string serialize(const math::Matrix<Piece>& board, bool use_icons = true);

}  // namespace melon::text_io

#endif // !MELON_TEXT_IO_HPP_
