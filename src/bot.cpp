#include "bot.hpp"

#include <immintrin.h>

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

auto evaluate_board(Bitboard bitboard) -> int {
    int score = 0;
    int exponent = 5;

    for (auto col_mask : Board::column_masks) {
        uint64_t extracted_col = _pext_u64(bitboard, col_mask);
        int streak = longestStreakOfOnes(extracted_col);

        score += streak_eval(streak);
    }

    for (auto row_mask : Board::row_masks) {
        uint64_t extracted_row = _pext_u64(bitboard, row_mask);
        int streak = longestStreakOfOnes(extracted_row);

        score += streak_eval(streak);
    }

    for (auto diag_mask : Board::diagonal_mask) {
        uint64_t extracted_diag = _pext_u64(bitboard, diag_mask);
        int streak = longestStreakOfOnes(extracted_diag);

        score += streak_eval(streak);
    }

    for (auto anti_diag_mask : Board::other_diagonal_mask) {
        uint64_t extracted_anti_diag = _pext_u64(bitboard, anti_diag_mask);
        int streak = longestStreakOfOnes(extracted_anti_diag);

        score += streak_eval(streak);
    }

    return score;
}

auto evaluate(Game game) -> int {
    int red_score = evaluate_board(game.board.red_bitboard);
    int yellow_score = evaluate_board(game.board.yellow_bitboard);

    if (game.board.check_win(Turn::red)) {
        return 10000;
    } else if (game.board.check_win(Turn::yellow)) {
        return 10000;
    }

    switch (game.turn) {
        case Turn::red:
            return red_score - yellow_score;
        case Turn::yellow:
            return yellow_score - red_score;
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
