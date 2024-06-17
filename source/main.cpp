#include <sys/types.h>

#include <bitset>
#include <cstdint>
#include <iostream>
#include <string>

const uint64_t column_masks[7] = {
    0b0000000000000000000000'1000000'1000000'1000000'1000000'1000000'1000000,
    0b0000000000000000000000'0100000'0100000'0100000'0100000'0100000'0100000,
    0b0000000000000000000000'0010000'0010000'0010000'0010000'0010000'0010000,
    0b0000000000000000000000'0001000'0001000'0001000'0001000'0001000'0001000,
    0b0000000000000000000000'0000100'0000100'0000100'0000100'0000100'0000100,
    0b0000000000000000000000'0000010'0000010'0000010'0000010'0000010'0000010,
    0b0000000000000000000000'0000001'0000001'0000001'0000001'0000001'0000001,
};

enum Turn { yellow, red };

class Game {
  public:
    Turn turn = Turn::yellow;
};

// 0 0 0 0 0 0 0
// 0 0 0 0 0 0 0
// 0 0 0 0 0 0 0
// 0 0 0 0 0 0 0
// 0 0 0 0 0 0 0
// 0 0 0 0 0 0 0
//             ^ lsb

class Board {
  public:
    uint64_t yellow_bitbaord = 1;
    uint64_t red_bitbaord = 0b10;

    void print_board() {
        for (int row = 5; row >= 0; row--) {
            for (int col = 6; col >= 0; col--) {
                uint64_t index = static_cast<uint64_t>(row) * 7 + col;
                uint64_t field_mask = static_cast<uint64_t>(1) << index;

                //std::cout << std::bitset<42>(field_mask) << "\n";
                //continue;
                if ((yellow_bitbaord & field_mask) == 1) {
                    std::cout << "◯ ";
                } else if ((red_bitbaord & field_mask) == 1) {
                    std::cout << "● ";
                } else {
                    std::cout << ". ";
                }
            }
            std::cout << "\n";
        }
    }
};

#include "lib.hpp"

auto main() -> int {
    Board board = Board();

    board.print_board();

    return 0;
}
