/*
 * Each piece has "Traits" defined in a JSON.
 */
#ifndef MELON_TRAITS_H_
#define MELON_TRAITS_H_

#include <array>
#include <vector>

#include "constants.hpp"
#include "melon/math/matrix.hpp"
#include "melon/math/vector.hpp"
#include "melon/util.hpp"

namespace melon {

enum class Action : byte { EN_PASSANT, CASTLE, DOUBLE_STEP };

enum class Effect : byte { EN_PASSANT, CASTLE, PROMOTION, CHECK, CHECKMATE };

enum class Shape : byte { POINT, RAY };

/*
 * a Geometry is several shapes overlaid on top of one another
 * Each shape is oriented with a math::Vector
 * parallel arrays (struct of arrays) -> invariant: shapes.size() == orientations.size()
 */
class Geometry {
  std::vector<Shape> shapes;
  std::vector<math::Vector<int>> orientations;

public:
  /*
   * Returns a "mask" where Matrix.shape() == shape that describes this Geometry
   * shapes are applied in their corresponding orientations, overlaid on top of one another
   * basically, shape[0] || shape[1] || ... elementwise
   */
  [[nodiscard]] auto mask(math::Vector<int> origin, std::tuple<std::size_t, std::size_t> shape) const noexcept -> math::Matrix<byte>;
  [[nodiscard]] std::string serialize() const noexcept;
  [[nodiscard]] static Geometry deserialize() noexcept;
};

/*
 * Traits is a simple aggregate struct with some associated static functions for accessing the global Traits::db() and loading more Traits from JSON
 */
struct Traits {
  Geometry moves;
  Geometry attacks;
  std::vector<Action> actions;
  std::vector<Effect> effects;
  /*
   * returns array with traits for every loaded piece defining each one's behavior.
   * the array is indexed by Piece::id() and returns the corresponding Traits struct.
   * must not be accessed in dtor of global object (static dtor ordering issues).
   * initialized on first call of this method and can load more pieces using Traits::load_traits.
   * the returned reference isn't const because Traits::load_traits needs to modify it.
   */
  [[nodiscard]] static auto db() noexcept -> std::array<Traits, constants::MAX_PIECES>&;  // TODO: fine-grained lazy initialization, is it needed?
  
  /*
   * constructs Traits from data and overwrites Traits::db()[id] with the new Traits.
   * fallible, returns a bool: true -> load_traits succeeded, false -> load_traits failed 
   * nlohmann::json::parse takes an rvalue reference
   */
  [[nodiscard]] static bool load_traits(unsigned char id, std::string&& data) noexcept;
};

}  // namespace melon

#endif
