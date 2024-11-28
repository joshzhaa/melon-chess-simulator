/*
 * Each piece has "Traits" defined in a JSON.
 * Traits is a simple aggregate struct with some associated static functions for accessing
 * the global Traits::db() and loading more Traits from JSON
 */
#ifndef MELON_TRAITS_H_
#define MELON_TRAITS_H_

#include <array>

#include "constants.hpp"

namespace melon {

// TODO: determine struct layout
struct Traits {
  /*
   * returns array with traits for every loaded piece defining each one's behavior.
   * the array is indexed by Piece::id() and returns the corresponding Traits struct.
   * must not be accessed in dtor of global object (static dtor ordering issues).
   * initialized on first call of this method with Traits::load_default_traits.
   * can load more pieces using Traits::load_traits.
   * the returned reference isn't const because this->load_traits modifies it.
   */
  static auto db() noexcept -> std::array<Traits, constants::MAX_PIECES>&;  // TODO: fine-grained lazy initialization, is it needed?
  //
  /*
   * constructs Traits from data and overwrites Traits::db()[id] with the new Traits.
   * fallible, returns a bool: true -> load_traits succeeded, false -> load_traits failed
   * nlohmann::json::parse takes an rvalue reference
   */
  static bool load_traits(unsigned char id, std::string&& data) noexcept;
};

}  // namespace melon

#endif
