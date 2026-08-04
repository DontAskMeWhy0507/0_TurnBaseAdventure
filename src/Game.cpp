#include "Game.hpp"

#include <iostream>

Game::Game()
    : m_running(false)
    , m_savePath("data/save.dat")
{
}

Game::~Game() {
    shutdown();
}

bool Game::init() {
    std::cout << "[Game] Initializing..." << std::endl;

    // TODO: Load assets, set up world, etc.

    std::cout << "[Game] Init complete." << std::endl;
    return true;
}

void Game::run() {
    std::cout << "[Game] Entering main loop." << std::endl;
    m_running = true;

    // TODO: Implement the game loop
    // while (m_running) {
    //     processInput();
    //     update();
    //     render();
    // }

    // Placeholder — exit immediately
    std::cout << "[Game] No game loop yet — exiting." << std::endl;
    m_running = false;
}

void Game::shutdown() {
    if (m_running) {
        m_running = false;
    }
    std::cout << "[Game] Shutdown complete." << std::endl;
}
