/**
 * TurnBaseAdventure - Game entry point
 *
 * A turn-based adventure game built with C++11.
 */

#include <iostream>
#include <string>

#include "GameApp.hpp"

int main() {
    std::cout << "TurnBaseAdventure v" << GAME_VERSION << std::endl;

    GameApp app;
    if (!app.init()) {
        std::cerr << "Fatal: GameApp::init() failed." << std::endl;
        return 1;
    }

    app.run();
    app.shutdown();

    return 0;
}
