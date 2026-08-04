#include "GameApp.hpp"

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

    // TODO: When modules are ready, instantiate managers here:
    //   m_roster    = new CharacterRoster();
    //   m_teamMgr   = new TeamManager(m_roster);
    //   m_fileMgr   = new PersistenceManager();
    //   m_battle    = new BattleEngine(m_roster);
    // And pass them to m_menu (e.g. m_menu.setDependencies(...))

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

    // TODO: delete owned manager instances when modules arrive.

    std::cout << "[GameApp] Shutdown complete." << std::endl;
}
