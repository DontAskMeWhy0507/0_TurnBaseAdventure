#ifndef TURN_BASE_ADVENTURE_MENU_CONTROLLER_HPP
#define TURN_BASE_ADVENTURE_MENU_CONTROLLER_HPP

/**
 * Console-based menu system for the Turn-Based Adventure game.
 *
 * Owns the navigation logic for main + sub-menus.
 * Sub-menus are placeholders until Member-3 (Roster/Team) and
 * Member-4 (Battle/FileIO) deliver their modules.
 */
class MenuController {
public:
    MenuController()  = default;
    ~MenuController() = default;

    /// Display the main menu and dispatch to sub-menus.
    /// Returns when the user chooses Exit.
    void runMainMenu();

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
    void showTeamDelete();
    void showTeamAddMember();
    void showTeamRemoveMember();
    void showTeamList();

    void showBattleStart();
    void showSave();
    void showLoad();
};

#endif // TURN_BASE_ADVENTURE_MENU_CONTROLLER_HPP
