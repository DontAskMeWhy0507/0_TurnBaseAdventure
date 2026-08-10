#ifndef TURN_BASE_ADVENTURE_MENU_CONTROLLER_HPP
#define TURN_BASE_ADVENTURE_MENU_CONTROLLER_HPP

class TeamManager;
class CharacterRoster;
class BattleEngine;

/**
 * Console-based menu system for the Turn-Based Adventure game.
 *
 * Owns the navigation logic for main + sub-menus.
 * Managers are injected via setter methods (TeamManager, etc.) as they
 * become available from GameApp.
 */
class MenuController {
public:
    MenuController()  = default;
    ~MenuController() = default;

    /// Display the main menu and dispatch to sub-menus.
    /// Returns when the user chooses Exit.
    void runMainMenu();

    /// Inject the TeamManager (called once during GameApp::init).
    void setTeamManager(TeamManager* mgr);
    void setCharacterRoster(CharacterRoster* roster);
    void setBattleEngine(BattleEngine* engine);

private:
    // --- Main menu ---
    void printHeader() const;
    void printMainMenuOptions() const;

    // --- Sub-menus (placeholder stubs for now) ---
    void showRosterMenu();
    void showTeamMenu();
    void showBattleMenu();
    void showSaveLoadMenu();

    // --- Sub-sub-menu placeholders ---
    void showRosterAdd();
    void showRosterEdit();
    void showRosterDelete();
    void showRosterSearch();

    void showTeamCreate();
    void showTeamRename();
    void showTeamDelete();
    void showTeamAddMember();
    void showTeamRemoveMember();
    void showTeamList();

    void showBattleStart();
    void showSave();
    void showLoad();

    TeamManager* m_teamManager = nullptr;
    CharacterRoster* m_roster = nullptr;
    BattleEngine* m_battleEngine = nullptr;
};

#endif // TURN_BASE_ADVENTURE_MENU_CONTROLLER_HPP
