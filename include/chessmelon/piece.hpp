#ifndef __PIECE_H__

class Piece {
  char id_;
  bool moved_;

public:
  explicit Piece(char id);
  bool move();
};

#endif // __PIECE_H__
