#include "MenuController.hpp"
#include "InputHandler.hpp"
#include "ICharacterExistenceChecker.hpp"
#include "TeamManager.hpp"
#include "TeamPersistence.hpp"
#include "CharacterRoster.hpp"
#include "BattleEngine.h"
#include "Team.hpp"

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

} // namespace

// ─────────────────────────────────────────────────────────────────────────────
//  Dependency Injection
// ─────────────────────────────────────────────────────────────────────────────

void MenuController::setTeamManager(TeamManager* mgr) {
    m_teamManager = mgr;
}

void MenuController::setCharacterRoster(CharacterRoster* roster) {
    m_roster = roster;
}

void MenuController::setBattleEngine(BattleEngine* engine) {
    m_battleEngine = engine;
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
                if (!m_roster || m_roster->getCharacterCount() == 0) {
                    std::cout << "\n  No characters in roster.\n";
                } else {
                    std::cout << "\n  --- All Characters ---\n";
                    for (const auto& item : m_roster->getAllCharacters()) {
                        std::cout << "  [" << item->getId() << "] " << item->getName()
                                  << " | " << item->getTypeName() << " | "
                                  << item->getStatsString() << '\n';
                    }
                }
                InputHandler::pause();
                break;
        }
    }
}

void MenuController::showRosterAdd() {
    if (!m_roster) { std::cout << "\n  [ERROR] CharacterRoster not available.\n"; InputHandler::pause(); return; }
    std::cout << "\n  --- Add Character ---\n"
              << "     1. Warrior\n     2. Mage\n";
    int type = InputHandler::getInt("  Character type: ", 1, 2);
    int id = InputHandler::getInt("  ID: ", 1, 2147483647);
    std::string name = InputHandler::getString("  Name: ");
    int hp = InputHandler::getInt("  Max HP: ", 1, 2147483647);
    std::unique_ptr<Character> character;
    if (type == 1) {
        int attack = InputHandler::getInt("  Attack power: ", 1, 2147483647);
        character.reset(new Warrior(id, name, hp, attack));
    } else {
        int maxMana = InputHandler::getInt("  Max mana: ", 1, 2147483647);
        int spell = InputHandler::getInt("  Spell damage: ", 1, 2147483647);
        int cost = InputHandler::getInt("  Mana cost: ", 1, 2147483647);
        int fallback = InputHandler::getInt("  Fallback damage: ", 1, 2147483647);
        character.reset(new Mage(id, name, hp, maxMana, spell, cost, fallback));
    }
    std::cout << (m_roster->addCharacter(std::move(character))
        ? "  [OK] Character added.\n" : "  [ERROR] Character ID already exists or data is invalid.\n");
    InputHandler::pause();
}

void MenuController::showRosterEdit() {
    if (!m_roster) { std::cout << "\n  [ERROR] CharacterRoster not available.\n"; InputHandler::pause(); return; }
    int id = InputHandler::getInt("  Character ID to edit: ", 1, 2147483647);
    if (!m_roster->hasCharacter(id)) { std::cout << "  [ERROR] Character not found.\n"; InputHandler::pause(); return; }
    std::string name = InputHandler::getString("  Name: ");
    int hp = InputHandler::getInt("  Max HP: ", 1, 2147483647);
    std::unique_ptr<Character> replacement;
    const Character* original = m_roster->getCharacterById(id);
    if (dynamic_cast<const Warrior*>(original)) {
        int attack = InputHandler::getInt("  Attack power: ", 1, 2147483647);
        replacement.reset(new Warrior(id, name, hp, attack));
    } else {
        int maxMana = InputHandler::getInt("  Max mana: ", 1, 2147483647);
        int spell = InputHandler::getInt("  Spell damage: ", 1, 2147483647);
        int cost = InputHandler::getInt("  Mana cost: ", 1, 2147483647);
        int fallback = InputHandler::getInt("  Fallback damage: ", 1, 2147483647);
        replacement.reset(new Mage(id, name, hp, maxMana, spell, cost, fallback));
    }
    std::cout << (m_roster->replaceCharacter(std::move(replacement))
        ? "  [OK] Character updated.\n" : "  [ERROR] Update failed.\n");
    InputHandler::pause();
}

void MenuController::showRosterDelete() {
    if (!m_roster || !m_teamManager) { std::cout << "\n  [ERROR] Managers not available.\n"; InputHandler::pause(); return; }
    int id = InputHandler::getInt("  Character ID to delete: ", 1, 2147483647);
    if (!m_roster->hasCharacter(id)) { std::cout << "  [ERROR] Character not found.\n"; InputHandler::pause(); return; }
    if (!InputHandler::getYesNo("  Delete this character")) return;
    m_teamManager->removeCharacterFromAllTeams(id);
    std::cout << (m_roster->removeCharacterById(id)
        ? "  [OK] Character deleted and removed from all teams.\n" : "  [ERROR] Delete failed.\n");
    InputHandler::pause();
}

void MenuController::showRosterSearch() {
    if (!m_roster) { std::cout << "\n  [ERROR] CharacterRoster not available.\n"; InputHandler::pause(); return; }
    std::cout << "  1. Find by ID\n  2. Find by name\n";
    int mode = InputHandler::getInt("  Search mode: ", 1, 2);
    if (mode == 1) {
        int id = InputHandler::getInt("  Character ID: ", 1, 2147483647);
        const Character* c = m_roster->getCharacterById(id);
        if (!c) std::cout << "  [ERROR] Character not found.\n";
        else std::cout << "  [" << c->getId() << "] " << c->getName() << " " << c->getTypeName()
                        << " | " << c->getStatsString() << "\n";
    } else {
        std::string query = InputHandler::getString("  Name search: ");
        const CharacterRoster& readOnlyRoster = *m_roster;
        const std::vector<const Character*> matches = readOnlyRoster.findByName(query);
        if (matches.empty()) {
            std::cout << "  [ERROR] No characters match '" << query << "'.\n";
        } else for (const Character* c : matches) {
            std::cout << "  [" << c->getId() << "] " << c->getName() << " " << c->getTypeName()
                      << " | " << c->getStatsString() << "\n";
        }
    }
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
                  << "     2. Rename Team\n"
                  << "     3. Delete Team\n"
                  << "     4. Add Member to Team\n"
                  << "     5. Remove Member from Team\n"
                  << "     6. List All Teams\n"
                  << "     0. Back to Main Menu\n"
                  << "  -----------------------\n";

        int choice = InputHandler::getInt("  Enter your choice: ", 0, 6);
        switch (choice) {
            case 0: inMenu = false; break;
            case 1: showTeamCreate();       break;
            case 2: showTeamRename();       break;
            case 3: showTeamDelete();       break;
            case 4: showTeamAddMember();    break;
            case 5: showTeamRemoveMember(); break;
            case 6: showTeamList();         break;
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

void MenuController::showTeamRename() {
    std::cout << "\n  --- Rename Team ---\n";
    if (!m_teamManager) {
        std::cout << "  [ERROR] TeamManager not available yet.\n";
        InputHandler::pause();
        return;
    }
    int teamId = InputHandler::getInt("  Team ID: ", 1, 2147483647);
    if (!m_teamManager->hasTeam(teamId)) {
        std::cout << "  [ERROR] Team not found.\n";
        InputHandler::pause();
        return;
    }
    std::string newName = InputHandler::getString("  New Team Name: ");
    TeamError result = m_teamManager->renameTeam(teamId, newName);
    std::cout << (result == TeamError::None
        ? "  [OK] Team renamed.\n"
        : std::string("  [ERROR] ") + teamErrorToString(result) + ".\n");
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

    if (!m_roster) {
        std::cout << "  [ERROR] CharacterRoster not available.\n";
        InputHandler::pause();
        return;
    }
    TeamError result = m_teamManager->addCharacterToTeam(teamId, characterId, *m_roster);
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
    if (!m_battleEngine || !m_teamManager || !m_roster) {
        std::cout << "\n  [ERROR] Battle dependencies are not available.\n";
        InputHandler::pause();
        return;
    }
    if (m_teamManager->teams().size() < 2) {
        std::cout << "\n  [ERROR] Create at least two teams first.\n";
        InputHandler::pause();
        return;
    }
    std::cout << "\n  --- Select Teams ---\n";
    for (const Team& team : m_teamManager->teams()) {
        std::cout << "  " << team.id() << ": " << team.name()
                  << " (" << team.memberCount() << " member(s))\n";
    }
    int teamAId = InputHandler::getInt("  Team A ID: ", 1, 2147483647);
    int teamBId = InputHandler::getInt("  Team B ID: ", 1, 2147483647);
    const Team* sourceA = m_teamManager->findTeamById(teamAId);
    const Team* sourceB = m_teamManager->findTeamById(teamBId);
    if (!sourceA || !sourceB || sourceA == sourceB) {
        std::cout << "  [ERROR] Teams must be distinct and exist.\n";
        InputHandler::pause();
        return;
    }

    Team battleA(sourceA->id(), sourceA->name());
    Team battleB(sourceB->id(), sourceB->name());
    for (int id : sourceA->memberIds()) battleA.addCharacter(m_roster->getCharacterById(id));
    for (int id : sourceB->memberIds()) battleB.addCharacter(m_roster->getCharacterById(id));
    if (!m_battleEngine->setupBattle(&battleA, &battleB) || !m_battleEngine->start()) {
        std::cout << "  [ERROR] Battle requires two non-empty teams with alive characters.\n";
        InputHandler::pause();
        return;
    }

    while (m_battleEngine->getState() == BattleState::IN_PROGRESS) {
        m_battleEngine->displayBattleStatus();
        Character* actor = m_battleEngine->getCurrentActor();
        const Team* targetTeam = m_battleEngine->getCurrentActorTeam() == &battleA ? &battleB : &battleA;
        std::cout << "\n  Current actor: " << actor->getName() << "\n  Targets:\n";
        for (Character* target : targetTeam->getMembers()) {
            if (target) std::cout << "    " << target->getId() << ": " << target->getName()
                                   << " (" << target->getStatsString() << ")\n";
        }
        int targetId = InputHandler::getInt("  Target ID (0 to quit battle): ", 0, 2147483647);
        if (targetId == 0) break;
        Character* target = m_roster->getCharacterById(targetId);
        if (!m_battleEngine->executeTurn(actor, target)) {
            std::cout << "  [ERROR] Invalid actor or target. Try again.\n";
        }
    }
    if (m_battleEngine->getState() == BattleState::FINISHED && m_battleEngine->getWinningTeam()) {
        std::cout << "\n  Winner: " << m_battleEngine->getWinningTeam()->getName() << "\n";
    }
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
    if (m_roster && m_roster->saveToFile("data/characters.txt")) {
        std::cout << "  [OK] Character data saved successfully.\n";
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

    if (m_roster) {
        m_roster->loadFromFile("data/characters.txt");
        // Reload teams once more with the real roster checker so invalid IDs
        // cannot enter the application state.
        std::vector<int> reloadedIds;
        for (const Team& team : m_teamManager->teams()) reloadedIds.push_back(team.id());
        for (int id : reloadedIds) m_teamManager->deleteTeam(id);
        TeamPersistence::loadTeams("data/teams.txt", *m_teamManager, m_roster);
    }

    std::cout << "  [OK] Team data reloaded. " << m_teamManager->teams().size()
              << " teams loaded.\n";
    InputHandler::pause();
}
