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
const uint64_t row_masks[6] = {
    0b0000000000000000000000'1111111'0000000'0000000'0000000'0000000'0000000,
    0b0000000000000000000000'0000000'1111111'0000000'0000000'0000000'0000000,
    0b0000000000000000000000'0000000'0000000'1111111'0000000'0000000'0000000,
    0b0000000000000000000000'0000000'0000000'0000000'1111111'0000000'0000000,
    0b0000000000000000000000'0000000'0000000'0000000'0000000'1111111'0000000,
    0b0000000000000000000000'0000000'0000000'0000000'0000000'0000000'1111111,
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
    uint64_t yellow_bitbaord = 0;
    uint64_t red_bitbaord = 0b10;

    auto play_move(unsigned int col, Turn turn) {
        // row out of range 0 <= col <= 6
        if (col > 6) {
            return 1;
        }

        uint64_t current_board;
        switch (turn) {
            case Turn::red:
                current_board = red_bitbaord;
            case Turn::yellow:
                current_board = yellow_bitbaord;
        }

    }

    void show_board() {
        for (int row = 5; row >= 0; row--) {
            for (int col = 6; col >= 0; col--) {
                uint64_t index = static_cast<uint64_t>(row) * 7 + col;
                uint64_t field_mask = static_cast<uint64_t>(1) << index;

                if ((yellow_bitbaord & field_mask) != 0) {
                    std::cout << "◯ ";
                } else if ((red_bitbaord & field_mask) != 0) {
                    std::cout << "● ";
                } else {
                    std::cout << ". ";
                }
            }
            std::cout << "\n";
        }
    }

    static void show_any_board(uint64_t board) {
        for (int row = 5; row >= 0; row--) {
            for (int col = 6; col >= 0; col--) {
                uint64_t index = static_cast<uint64_t>(row) * 7 + col;
                uint64_t field_mask = static_cast<uint64_t>(1) << index;

                if ((board & field_mask) != 0) {
                    std::cout << "● ";
                } else {
                    std::cout << ". ";
                }
            }
            std::cout << "\n";
        }
    }
};

auto main() -> int {
    for (uint64_t board : row_masks) {
        Board::show_any_board(board);
        std::cout << "\n";
    }

    return 0;
}
