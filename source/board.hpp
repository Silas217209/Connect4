#pragma once
#include <array>
#include <cstdint>

using bitboard = uint64_t;

enum turn { yellow, red };

struct legal_moves {
    std::array<int, 7> legal_moves;
    int count;
};

class board {
  public:
    bitboard yellow_bitboard = 0b0;
    bitboard red_bitboard = 0b0;

    auto play_move(unsigned int col, turn turn) -> int;
    auto check_win(turn turn) -> bool;
    void show_board() const;
    static void show_any_board(bitboard board);
    auto get_legal_moves(turn turn) -> legal_moves;

  private:
    static const std::array<bitboard, 7> column_masks;
    static const std::array<bitboard, 6> row_masks;
    static const std::array<bitboard, 6> diagonal_mask;
    static const std::array<bitboard, 6> other_diagonal_mask;
};
