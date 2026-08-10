#include "GameApp.hpp"
#include "TeamPersistence.hpp"

#include <iostream>

GameApp::GameApp()
    : m_running(false), m_shutdownComplete(false)
{
}

GameApp::~GameApp() {
    shutdown();
}

bool GameApp::init() {
    std::cout << "[GameApp] Initializing..." << std::endl;
    m_shutdownComplete = false;

    // Load the roster first so team member IDs can be validated.
    m_roster.loadFromFile("data/characters.txt");
    TeamPersistence::loadTeams("data/teams.txt", m_teamManager, &m_roster);

    // Inject managers into MenuController
    m_menu.setTeamManager(&m_teamManager);
    m_menu.setCharacterRoster(&m_roster);
    m_menu.setBattleEngine(&m_battleEngine);

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
    if (m_shutdownComplete) {
        return;
    }
    if (m_running) {
        m_running = false;
    }

    // Persist current team state before exit
    TeamPersistence::saveTeams("data/teams.txt", m_teamManager);
    m_roster.saveToFile("data/characters.txt");
    m_shutdownComplete = true;

    std::cout << "[GameApp] Shutdown complete." << std::endl;
}
