#pragma once

#include <array>
#include <cstdint>

#include "Errors.hpp"

using Bitboard = uint64_t;

enum Turn { yellow, red };

struct LegalMoves {
    std::array<int, 7> legal_moves;
    int count;
};

class Board {
  public:
    Bitboard yellow_bitboard = 0b0;
    Bitboard red_bitboard = 0b0;

    auto play_move(unsigned int col, Turn turn) -> PlayMoveError;
    auto check_win(Turn turn) -> bool;
    void show_board() const;
    static void show_any_board(Bitboard board);
    auto get_legal_moves(Turn turn) -> LegalMoves;

  private:
    static const std::array<Bitboard, 7> column_masks;
    static const std::array<Bitboard, 6> row_masks;
    static const std::array<Bitboard, 6> diagonal_mask;
    static const std::array<Bitboard, 6> other_diagonal_mask;
};
