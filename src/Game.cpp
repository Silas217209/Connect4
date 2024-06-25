#include "Game.hpp"

#include "Board.hpp"
#include "Errors.hpp"

Turn turn;
Board board;

Game::Game(Turn turn, Board board) : turn(turn), board(board) {}

auto Game::play_move(unsigned int col) -> PlayMoveError {
    PlayMoveError result = board.play_move(col, turn);
    if (result == PlayMoveError::column_out_of_range) {
        return PlayMoveError::column_out_of_range;
    }

    if (result == PlayMoveError::column_full) {
        return PlayMoveError::column_full;
    }

    switch (turn) {
        case yellow:
            turn = Turn::red;
            break;
        case red:
            turn = Turn::yellow;
            break;
    }

    return PlayMoveError::no_error;
}

auto Game::undo_move(unsigned int col) -> PlayMoveError {
    switch (turn) {
        case yellow:
            turn = Turn::red;
            break;
        case red:
            turn = Turn::yellow;
            break;
    }

    PlayMoveError result = board.undo_move(col, turn);
    if (result == PlayMoveError::column_out_of_range) {
        return PlayMoveError::column_out_of_range;
    }

    return PlayMoveError::no_error;
}
