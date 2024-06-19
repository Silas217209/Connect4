#pragma once
#include <array>
#include <cstdint>

#include "constants.hpp"

enum Turn { yellow, red };

class Board {
  public:
    bitboard yellow_bitboard = 0b0;
    bitboard red_bitboard = 0b0;

    auto play_move(unsigned int col, Turn turn) -> int;
    auto check_win(Turn turn) -> bool;
    void show_board() const;
    static void show_any_board(bitboard board);

  private:
    static const std::array<bitboard, 7> column_masks;
    static const std::array<bitboard, 6> row_masks;
    static const std::array<bitboard, 6> diagonal_mask;
    static const std::array<bitboard, 6> other_diagonal_mask;
};
