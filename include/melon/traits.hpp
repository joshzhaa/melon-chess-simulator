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

enum class Action : byte { EN_PASSANT = 0, CASTLE = 1, DOUBLE_STEP = 2 };
enum class Effect : byte { EN_PASSANT = 0, CASTLE = 1, PROMOTION = 2, CHECK = 3, CHECKMATE = 4 };
enum class Shape : byte { POINT = 0, RAY = 1 };

/*
 * a Geometry is several shapes overlaid on top of one another
 * Each shape is oriented with a math::Vector
 * parallel arrays (struct of arrays) -> invariant: shapes.size() == orientations.size()
 */
class Geometry {
  std::vector<Shape> shapes;
  std::vector<math::Vector<int>> orientations;

public:
  Geometry() = default;
  Geometry(std::vector<Shape>&& shapes_, std::vector<math::Vector<int>>&& orientations_)
      : shapes(std::move(shapes_)), orientations(std::move(orientations_)) {}
  /*
   * Returns a "mask" Matrix (Matrix.shape() == shape) that describes this Geometry
   * shapes are applied in their corresponding orientations, overlaid on top of one another
   * basically, shape[0] || shape[1] || ... elementwise
   */
  [[nodiscard]] auto mask(math::Vector<int> origin, std::tuple<std::size_t, std::size_t> shape) const noexcept -> math::Matrix<byte>;
  /*
   * Returns number of overlaid shapes in this Geometry
   */
  [[nodiscard]] std::size_t size() const noexcept { return shapes.size(); }

  class Iterator {
    std::size_t index;
    // TODO: possible without pointers?
    std::vector<Shape> const* shapes;
    std::vector<math::Vector<int>> const* orientations;

  public:
    Iterator(std::size_t initial_index, std::vector<Shape> const* shapes_ref, std::vector<math::Vector<int>> const* orientations_ref)
        : index{initial_index}, shapes{shapes_ref}, orientations{orientations_ref} {}

    auto operator*() const noexcept -> std::tuple<Shape, math::Vector<int>> {
      auto shape = (*shapes)[index];
      auto orientation = (*orientations)[index];
      return {shape, orientation};
    }

    Iterator operator++() noexcept {
      ++index;
      return *this;
    }

    bool operator!=(const Iterator& other) const noexcept {
      return index != other.index || shapes != other.shapes || orientations != other.orientations;
    }
  };

  [[nodiscard]] Iterator begin() const noexcept { return {0, &shapes, &orientations}; }
  [[nodiscard]] Iterator end() const noexcept { return {size(), &shapes, &orientations}; }
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
   * forced move b/c nlohmann::json::parse takes an rvalue reference
   */
  [[nodiscard]] static bool load_traits(std::string&& data) noexcept;
};

}  // namespace melon

#endif
