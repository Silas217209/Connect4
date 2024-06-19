#include "board.hpp"

// ˯ msb
// 0 0 0 0 0 0 0
// 0 0 0 0 0 0 0
// 0 0 0 0 0 0 0
// 0 0 0 0 0 0 0
// 0 0 0 0 0 0 0
// 0 0 0 0 0 0 0
//             ^ lsb

auto main() -> int {
    board board;
    board.show_board();
    return 0;
}
