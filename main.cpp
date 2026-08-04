/**
 * TurnBaseAdventure - Game entry point
 *
 * A turn-based adventure game built with C++11.
 */

#include <iostream>
#include <string>

#include "Game.hpp"

int main() {
    std::cout << "TurnBaseAdventure v" << GAME_VERSION << std::endl;
    std::cout << "Starting game..." << std::endl;

    Game game;
    game.init();
    game.run();

    std::cout << "Game finished. Goodbye!" << std::endl;
    return 0;
}
