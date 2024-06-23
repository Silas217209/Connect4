#include "Game.hpp"

#include "Board.hpp"

Turn turn;
Board board;

Game::Game(Turn turn, Board board) : turn(turn), board(board) {}

auto Game::play_move(unsigned int col) -> int {
    int result = board.play_move(col, turn);
    if (result != 0) {
        return 1;
    }

    switch (turn) {
        case yellow:
            turn = Turn::red;
            break;
        case red:
            turn = Turn::yellow;
            break;
    }

    return 0;
}
