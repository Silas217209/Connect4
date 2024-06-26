#pragma once

#include "Board.hpp"

auto bot(Board board) -> int;
auto bot2(Board board) -> int;

auto evaluate_board(Bitboard my_bitboard, Bitboard opp_bitboard) -> int;

auto longestStreakOfOnes(uint64_t num) -> int;
