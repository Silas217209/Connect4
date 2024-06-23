#include "Board.hpp"

#include <immintrin.h>

#include <array>
#include <iostream>

#include "Errors.hpp"

// Define some ANSI escape codes for colors
#define RESET "\033[0m"
#define RED "\033[38;5;196m"
#define YELLOW "\033[38;5;226m"
#define ANNOTATION "\033[38;5;023m"

auto Board::play_move(unsigned int col, Turn turn) -> PlayMoveError {
    // row out of range 0 <= col <= 6
    if (col > 6) {
        return PlayMoveError::column_out_of_range;
    }

    Bitboard current_board;
    switch (turn) {
        case Turn::red:
            current_board = red_bitboard;
            break;
        case Turn::yellow:
            current_board = yellow_bitboard;
            break;
    }

    if (((red_bitboard | yellow_bitboard) & column_masks[col])
        == column_masks[col]) {
        return PlayMoveError::column_full;
    }

    unsigned int current_row = 0;
    Bitboard current_field = column_masks[col] & row_masks[current_row];
    while ((current_field & (red_bitboard | yellow_bitboard)) != 0) {
        current_row++;
        current_field = column_masks[col] & row_masks[current_row];
    }

    current_board |= current_field;

    switch (turn) {
        case Turn::red:
            red_bitboard = current_board;
            break;
        case Turn::yellow:
            yellow_bitboard = current_board;
            break;
    }

    return PlayMoveError::no_error;
}

auto Board::undo_move(unsigned int col, Turn turn) -> PlayMoveError {
    // row out of range 0 <= col <= 6
    if (col > 6) {
        return PlayMoveError::column_out_of_range;
    }

    Bitboard current_board;
    switch (turn) {
        case Turn::red:
            current_board = red_bitboard;
            break;
        case Turn::yellow:
            current_board = yellow_bitboard;
            break;
    }
    unsigned int current_row = 5;
    Bitboard current_field = column_masks[col] & row_masks[current_row];
    while ((current_field & (red_bitboard | yellow_bitboard)) == 0) {
        current_row--;
        current_field = column_masks[col] & row_masks[current_row];
    }

    if ((current_board & current_field) == 0) {
        return PlayMoveError::column_invalid;
    }

    current_board ^= current_field;

    switch (turn) {
        case Turn::red:
            red_bitboard = current_board;
            break;
        case Turn::yellow:
            yellow_bitboard = current_board;
            break;
    }

    return PlayMoveError::no_error;
}

auto Board::check_win(Turn turn) -> bool {
    Bitboard current_board;
    switch (turn) {
        case Turn::red:
            current_board = red_bitboard;
            break;
        case Turn::yellow:
            current_board = yellow_bitboard;
            break;
    }

    // vertical
    for (auto col : column_masks) {
        Bitboard extracted_col = _pext_u64(current_board, col);
        Bitboard vert_comp = 0b1111;
        for (int i = 0; i < 3; i++) {
            if (extracted_col == vert_comp) {
                return true;
            }
            vert_comp <<= 1;
        }
    }

    // horizontal
    for (auto row : row_masks) {
        Bitboard extracted_row = _pext_u64(current_board, row);
        Bitboard horizontal_comp = 0b1111;

        for (int i = 0; i < 4; i++) {
            if (extracted_row == horizontal_comp) {
                return true;
            }

            horizontal_comp <<= 1;
        }
    }

    // diagonal bottom left to top right
    for (auto diag : diagonal_mask) {
        Bitboard extracted_diag = _pext_u64(current_board, diag);
        Bitboard diag_comp = 0b1111;
        for (int i = 0; i < 2; i++) {
            if (extracted_diag == diag_comp) {
                return true;
            }
            diag_comp <<= 1;
        }
    }
    // diagonal bottom left to top right
    for (auto diag : other_diagonal_mask) {
        Bitboard extracted_diag = _pext_u64(current_board, diag);
        Bitboard diag_comp = 0b1111;
        for (int i = 0; i < 2; i++) {
            if (extracted_diag == diag_comp) {
                return true;
            }
            diag_comp <<= 1;
        }
    }
    return false;
}

void Board::show_board() const {
    for (int i = 0; i < 7; i++) {
        std::cout << ANNOTATION << i + 1 << " " << RESET;
    }
    std::cout << "\n";
    for (int row = 5; row >= 0; row--) {
        for (int col = 6; col >= 0; col--) {
            Bitboard index = static_cast<uint64_t>(row) * 7 + col;
            Bitboard field_mask = static_cast<uint64_t>(1) << index;

            if ((yellow_bitboard & field_mask) != 0) {
                std::cout << YELLOW << "● " << RESET;
            } else if ((red_bitboard & field_mask) != 0) {
                std::cout << RED << "● " << RESET;
            } else {
                std::cout << ". ";
            }
        }
        std::cout << "\n";
    }
}

void Board::show_any_board(Bitboard board) {
    for (int row = 5; row >= 0; row--) {
        for (int col = 6; col >= 0; col--) {
            Bitboard index = static_cast<uint64_t>(row) * 7 + col;
            Bitboard field_mask = static_cast<uint64_t>(1) << index;

            if ((board & field_mask) != 0) {
                std::cout << "● ";
            } else {
                std::cout << ". ";
            }
        }
        std::cout << "\n";
    }
}

auto Board::get_legal_moves(Turn turn) -> LegalMoves {
    Bitboard current_board;
    switch (turn) {
        case Turn::red:
            current_board = red_bitboard;
            break;
        case Turn::yellow:
            current_board = yellow_bitboard;
            break;
    }
    std::array<int, 7> moves {};
    int count = 0;
    for (int i = 0; i < 7; i++) {
        // full
        if (((red_bitboard | yellow_bitboard) & column_masks[i])
            == column_masks[i]) {
            continue;
        }
        moves[count] = i;
        count++;
    }

    return LegalMoves {moves, count};
}
