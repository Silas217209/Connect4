#include "bot.hpp"

#include <immintrin.h>

#include <climits>
#include <iostream>

#include "Board.hpp"
#include "Game.hpp"

auto evaluate_board(Bitboard bitboard) -> int {
    const int value_2 = 0;
    const int value_3 = 1;
    const int value_4 = 0;

    int score = 0;
    // vertical
    for (auto col : Board::column_masks) {
        Bitboard extracted_col = _pext_u64(bitboard, col);
        Bitboard comp4 = 0b1111;
        Bitboard comp3 = 0b111;
        Bitboard comp2 = 0b11;

        for (int i = 0; i < 4; i++) {
            if (extracted_col == comp4) {
                score += value_4;
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

    // horizontal
    for (auto col : Board::row_masks) {
        Bitboard extracted_col = _pext_u64(bitboard, col);
        Bitboard comp4 = 0b1111;
        Bitboard comp3 = 0b111;
        Bitboard comp2 = 0b11;

        for (int i = 0; i < 5; i++) {
            if (extracted_col == comp4) {
                score += value_4;
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
    // diagonal 1
    for (auto col : Board::diagonal_mask) {
        Bitboard extracted_col = _pext_u64(bitboard, col);
        Bitboard comp4 = 0b1111;
        Bitboard comp3 = 0b111;
        Bitboard comp2 = 0b11;

        for (int i = 0; i < 4; i++) {
            if (extracted_col == comp4) {
                score += value_4;
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

    // diagonal 2
    for (auto col : Board::other_diagonal_mask) {
        Bitboard extracted_col = _pext_u64(bitboard, col);
        Bitboard comp4 = 0b1111;
        Bitboard comp3 = 0b111;
        Bitboard comp2 = 0b11;

        for (int i = 0; i < 4; i++) {
            if (extracted_col == comp4) {
                score += value_4;
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
    int red_score = evaluate_board(game.board.red_bitboard);
    int yellow_score = evaluate_board(game.board.yellow_bitboard);

    if (game.board.check_win(Turn::red)) {
        red_score += 10000;
    }

        if (game.board.check_win(Turn::red)) {
        red_score += 10000;
    }

    switch (game.turn) {
        case yellow:
            return yellow_score - red_score;
        case red:
            return red_score - yellow_score;
    }
}

auto alpha_beta(Game game, int alpha, int beta, int depth) -> int {
    if (depth == 0) {
        return evaluate(game);
    }

    int max = INT_MIN;
    LegalMoves moves = game.board.get_legal_moves(game.turn);
    for (int i = 0; i < moves.count; i++) {
        int move = moves.legal_moves[i];
        game.play_move(move);
        int score = -alpha_beta(game, -beta, -alpha, depth - 1);
        game.undo_move(move);

        if (score >= beta) {
            return score;
        }

        if (score > max) {
            max = score;
            if (score > alpha) {
                alpha = score;
            }
        }
    }
    return max;
}

auto bot(Board board) -> int {
    Game game(Turn::red, board);
    LegalMoves moves = game.board.get_legal_moves(Turn::red);

    int best_move = -1;
    int best_score = INT_MIN;

    for (int i = 0; i < moves.count; i++) {
        int move = moves.legal_moves[i];
        game.play_move(move);
        int score = alpha_beta(game, INT_MIN, INT_MAX, 7);
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
