/*
 * Defines raw string literals for json serilized standard chess pieces
 */

namespace melon::standard_pieces {

char const * const B = R"(
{
  "moves": [{ "shape": "line", "vector": [[1, 1], [1, -1], [-1, -1], [-1, 1]] }],
  "attacks": [{ "shape": "line", "vector": [[1, 1], [1, -1], [-1, -1], [-1, 1]] }]
}
)";

char const * const P = R"(
{
  "moves": [{ "shape": "point", "vector": [[0, 1]] }],
  "attacks": [{ "shape": "point", "vector": [[1, 1], [-1, 1]] }],
  "actions": ["enpassant", "double_move"],
  "effects": ["enpassant", "promotion"]
}
)";

char const * const K = R"(
{
  "moves": [{ "shape": "point", "vector": [[0, 1], [1, -1], [0, -1], [-1, -1], [-1, 0], [-1, 1], [0, 1], [1, 1]]}]
  "attacks": [{ "shape": "point", "vector": [[0, 1], [1, -1], [0, -1], [-1, -1], [-1, 0], [-1, 1], [0, 1], [1, 1]]}]
  "actions": ["castle"],
  "effects": ["check", "checkmate"]
}
)";

}
