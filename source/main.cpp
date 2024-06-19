#include <immintrin.h>
#include <sys/types.h>


#include "board.hpp"
#include "constants.hpp"

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

auto main() -> int {
    Board board;
    board.show_board();
    return 0;
}
