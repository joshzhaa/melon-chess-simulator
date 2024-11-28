/*
 * Each piece has "traits" defined in a JSON
 */
#include <array>

namespace melon {

constexpr std::size_t MAX_PIECES = sizeof(unsigned char);

struct Traits {

  explicit Traits() {
  }

  static auto db() -> const std::array<Traits, MAX_PIECES>& {
    static std::array<Traits, MAX_PIECES> known_pieces = load_default_traits();
    return known_pieces;
  }

  static auto load_default_traits() -> std::array<Traits, MAX_PIECES> {}
};

}
