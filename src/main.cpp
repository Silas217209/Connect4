#include <cstdlib>
#include <exception>
#include <iostream>
#include <random>
#include <string>

#include "Board.hpp"
#include "Errors.hpp"
#include "Game.hpp"
#include "bot.hpp"

auto random_turn() -> Turn {
    std::random_device rd;
    std::mt19937 gen(rd());

    // Create a uniform distribution for the range of enum values
    std::uniform_int_distribution<> dist(0, 1);

    // Generate a random value and cast it to the enum type
    return static_cast<Turn>(dist(gen));
}

auto human(Board board) -> int {
    while (true) {
        std::string input;
        std::cout << "Column: ";
        std::cin >> input;
        std::cout << "\n";

        try {
            int result = std::stoi(input);
            return result;
        } catch (std::exception) {
            std::cout << "Invalid Input\n";
        }
    }
}

auto main() -> int {
    Board board;
    Game game(random_turn(), board);

    switch (game.turn) {
        case yellow:
            std::cout << "Human begins\n";
            break;
        case red:
            std::cout << "Bot begins\n";
            break;
    }

    while (true) {
        game.board.show_board();
        std::cout << "\n";

        switch (game.turn) {
            case yellow: {
                PlayMoveError res = PlayMoveError::column_out_of_range;

                while (res == PlayMoveError::column_out_of_range) {
                    int move = human(game.board);
                    res = game.play_move(move);
                }

                if (res != PlayMoveError::no_error) {
                    std::cout << "Invalid move: Human Lost!\n";
                    return 1;
                }
                if (game.board.check_win(Turn::yellow)) {
                    std::cout << "Human won!\n";
                    return 0;
                }
                break;
            }
            case red: {
                int move = bot(game.board);
                PlayMoveError res = game.play_move(move);
                if (res != PlayMoveError::no_error) {
                    std::cout << "Invalid move: Bot Lost!\n";
                    return 1;
                }
                if (game.board.check_win(Turn::red)) {
                    std::cout << "Bot defeated Humanity!\n";
                    return 0;
                }
                break;
            }
        }
    }
}
