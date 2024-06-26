#include "bot.hpp"

#include <immintrin.h>

#include <array>
#include <bit>
#include <climits>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <iostream>

#include "Board.hpp"
#include "Game.hpp"

auto longestStreakOfOnes(uint64_t num) -> int {
    int maxStreak = 0;
    int currentStreak = 0;

    while (num != 0) {
        if (num & 1) {
            currentStreak++;
            if (currentStreak > maxStreak) {
                maxStreak = currentStreak;
            }
        } else {
            currentStreak = 0;
        }
        num >>= 1;  // Shift the bits to the right
    }

    return maxStreak;
};

auto streak_eval(int streak) -> int {
    if (streak == 4) {
        return 1000;
    }
    if (streak == 3) {
        return 100;
    }

    if (streak == 2) {
        return 0;
    }

    return -5;
}

/*
 * stolen from https://github.com/stratzilla/connect-four/tree/master
+-----------------+-----------------+------------+----------+
| Friendly Pieces | Opposing Pieces | Empty Spot | Score    |
+-----------------+-----------------+------------+----------+
| 4               | 0               | 0          | +500,001 |
| 3               | 0               | 1          | +5,000   |
| 2               | 0               | 2          | +500     |
| 0               | 2               | 2          | -501     |
| 0               | 3               | 1          | -5,001   |
| 0               | 4               | 0          | -500,000 |
+-----------------+-----------------+------------+----------+
*/

auto eval_scoreset(int my, int opp, int empty) -> int {
    if (my == 4 && opp == 0 && empty == 0) {
        return 500001;
    }
    if (my == 3 && opp == 0 && empty == 1) {
        return 5000;
    }
    if (my == 2 && opp == 0 && empty == 2) {
        return 500;
    }
    if (my == 0 && opp == 2 && empty == 2) {
        return -501;
    }
    if (my == 0 && opp == 3 && empty == 1) {
        return -5001;
    }

    return 0;
}

auto evaluate_board(Bitboard my_bitboard, Bitboard opp_bitboard) -> int {
    int score = 0;
    int exponent = 5;

    for (auto col_mask : Board::column_masks) {
        uint64_t extracted_col = _pext_u64(my_bitboard, col_mask);

        // loop over extracted column in 4 bit windows
        uint64_t mask = 0b1111;
        for (int i = 0; i < 4; i++) {
            uint64_t window = extracted_col & mask;
            int my = std::popcount(window & my_bitboard);
            int opp = std::popcount(window & opp_bitboard);
            int empty = 4 - my - opp;
            score += eval_scoreset(my, opp, empty);
            mask <<= 1;
        }
    }

    for (auto row_mask : Board::row_masks) {
        uint64_t extracted_row = _pext_u64(my_bitboard, row_mask);

        // loop over extracted row in 4 bit windows
        uint64_t mask = 0b1111;
        for (int i = 0; i < 4; i++) {
            uint64_t window = extracted_row & mask;
            int my = std::popcount(window & my_bitboard);
            int opp = std::popcount(window & opp_bitboard);
            int empty = 4 - my - opp;
            score += eval_scoreset(my, opp, empty);
            mask <<= 1;
        }
    }

    for (auto diag_mask : Board::diagonal_mask) {
        uint64_t extracted_diag = _pext_u64(my_bitboard, diag_mask);

        // loop over extracted diagonal in 4 bit windows
        uint64_t mask = 0b1111;
        for (int i = 0; i < 4; i++) {
            uint64_t window = extracted_diag & mask;
            int my = std::popcount(window & my_bitboard);
            int opp = std::popcount(window & opp_bitboard);
            int empty = 4 - my - opp;
            score += eval_scoreset(my, opp, empty);
            mask <<= 1;
        }
    }

    for (auto anti_diag_mask : Board::other_diagonal_mask) {
        uint64_t extracted_anti_diag = _pext_u64(my_bitboard, anti_diag_mask);

        // loop over extracted anti-diagonal in 4 bit windows
        uint64_t mask = 0b1111;
        for (int i = 0; i < 4; i++) {
            uint64_t window = extracted_anti_diag & mask;
            int my = std::popcount(window & my_bitboard);
            int opp = std::popcount(window & opp_bitboard);
            int empty = 4 - my - opp;
            score += eval_scoreset(my, opp, empty);
            mask <<= 1;
        }
    }

    return score;
}

auto evaluate(Game game) -> int {
    int red_score = evaluate_board(game.board.red_bitboard, game.board.yellow_bitboard);
    int yellow_score = evaluate_board(game.board.yellow_bitboard, game.board.red_bitboard);

    if (game.board.check_win(Turn::red)) {
        return 10000;
    } else if (game.board.check_win(Turn::yellow)) {
        return -10000;
    }

    return red_score - yellow_score;
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
        // if other wins, avoid this move
        if (game.board.check_win(
                game.turn == Turn::yellow ? Turn::red : Turn::yellow
            )) {
            game.undo_move(move);
            return -10000;
        }
        int score = -alpha_beta(game, -beta, -alpha, depth - 1);
        game.undo_move(move);

        if (score >= beta)
            return score;  // fail-soft beta-cutoff
        if (score > bestscore) {
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
    const int max_depth = 10;
    int current_depth = 0;
    while (current_depth <= max_depth) {
        // add best move to beginning of moves
        if (best_move != -1) {
            int temp = moves.legal_moves[0];
            for (int i = 0; i < moves.count; i++) {
                if (moves.legal_moves[i] == best_move) {
                    moves.legal_moves[i] = temp;
                    break;
                }
                moves.legal_moves[i] = moves.legal_moves[i + 1];
            }
        }
        for (int i = 0; i < moves.count; i++) {
            int move = moves.legal_moves[i];
            game.play_move(move);
            int score = alpha_beta(game, INT_MIN, INT_MAX, current_depth);
            game.undo_move(move);

            if (score > best_score) {
                best_move = move;
                best_score = score;
            }
        }
        std::cout << "Depth: " << current_depth
                  << ", Best Move: " << best_move + 1
                  << ", Best Score: " << best_score << "\n";
        current_depth++;
    }
    std::cout << "Best Move: " << best_move + 1
              << ", Best Score: " << best_score << "\n";

    return best_move;
}
