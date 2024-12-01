/*
 * This header provides nlohmann::json conversions for traits.hpp types
 */

#ifndef MELON_TRAITS_JSON_HPP_
#define MELON_TRAITS_JSON_HPP_

#include <nlohmann/json.hpp>
#include <iostream>

#include "traits.hpp"

using json = nlohmann::json;

namespace melon {

NLOHMANN_JSON_SERIALIZE_ENUM(
  Action,
  {
    {Action::EN_PASSANT, "en_passant"},
    {Action::CASTLE, "castle"},
    {Action::DOUBLE_STEP, "double_step"},
  }
)

NLOHMANN_JSON_SERIALIZE_ENUM(
  Effect,
  {
    {Effect::EN_PASSANT, "en_passant"},
    {Effect::CASTLE, "castle"},
    {Effect::PROMOTION, "promotion"},
    {Effect::CHECK, "check"},
    {Effect::CHECKMATE, "checkmate"},
  }
)

NLOHMANN_JSON_SERIALIZE_ENUM(
  Shape,
  {
    {Shape::POINT, "point"},
    {Shape::RAY, "ray"},
  }
)

inline void to_json(json& json, const std::tuple<Shape, math::Vector<int>>& pair) {
  auto [shape, orientation] = pair;
  json = {
    {"shape", shape},
    {"orientation", {orientation.x, orientation.y}},
  };
}

inline void from_json(const json& json, std::tuple<Shape, math::Vector<int>>& pair) {
  pair = {json["shape"], {json["orientation"][0], json["orientation"][1]}};
}

inline void to_json(json& json, const Geometry& geometry) {
  std::vector<std::tuple<Shape, math::Vector<int>>> list;
  for (const auto [shape, orientation] : geometry) list.emplace_back(shape, orientation);
  json = list;
}

// TODO: this function has no way to indicate a failure, but user json is fallible
inline void from_json(const json& json, Geometry& geometry) {
  std::vector<Shape> shapes;
  std::vector<math::Vector<int>> orientations;
  for (const auto& pair : json) {
    std::cout << pair << '\n';
    // shapes.push_back(shape);
    // orientations.push_back(orientation);
  }
  geometry = Geometry{std::move(shapes), std::move(orientations)};
}

inline void to_json(json& json, const Traits& traits) {
  json = {
    {"moves", traits.moves},
    {"attacks", traits.attacks},
    {"actions", traits.actions},
    {"effects", traits.effects},
  };
}

inline void from_json(const json& json, Traits& traits) {
  traits = Traits{
    json["moves"],
    json["attacks"],
    json["actions"],
    json["effects"],
  };
}

}  // namespace melon

#endif  // !MELON_TRAITS_JSON_HPP_
