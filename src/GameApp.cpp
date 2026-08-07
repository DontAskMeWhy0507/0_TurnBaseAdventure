#include "GameApp.hpp"
#include "TeamPersistence.hpp"

#include <iostream>

GameApp::GameApp()
    : m_running(false)
{
}

GameApp::~GameApp() {
    shutdown();
}

bool GameApp::init() {
    std::cout << "[GameApp] Initializing..." << std::endl;

    // Load persisted teams (data/teams.txt); non-fatal if the file doesn't exist.
    TeamPersistence::loadTeams("data/teams.txt", m_teamManager);

    // Inject managers into MenuController
    m_menu.setTeamManager(&m_teamManager);

    std::cout << "[GameApp] Init complete." << std::endl;
    return true;
}

void GameApp::run() {
    std::cout << "[GameApp] Entering game loop." << std::endl;
    m_running = true;

    // The entire UI is driven by the menu controller.
    m_menu.runMainMenu();

    m_running = false;
}

void GameApp::shutdown() {
    if (m_running) {
        m_running = false;
    }

    // Persist current team state before exit
    TeamPersistence::saveTeams("data/teams.txt", m_teamManager);

    std::cout << "[GameApp] Shutdown complete." << std::endl;
}
