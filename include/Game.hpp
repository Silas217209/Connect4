#pragma once

#include "Board.hpp"

class Game {
  public:
    Game(Turn turn, Board board);

    enum Turn turn;
    class Board board;

    auto play_move(unsigned int col) -> int;
};
