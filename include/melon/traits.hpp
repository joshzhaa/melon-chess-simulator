/*
 * Each piece has "Traits" defined in a JSON.
 */
#ifndef MELON_TRAITS_HPP_
#define MELON_TRAITS_HPP_

#include <array>
#include <expected>
#include <vector>

#include "constants.hpp"
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
  std::vector<Shape> shapes_;
  std::vector<math::Vector<int>> orientations_;

public:
  // factory func, to avoid defining a ctor -> needing to explicitly default ctor (too verbose to delete)
  [[nodiscard]] static Geometry make_geometry(std::vector<Shape>&& shapes, std::vector<math::Vector<int>>&& orientations) noexcept {
    Geometry result{};
    result.shapes_ = std::move(shapes);
    result.orientations_ = std::move(orientations);
    return result;
  }
  /*
   * Returns number of overlaid shapes in this Geometry
   */
  [[nodiscard]] std::size_t size() const noexcept { return shapes_.size(); }

  class Iterator {
    std::vector<Shape>::const_iterator shape_;
    std::vector<math::Vector<int>>::const_iterator orientation_;

  public:
    Iterator(std::vector<Shape>::const_iterator shape_iter, std::vector<math::Vector<int>>::const_iterator orientation_iter)
      : shape_{shape_iter},
        orientation_{orientation_iter} {}

    auto operator*() const noexcept -> std::tuple<Shape, math::Vector<int>> { return {*shape_, *orientation_}; }

    Iterator operator++() noexcept {
      ++shape_;
      ++orientation_;
      return *this;
    }

    bool operator!=(const Iterator& other) const noexcept { return shape_ != other.shape_ or orientation_ != other.orientation_; }
  };

  [[nodiscard]] Iterator begin() const noexcept { return {shapes_.cbegin(), orientations_.cbegin()}; }
  [[nodiscard]] Iterator end() const noexcept { return {shapes_.cend(), orientations_.cend()}; }
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
  [[nodiscard]] static bool load_traits(byte id, std::string&& data) noexcept;
};

}  // namespace melon

#endif
