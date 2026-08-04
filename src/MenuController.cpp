#include "MenuController.hpp"
#include "InputHandler.hpp"

#include <iostream>

// ─────────────────────────────────────────────────────────────────────────────
//  Main Menu
// ─────────────────────────────────────────────────────────────────────────────

void MenuController::printHeader() const {
    std::cout << '\n'
              << "  ========================================\n"
              << "     TURN-BASED ADVENTURE GAME\n"
              << "  ========================================\n";
}

void MenuController::printMainMenuOptions() const {
    std::cout << "     1. Character Roster Management\n"
              << "     2. Team Management\n"
              << "     3. Battle\n"
              << "     4. Save & Load Data\n"
              << "     0. Exit\n"
              << "  ----------------------------------------\n";
}

void MenuController::runMainMenu() {
    bool inMenu = true;
    while (inMenu) {
        printHeader();
        printMainMenuOptions();

        int choice = InputHandler::getInt("  Enter your choice: ", 0, 4);

        switch (choice) {
            case 0:
                std::cout << "\n  Goodbye!\n" << std::endl;
                inMenu = false;
                break;
            case 1: showRosterMenu();    break;
            case 2: showTeamMenu();      break;
            case 3: showBattleMenu();    break;
            case 4: showSaveLoadMenu();  break;
        }
    }
}

// ─────────────────────────────────────────────────────────────────────────────
//  1. Character Roster Sub-Menu
// ─────────────────────────────────────────────────────────────────────────────

void MenuController::showRosterMenu() {
    bool inMenu = true;
    while (inMenu) {
        std::cout << '\n'
                  << "  === CHARACTER ROSTER MANAGEMENT ===\n"
                  << "     1. Add Character\n"
                  << "     2. Edit Character\n"
                  << "     3. Delete Character\n"
                  << "     4. Search Character\n"
                  << "     5. List All Characters\n"
                  << "     0. Back to Main Menu\n"
                  << "  -----------------------------------\n";

        int choice = InputHandler::getInt("  Enter your choice: ", 0, 5);
        switch (choice) {
            case 0: inMenu = false; break;
            case 1: showRosterAdd();     break;
            case 2: showRosterEdit();    break;
            case 3: showRosterDelete();  break;
            case 4: showRosterSearch();  break;
            case 5:
                std::cout << "\n  [TODO] List all characters — coming soon.\n";
                InputHandler::pause();
                break;
        }
    }
}

void MenuController::showRosterAdd() {
    std::cout << "\n  [TODO] Add Character — waiting for Member-3 (CharacterRoster).\n";
    InputHandler::pause();
}

void MenuController::showRosterEdit() {
    std::cout << "\n  [TODO] Edit Character — waiting for Member-3 (CharacterRoster).\n";
    InputHandler::pause();
}

void MenuController::showRosterDelete() {
    std::cout << "\n  [TODO] Delete Character — waiting for Member-3 (CharacterRoster).\n";
    InputHandler::pause();
}

void MenuController::showRosterSearch() {
    std::cout << "\n  [TODO] Search Character — waiting for Member-3 (CharacterRoster).\n";
    InputHandler::pause();
}

// ─────────────────────────────────────────────────────────────────────────────
//  2. Team Management Sub-Menu
// ─────────────────────────────────────────────────────────────────────────────

void MenuController::showTeamMenu() {
    bool inMenu = true;
    while (inMenu) {
        std::cout << '\n'
                  << "  === TEAM MANAGEMENT ===\n"
                  << "     1. Create Team\n"
                  << "     2. Delete Team\n"
                  << "     3. Add Member to Team\n"
                  << "     4. Remove Member from Team\n"
                  << "     5. List All Teams\n"
                  << "     0. Back to Main Menu\n"
                  << "  -----------------------\n";

        int choice = InputHandler::getInt("  Enter your choice: ", 0, 5);
        switch (choice) {
            case 0: inMenu = false; break;
            case 1: showTeamCreate();       break;
            case 2: showTeamDelete();       break;
            case 3: showTeamAddMember();    break;
            case 4: showTeamRemoveMember(); break;
            case 5: showTeamList();         break;
        }
    }
}

void MenuController::showTeamCreate() {
    std::cout << "\n  [TODO] Create Team — waiting for Member-3 (TeamManager).\n";
    InputHandler::pause();
}

void MenuController::showTeamDelete() {
    std::cout << "\n  [TODO] Delete Team — waiting for Member-3 (TeamManager).\n";
    InputHandler::pause();
}

void MenuController::showTeamAddMember() {
    std::cout << "\n  [TODO] Add Member to Team — waiting for Member-3 (TeamManager).\n";
    InputHandler::pause();
}

void MenuController::showTeamRemoveMember() {
    std::cout << "\n  [TODO] Remove Member from Team — waiting for Member-3 (TeamManager).\n";
    InputHandler::pause();
}

void MenuController::showTeamList() {
    std::cout << "\n  [TODO] List All Teams — waiting for Member-3 (TeamManager).\n";
    InputHandler::pause();
}

// ─────────────────────────────────────────────────────────────────────────────
//  3. Battle Sub-Menu
// ─────────────────────────────────────────────────────────────────────────────

void MenuController::showBattleMenu() {
    bool inMenu = true;
    while (inMenu) {
        std::cout << '\n'
                  << "  === BATTLE ===\n"
                  << "     1. Start Battle\n"
                  << "     0. Back to Main Menu\n"
                  << "  ---------------\n";

        int choice = InputHandler::getInt("  Enter your choice: ", 0, 1);
        switch (choice) {
            case 0: inMenu = false; break;
            case 1: showBattleStart(); break;
        }
    }
}

void MenuController::showBattleStart() {
    std::cout << "\n  [TODO] Start Battle — waiting for Member-4 (BattleEngine).\n";
    InputHandler::pause();
}

// ─────────────────────────────────────────────────────────────────────────────
//  4. Save & Load Sub-Menu
// ─────────────────────────────────────────────────────────────────────────────

void MenuController::showSaveLoadMenu() {
    bool inMenu = true;
    while (inMenu) {
        std::cout << '\n'
                  << "  === SAVE & LOAD DATA ===\n"
                  << "     1. Save All Data\n"
                  << "     2. Load All Data\n"
                  << "     0. Back to Main Menu\n"
                  << "  ------------------------\n";

        int choice = InputHandler::getInt("  Enter your choice: ", 0, 2);
        switch (choice) {
            case 0: inMenu = false; break;
            case 1: showSave(); break;
            case 2: showLoad(); break;
        }
    }
}

void MenuController::showSave() {
    std::cout << "\n  [TODO] Save All Data — waiting for Member-4 (PersistenceManager).\n";
    InputHandler::pause();
}

void MenuController::showLoad() {
    std::cout << "\n  [TODO] Load All Data — waiting for Member-4 (PersistenceManager).\n";
    InputHandler::pause();
}
