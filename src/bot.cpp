#include "bot.hpp"

#include <immintrin.h>

#include <climits>
#include <iostream>

#include "Board.hpp"
#include "Game.hpp"

auto evaluate_board(Bitboard bitboard, Bitboard opposing_bitboard) -> int {
    const int value_2 = 3;
    const int value_3 = 50;
    const int value_4 = 0;

    int score = 0;
    // vertical
    for (auto col : Board::column_masks) {
        Bitboard extracted_col = _pext_u64(bitboard, col);
        Bitboard extracted_col_opp = _pext_u64(opposing_bitboard, col);

        Bitboard comp4 = 0b1111;
        Bitboard comp3 = 0b111;
        Bitboard comp2 = 0b11;

        for (int i = 0; i < 4; i++) {
            if ((extracted_col_opp & comp4) != 0) {
                continue;
            }
            if (extracted_col == comp3) {
                score += value_3;
            } else if (extracted_col == comp2) {
                score += value_2;
            }
            comp4 <<= 1;
            comp3 <<= 1;
            comp2 <<= 1;
        }
    }

    // horizontal
    for (auto col : Board::row_masks) {
        Bitboard extracted_col = _pext_u64(bitboard, col);
        Bitboard extracted_col_opp = _pext_u64(opposing_bitboard, col);
        Bitboard comp4 = 0b1111;
        Bitboard comp3 = 0b111;
        Bitboard comp2 = 0b11;

        for (int i = 0; i < 5; i++) {
            if ((extracted_col_opp & comp4) != 0) {
                continue;
            }
            if (extracted_col == comp3) {
                score += value_3;
            } else if (extracted_col == comp2) {
                score += value_2;
            }
            comp4 <<= 1;
            comp3 <<= 1;
            comp2 <<= 1;
        }
    }
    // diagonal 1
    for (auto col : Board::diagonal_mask) {
        Bitboard extracted_col = _pext_u64(bitboard, col);
        Bitboard extracted_col_opp = _pext_u64(opposing_bitboard, col);
        Bitboard comp4 = 0b1111;
        Bitboard comp3 = 0b111;
        Bitboard comp2 = 0b11;

        for (int i = 0; i < 4; i++) {
            if ((extracted_col_opp & comp4) != 0) {
                continue;
            }

            if (extracted_col == comp3) {
                score += value_3;
            } else if (extracted_col == comp2) {
                score += value_2;
            }
            comp4 <<= 1;
            comp3 <<= 1;
            comp2 <<= 1;
        }
    }

    // diagonal 2
    for (auto col : Board::other_diagonal_mask) {
        Bitboard extracted_col = _pext_u64(bitboard, col);
        Bitboard extracted_col_opp = _pext_u64(bitboard, col);
        Bitboard comp4 = 0b1111;
        Bitboard comp3 = 0b111;
        Bitboard comp2 = 0b11;

        for (int i = 0; i < 4; i++) {
            if ((extracted_col_opp & comp4) != 0) {
                continue;
            }
            if (extracted_col == comp3) {
                score += value_3;
            } else if (extracted_col == comp2) {
                score += value_2;
            }
            comp3 <<= 1;
            comp2 <<= 1;
        }
    }

    return score;
}

auto evaluate(Game game) -> int {
    int red_score =
        evaluate_board(game.board.red_bitboard, game.board.yellow_bitboard);
    int yellow_score =
        evaluate_board(game.board.yellow_bitboard, game.board.red_bitboard);
    /*
    if (game.board.check_win(Turn::red)) {
        red_score = 10000;
    }
    if (game.board.check_win(Turn::yellow)) {
        yellow_score = 10000;
    }*/
    switch (game.turn) {
        case yellow:
            return yellow_score - red_score;
        case red:
            return red_score - yellow_score;
    }
}

auto alpha_beta(Game game, int alpha, int beta, int depth) -> int {
    if (depth == 0 || game.board.check_win(Turn::yellow)
        || game.board.check_win(Turn::red)) {
        return evaluate(game);
    }

    int bestscore = INT_MIN;
    LegalMoves moves = game.board.get_legal_moves(game.turn);
    for (int i = 0; i < moves.count; i++) {
        int move = moves.legal_moves[i];
        game.play_move(move);
        int score = -alpha_beta(game, -beta, -alpha, depth - 1);
        game.undo_move(move);

        if (score >= beta)
            return score;  // fail-soft beta-cutoff
        if (score >= bestscore) {
            bestscore = score;
            if (score > alpha)
                alpha = score;
        }
    }
    return bestscore;
}

auto bot(Board board) -> int {
    Game game(Turn::red, board);
    LegalMoves moves = game.board.get_legal_moves(Turn::red);

    int best_move = -1;
    int best_score = INT_MIN;

    for (int i = 0; i < moves.count; i++) {
        int move = moves.legal_moves[i];
        game.play_move(move);
        int score = alpha_beta(game, INT_MIN, INT_MAX, 12);
        game.undo_move(move);

        std::cout << "Move: " << move << "\t" << "Score: " << score << "\n";

        if (score > best_score) {
            best_move = move;
            best_score = score;
        }
    }
    std::cout << best_move << "\n";

    return best_move;
}
