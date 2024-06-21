#include <sys/types.h>

#include <functional>
#include <iostream>
#include <random>

#include "board.hpp"
#include "game.hpp"

// ˯ msb
// 0 0 0 0 0 0 0
// 0 0 0 0 0 0 0
// 0 0 0 0 0 0 0
// 0 0 0 0 0 0 0
// 0 0 0 0 0 0 0
// 0 0 0 0 0 0 0
//             ^ lsb

auto random_turn() -> turn {
    std::random_device rd;
    std::mt19937 gen(rd());

    // Create a uniform distribution for the range of enum values
    std::uniform_int_distribution<> dist(0, 1);

    // Generate a random value and cast it to the enum type
    return static_cast<turn>(dist(gen));
}

auto main() -> int {
    board board;
    game game(random_turn(), board);



    return 0;
}
