#include "MenuController.hpp"
#include "InputHandler.hpp"
#include "ICharacterExistenceChecker.hpp"
#include "TeamManager.hpp"
#include "TeamPersistence.hpp"

#include <iostream>

// ─────────────────────────────────────────────────────────────────────────────
//  Helpers
// ─────────────────────────────────────────────────────────────────────────────

namespace {

const char* teamErrorToString(TeamError err) {
    switch (err) {
        case TeamError::None:                  return "Success";
        case TeamError::InvalidId:             return "Team ID must be a positive integer";
        case TeamError::DuplicateId:           return "Team ID already exists";
        case TeamError::EmptyName:             return "Team name cannot be empty";
        case TeamError::DuplicateName:         return "Team name already used by another team";
        case TeamError::TeamNotFound:          return "Team not found";
        case TeamError::CharacterNotInRoster:  return "Character does not exist in the roster";
        case TeamError::DuplicateMember:       return "Character is already a member of this team";
        case TeamError::TeamFull:              return "Team is full (maximum 5 members)";
        case TeamError::MemberNotInTeam:       return "Character is not a member of this team";
    }
    return "Unknown error";
}

/// Placeholder existence checker: lets any character ID through.
/// Replace with a real CharacterRoster when Member-3 delivers it.
class AllowAllExistenceChecker : public ICharacterExistenceChecker {
public:
    bool characterExists(int /*characterId*/) const override { return true; }
};

} // namespace

// ─────────────────────────────────────────────────────────────────────────────
//  Dependency Injection
// ─────────────────────────────────────────────────────────────────────────────

void MenuController::setTeamManager(TeamManager* mgr) {
    m_teamManager = mgr;
}

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
    std::cout << "\n  --- Create New Team ---\n";
    if (m_teamManager == nullptr) {
        std::cout << "  [ERROR] TeamManager not available yet.\n";
        InputHandler::pause();
        return;
    }

    int teamId = InputHandler::getInt("  Enter Team ID (positive integer): ", 1, 2147483647);
    std::string teamName = InputHandler::getString("  Enter Team Name: ", false);

    TeamError result = m_teamManager->createTeam(teamId, teamName);
    if (result == TeamError::None) {
        std::cout << "  [OK] Team '" << teamName << "' (ID: " << teamId << ") created.\n";
    } else {
        std::cout << "  [ERROR] " << teamErrorToString(result) << ".\n";
    }
    InputHandler::pause();
}

void MenuController::showTeamDelete() {
    std::cout << "\n  --- Delete Team ---\n";
    if (m_teamManager == nullptr) {
        std::cout << "  [ERROR] TeamManager not available yet.\n";
        InputHandler::pause();
        return;
    }

    int teamId = InputHandler::getInt("  Enter Team ID to delete: ", 1, 2147483647);
    const Team* team = m_teamManager->findTeamById(teamId);
    if (team == nullptr) {
        std::cout << "  [ERROR] Team not found.\n";
        InputHandler::pause();
        return;
    }

    bool confirm = InputHandler::getYesNo("  Delete team '" + team->name() + "'?");
    if (!confirm) {
        std::cout << "  Cancelled.\n";
        InputHandler::pause();
        return;
    }

    TeamError result = m_teamManager->deleteTeam(teamId);
    if (result == TeamError::None) {
        std::cout << "  [OK] Team deleted.\n";
    } else {
        std::cout << "  [ERROR] " << teamErrorToString(result) << ".\n";
    }
    InputHandler::pause();
}

void MenuController::showTeamAddMember() {
    std::cout << "\n  --- Add Member to Team ---\n";
    if (m_teamManager == nullptr) {
        std::cout << "  [ERROR] TeamManager not available yet.\n";
        InputHandler::pause();
        return;
    }

    int teamId = InputHandler::getInt("  Enter Team ID: ", 1, 2147483647);
    if (!m_teamManager->hasTeam(teamId)) {
        std::cout << "  [ERROR] Team not found.\n";
        InputHandler::pause();
        return;
    }

    const Team* team = m_teamManager->findTeamById(teamId);
    if (team->isFull()) {
        std::cout << "  [ERROR] Team is already full (max 5 members).\n";
        InputHandler::pause();
        return;
    }

    int characterId = InputHandler::getInt("  Enter Character ID to add: ", 1, 2147483647);

    AllowAllExistenceChecker checker;
    TeamError result = m_teamManager->addCharacterToTeam(teamId, characterId, checker);
    if (result == TeamError::None) {
        std::cout << "  [OK] Character " << characterId << " added to team '" << team->name() << "'.\n";
    } else {
        std::cout << "  [ERROR] " << teamErrorToString(result) << ".\n";
    }
    InputHandler::pause();
}

void MenuController::showTeamRemoveMember() {
    std::cout << "\n  --- Remove Member from Team ---\n";
    if (m_teamManager == nullptr) {
        std::cout << "  [ERROR] TeamManager not available yet.\n";
        InputHandler::pause();
        return;
    }

    int teamId = InputHandler::getInt("  Enter Team ID: ", 1, 2147483647);
    const Team* team = m_teamManager->findTeamById(teamId);
    if (team == nullptr) {
        std::cout << "  [ERROR] Team not found.\n";
        InputHandler::pause();
        return;
    }

    if (team->isEmpty()) {
        std::cout << "  Team '" << team->name() << "' has no members.\n";
        InputHandler::pause();
        return;
    }

    int characterId = InputHandler::getInt("  Enter Character ID to remove: ", 1, 2147483647);

    TeamError result = m_teamManager->removeCharacterFromTeam(teamId, characterId);
    if (result == TeamError::None) {
        std::cout << "  [OK] Character " << characterId << " removed from team '" << team->name() << "'.\n";
    } else {
        std::cout << "  [ERROR] " << teamErrorToString(result) << ".\n";
    }
    InputHandler::pause();
}

void MenuController::showTeamList() {
    std::cout << "\n  --- All Teams ---\n";
    if (m_teamManager == nullptr) {
        std::cout << "  [ERROR] TeamManager not available yet.\n";
        InputHandler::pause();
        return;
    }

    const std::vector<Team>& teams = m_teamManager->teams();
    if (teams.empty()) {
        std::cout << "  No teams exist yet.\n";
    } else {
        for (const Team& team : teams) {
            std::cout << "  Team #" << team.id() << " | " << team.name()
                      << " | " << team.memberCount() << " member(s)";
            const std::vector<int>& ids = team.memberIds();
            if (!ids.empty()) {
                std::cout << " | IDs: ";
                for (std::size_t i = 0; i < ids.size(); ++i) {
                    if (i > 0) std::cout << ", ";
                    std::cout << ids[i];
                }
            }
            std::cout << '\n';
        }
    }
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
    std::cout << "\n  --- Save All Data ---\n";
    if (m_teamManager == nullptr) {
        std::cout << "  [ERROR] TeamManager not available yet.\n";
        InputHandler::pause();
        return;
    }

    if (TeamPersistence::saveTeams("data/teams.txt", *m_teamManager)) {
        std::cout << "  [OK] Team data saved successfully.\n";
    } else {
        std::cout << "  [ERROR] Failed to save team data.\n";
    }
    InputHandler::pause();
}

void MenuController::showLoad() {
    std::cout << "\n  --- Load All Data ---\n";
    if (m_teamManager == nullptr) {
        std::cout << "  [ERROR] TeamManager not available yet.\n";
        InputHandler::pause();
        return;
    }

    std::cout << "  This will reload team data from file and discard\n"
              << "  any unsaved changes. Continue?\n";
    if (!InputHandler::getYesNo("  Reload")) {
        return;
    }

    // Collect and delete all current teams before reloading
    std::vector<int> idsToDelete;
    for (const Team& t : m_teamManager->teams()) {
        idsToDelete.push_back(t.id());
    }
    for (int id : idsToDelete) {
        m_teamManager->deleteTeam(id);
    }

    TeamPersistence::loadTeams("data/teams.txt", *m_teamManager);

    std::cout << "  [OK] Team data reloaded. " << m_teamManager->teams().size()
              << " teams loaded.\n";
    InputHandler::pause();
}
