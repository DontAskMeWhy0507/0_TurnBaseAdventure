#ifndef TURN_BASE_ADVENTURE_GAME_APP_HPP
#define TURN_BASE_ADVENTURE_GAME_APP_HPP

#include "MenuController.hpp"
#include "TeamManager.hpp"
#include "CharacterRoster.hpp"
#include "BattleEngine.h"

constexpr const char* GAME_VERSION = "0.1.0";

/**
 * Top-level application class — owns the game lifecycle.
 *
 * Responsibilities:
 *   - Bootstrap subsystems
 *   - Delegate to MenuController for user interaction
 *   - Cleanup on exit
 *
 * Ownership notes (for later integration):
 *   When Member-3 and Member-4 deliver their modules, GameApp will own
 *   their manager instances (CharacterRoster*, TeamManager*, etc.) and
 *   pass references into MenuController.
 */
class GameApp {
public:
    GameApp();
    ~GameApp();

    /// Initialise subsystems. Returns false on fatal error.
    bool init();

    /// Enter the main game loop (blocks until shutdown).
    void run();

    /// Release resources; safe to call multiple times.
    void shutdown();

private:
    bool            m_running;
    bool            m_shutdownComplete;
    TeamManager     m_teamManager;
    CharacterRoster m_roster;
    BattleEngine    m_battleEngine;
    MenuController  m_menu;
};

#endif // TURN_BASE_ADVENTURE_GAME_APP_HPP
