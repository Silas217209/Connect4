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

const unsigned int row_count = 6;
const unsigned int col_count = 7;

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
    uint64_t yellow_bitbaord = 0b0;
    uint64_t red_bitbaord = 0b0;

    void show_board() const {
        for (int row = (row_count - 1); row >= 0; row--) {
            for (int col = col_count; col >= 0; col--) {
                uint64_t index = static_cast<uint64_t>(row) * col_count + col;
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
        for (int row = (row_count - 1); row >= 0; row--) {
            for (int col = col_count; col >= 0; col--) {
                uint64_t index = static_cast<uint64_t>(row) * col_count + col;
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
    for (uint64_t board : column_masks) {
        Board::show_any_board(board);
    }

    return 0;
}

