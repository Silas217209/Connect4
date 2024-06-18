#include <immintrin.h>
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
    0b0000000000000000000000'0000000'0000000'0000000'0000000'0000000'1111111,
    0b0000000000000000000000'0000000'0000000'0000000'0000000'1111111'0000000,
    0b0000000000000000000000'0000000'0000000'0000000'1111111'0000000'0000000,
    0b0000000000000000000000'0000000'0000000'1111111'0000000'0000000'0000000,
    0b0000000000000000000000'0000000'1111111'0000000'0000000'0000000'0000000,
    0b0000000000000000000000'1111111'0000000'0000000'0000000'0000000'0000000,
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
    uint64_t yellow_bitbaord = 0b0;
    uint64_t red_bitbaord = 0b0;

    auto play_move(unsigned int col, Turn turn) {
        // row out of range 0 <= col <= 6
        if (col > 6) {
            return 1;
        }

        uint64_t current_board;
        switch (turn) {
            case Turn::red:
                current_board = red_bitbaord;
                break;
            case Turn::yellow:
                current_board = yellow_bitbaord;
                break;
        }

        unsigned int current_row = 0;
        uint64_t current_field = column_masks[col] & row_masks[current_row];
        while ((current_field & (red_bitbaord | yellow_bitbaord)) != 0) {
            current_row++;
            current_field = column_masks[col] & row_masks[current_row];
        }

        current_board |= current_field;

        switch (turn) {
            case Turn::red:
                red_bitbaord = current_board;
                break;
            case Turn::yellow:
                yellow_bitbaord = current_board;
                break;
        }

        return 0;
    }

    auto check_win(Turn turn) -> bool {
        uint64_t current_board;
        switch (turn) {
            case Turn::red:
                current_board = red_bitbaord;
                break;
            case Turn::yellow:
                current_board = yellow_bitbaord;
                break;
        }

        // vertical
        for (auto col : column_masks) {
            uint64_t extracted_col = _pext_u64(current_board, col);
            uint64_t starting_comp = 0b1111;
            for (int i = 0; i < 3; i++) {
                starting_comp <<= 1;
                if (extracted_col == starting_comp) {
                    return true;
                }
            }
        }

        return false;
    }

    void show_board() const {
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
    Board board;

    board.play_move(0, Turn::yellow);
    board.play_move(0, Turn::yellow);
    board.play_move(0, Turn::yellow);
    board.play_move(0, Turn::yellow);
    board.show_board();

    std::cout << "\n" << board.check_win(Turn::yellow) << "\n";

    return 0;
}
