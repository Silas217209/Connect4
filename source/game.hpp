#pragma once

#include "board.hpp"

class game {
  public:
    game(turn turn, board board);

    enum turn turn;
    class board board;

    auto play_move(unsigned int col) -> int {
        int result = board.play_move(col, turn);
        if (result != 0) {
            return 1;
        }

        switch (turn) {
            case yellow:
                turn = turn::red;
                break;
            case red:
                turn = turn::yellow;
                break;
        }

        return 0;
    }
};
