#include "bot.hpp"

#include <immintrin.h>

#include <climits>
#include <cmath>
#include <cstdint>
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

auto evaluate_board(Bitboard bitboard, Bitboard opposing_bitboard) -> int {
    int score = 0;
    int exponent = 3;

    for (auto col_mask : Board::column_masks) {
        uint64_t extracted_col = _pext_u64(bitboard, col_mask);
        int streak = longestStreakOfOnes(extracted_col);

        score += pow(streak, exponent);
    }

    for (auto row_mask : Board::row_masks) {
        uint64_t extracted_row = _pext_u64(bitboard, row_mask);
        int streak = longestStreakOfOnes(extracted_row);

        score += pow(streak, exponent);
    }

    for (auto diag_mask : Board::diagonal_mask) {
        uint64_t extracted_diag = _pext_u64(bitboard, diag_mask);
        int streak = longestStreakOfOnes(extracted_diag);

        score += pow(streak, exponent);
    }

    for (auto anti_diag_mask : Board::other_diagonal_mask) {
        uint64_t extracted_anti_diag = _pext_u64(bitboard, anti_diag_mask);
        int streak = longestStreakOfOnes(extracted_anti_diag);

        score += pow(streak, exponent);
    }

    return score;
}

auto evaluate(Game game) -> int {
    int red_score =
        evaluate_board(game.board.red_bitboard, game.board.yellow_bitboard);
    int yellow_score =
        evaluate_board(game.board.yellow_bitboard, game.board.red_bitboard);

    if (game.board.check_win(Turn::red)) {
        red_score = 10000;
    }
    if (game.board.check_win(Turn::yellow)) {
        yellow_score = 10000;
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

// implement iterative deepening
auto bot(Board board) -> int {
    Game game(Turn::red, board);
    LegalMoves moves = game.board.get_legal_moves(Turn::red);

    int best_move = -1;
    int best_score = INT_MIN;
    for (int i = 0; i < moves.count; i++) {
        int move = moves.legal_moves[i];
        game.play_move(move);
        int score = alpha_beta(game, INT_MIN, INT_MAX, 5);
        game.undo_move(move);

        if (score > best_score) {
            best_move = move;
            best_score = score;
        }
    }

    std::cout << "Best Move: " << best_move + 1
              << ", Best Score: " << best_score << "\n";

    return best_move;
}
