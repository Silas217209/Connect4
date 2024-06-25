#pragma once

#include "Board.hpp"

auto bot(Board board) -> int;

auto evaluate_board(Bitboard bitboard) -> int;

auto longestStreakOfOnes(uint64_t num) -> int;
