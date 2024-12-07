/*
 * This header provides nlohmann::json conversions for traits.hpp types
 */

#ifndef MELON_TRAITS_JSON_HPP
#define MELON_TRAITS_JSON_HPP

#include <nlohmann/json.hpp>

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

inline void to_json(json& json, const Geometry& geometry) {
  std::vector<std::tuple<Shape, math::Vector<int>>> list;
  for (const auto [shape, orientation] : geometry) list.emplace_back(shape, orientation);
  json = list;
}

inline void from_json(const json& json, Geometry& geometry) {
  geometry = Geometry{};
  std::vector<Shape> shapes;
  std::vector<math::Vector<int>> orientations;
  for (const auto& obj : json) {
    if (not obj.contains("shape")) return;
    shapes.push_back(obj["shape"]);
    if (not obj.contains("orientation") or obj["orientation"].size() != 2) return;
    orientations.emplace_back(obj["orientation"][0], obj["orientation"][1]);
  }
  geometry = Geometry::make_geometry(std::move(shapes), std::move(orientations));
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
    .moves = json["moves"],
    .attacks = json["attacks"],
    .actions = json["actions"],
    .effects = json["effects"],
  };
}

}  // namespace melon

#endif  // MELON_TRAITS_JSON_HPP
