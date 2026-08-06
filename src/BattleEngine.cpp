#include "BattleEngine.h"
#include <iostream>
#include <iomanip>

BattleEngine::BattleEngine()
    : m_teamA(nullptr),
      m_teamB(nullptr),
      m_state(BattleState::READY),
      m_currentTeamTurn(0),
      m_actorIndexA(0),
      m_actorIndexB(0),
      m_winningTeam(nullptr) {}

bool BattleEngine::setupBattle(Team* teamA, Team* teamB) {
    // TC-06: Reject null team, empty team, or same team pointer
    if (!teamA || !teamB || teamA == teamB) {
        std::cout << "[BattleEngine] Error: Teams must be distinct non-null objects.\n";
        m_state = BattleState::READY;
        return false;
    }

    if (!teamA->hasAliveCharacters() || !teamB->hasAliveCharacters() ||
        teamA->getMembers().empty() || teamB->getMembers().empty()) {
        std::cout << "[BattleEngine] Error: Teams must not be empty and must contain alive characters.\n";
        m_state = BattleState::READY;
        return false;
    }

    m_teamA = teamA;
    m_teamB = teamB;
    m_state = BattleState::READY;
    m_winningTeam = nullptr;
    m_currentTeamTurn = 0;
    m_actorIndexA = 0;
    m_actorIndexB = 0;

    std::cout << "[BattleEngine] Battle setup successful between '" << teamA->getName() 
              << "' and '" << teamB->getName() << "'. Status: READY.\n";
    return true;
}

void BattleEngine::resetTeamHpMana(Team* team) {
    if (!team) return;
    for (auto* character : team->getMembers()) {
        if (character) {
            character->resetHpMana();
        }
    }
}

bool BattleEngine::start() {
    // TC-06 / TC-07 validation
    if (!m_teamA || !m_teamB) {
        std::cout << "[BattleEngine] Error: Cannot start battle. Teams not configured.\n";
        return false;
    }

    if (m_state == BattleState::IN_PROGRESS) {
        std::cout << "[BattleEngine] Warning: Battle is already IN_PROGRESS.\n";
        return true;
    }

    // TC-07: Reset currentHp and currentMana for all characters
    resetTeamHpMana(m_teamA);
    resetTeamHpMana(m_teamB);

    m_state = BattleState::IN_PROGRESS;
    m_currentTeamTurn = 0; // Team A goes first
    m_actorIndexA = 0;
    m_actorIndexB = 0;
    m_winningTeam = nullptr;

    // Locate first alive actor in Team A
    m_actorIndexA = findNextAliveIndex(m_teamA, 0);

    std::cout << "[BattleEngine] Battle Started! HP/Mana reset. Status: IN_PROGRESS.\n";
    Character* firstActor = getCurrentActor();
    if (firstActor) {
        std::cout << "[BattleEngine] First turn belongs to: " << firstActor->getName() 
                  << " (" << m_teamA->getName() << ").\n";
    }

    return true;
}

size_t BattleEngine::findNextAliveIndex(const Team* team, size_t startIndex) const {
    if (!team || team->getMembers().empty()) return 0;

    const auto& members = team->getMembers();
    size_t count = members.size();

    for (size_t i = 0; i < count; ++i) {
        size_t idx = (startIndex + i) % count;
        if (members[idx] && members[idx]->isAlive()) {
            return idx;
        }
    }
    return startIndex; // Default fallback
}

Character* BattleEngine::getCurrentActor() {
    if (m_state != BattleState::IN_PROGRESS) {
        return nullptr;
    }

    Team* currentTeam = (m_currentTeamTurn == 0) ? m_teamA : m_teamB;
    size_t& currentIndex = (m_currentTeamTurn == 0) ? m_actorIndexA : m_actorIndexB;

    if (!currentTeam || !currentTeam->hasAliveCharacters()) {
        return nullptr;
    }

    // TC-13: Skip defeated characters automatically
    const auto& members = currentTeam->getMembers();
    if (currentIndex >= members.size() || !members[currentIndex]->isAlive()) {
        currentIndex = findNextAliveIndex(currentTeam, currentIndex);
    }

    return members[currentIndex];
}

const Team* BattleEngine::getCurrentActorTeam() const {
    if (m_state != BattleState::IN_PROGRESS) return nullptr;
    return (m_currentTeamTurn == 0) ? m_teamA : m_teamB;
}

void BattleEngine::advanceTurn() {
    if (m_state != BattleState::IN_PROGRESS) return;

    // Advance member index in team that just acted
    if (m_currentTeamTurn == 0) {
        m_actorIndexA = (m_actorIndexA + 1) % m_teamA->getMembers().size();
    } else {
        m_actorIndexB = (m_actorIndexB + 1) % m_teamB->getMembers().size();
    }

    // Switch team (0 -> 1, 1 -> 0)
    m_currentTeamTurn = 1 - m_currentTeamTurn;

    // Align active actor in the new team to the next alive member (TC-13)
    Team* newTeam = (m_currentTeamTurn == 0) ? m_teamA : m_teamB;
    size_t& newIndex = (m_currentTeamTurn == 0) ? m_actorIndexA : m_actorIndexB;

    if (newTeam && newTeam->hasAliveCharacters()) {
        newIndex = findNextAliveIndex(newTeam, newIndex);
    }
}

bool BattleEngine::executeTurn(Character* actor, Character* target) {
    // TC-15: Reject action if Battle is FINISHED
    if (m_state == BattleState::FINISHED) {
        std::cout << "[BattleEngine] Action rejected: Battle is FINISHED.\n";
        return false;
    }

    if (m_state != BattleState::IN_PROGRESS) {
        std::cout << "[BattleEngine] Action rejected: Battle is not IN_PROGRESS.\n";
        return false;
    }

    Character* expectedActor = getCurrentActor();

    // TC-08: Reject selection of character when it's not their turn
    if (!actor || actor != expectedActor) {
        std::cout << "[BattleEngine] Action rejected: It is not " 
                  << (actor ? actor->getName() : "Null") << "'s turn! Current actor: " 
                  << (expectedActor ? expectedActor->getName() : "None") << ".\n";
        return false;
    }

    // TC-12: Reject target if null or HP == 0
    if (!target || !target->isAlive()) {
        std::cout << "[BattleEngine] Action rejected: Target " 
                  << (target ? target->getName() : "Null") << " is already defeated (HP=0).\n";
        return false;
    }

    // Execute polymorphic action (TC-09, TC-10, TC-11)
    std::cout << "\n--- TURN EXECUTION ---\n";
    actor->performAction(*target);

    // TC-14: Check win/loss condition
    if (!m_teamA->hasAliveCharacters()) {
        m_state = BattleState::FINISHED;
        m_winningTeam = m_teamB;
        std::cout << "\n=========================================\n";
        std::cout << "[BattleEngine] BATTLE FINISHED! Winner: " << m_teamB->getName() << "\n";
        std::cout << "=========================================\n";
        return true;
    }

    if (!m_teamB->hasAliveCharacters()) {
        m_state = BattleState::FINISHED;
        m_winningTeam = m_teamA;
        std::cout << "\n=========================================\n";
        std::cout << "[BattleEngine] BATTLE FINISHED! Winner: " << m_teamA->getName() << "\n";
        std::cout << "=========================================\n";
        return true;
    }

    // Advance turn to next alive character/team (TC-13)
    advanceTurn();

    Character* nextActor = getCurrentActor();
    if (nextActor) {
        std::cout << "[BattleEngine] Turn complete. Next actor: " << nextActor->getName()
                  << " (" << getCurrentActorTeam()->getName() << ").\n";
    }

    return true;
}

void BattleEngine::displayBattleStatus() const {
    std::cout << "\n================ BATTLE STATUS ================\n";
    std::string stateStr;
    switch (m_state) {
        case BattleState::READY: stateStr = "READY"; break;
        case BattleState::IN_PROGRESS: stateStr = "IN_PROGRESS"; break;
        case BattleState::FINISHED: stateStr = "FINISHED"; break;
    }
    std::cout << "State: " << stateStr << "\n";

    if (m_teamA) {
        std::cout << "\nTeam A: " << m_teamA->getName() << "\n";
        for (const auto* c : m_teamA->getMembers()) {
            if (!c) continue;
            std::cout << "  - [" << c->getId() << "] " << c->getName() 
                      << " (HP: " << c->getCurrentHp() << "/" << c->getMaxHp();
            if (c->getType() == CharacterType::MAGE) {
                const auto* mage = static_cast<const Mage*>(c);
                std::cout << ", Mana: " << mage->getCurrentMana() << "/" << mage->getMaxMana();
            }
            std::cout << ") " << (c->isAlive() ? "[ALIVE]" : "[DEFEATED]") << "\n";
        }
    }

    if (m_teamB) {
        std::cout << "\nTeam B: " << m_teamB->getName() << "\n";
        for (const auto* c : m_teamB->getMembers()) {
            if (!c) continue;
            std::cout << "  - [" << c->getId() << "] " << c->getName() 
                      << " (HP: " << c->getCurrentHp() << "/" << c->getMaxHp();
            if (c->getType() == CharacterType::MAGE) {
                const auto* mage = static_cast<const Mage*>(c);
                std::cout << ", Mana: " << mage->getCurrentMana() << "/" << mage->getMaxMana();
            }
            std::cout << ") " << (c->isAlive() ? "[ALIVE]" : "[DEFEATED]") << "\n";
        }
    }

    if (m_state == BattleState::IN_PROGRESS) {
        const Team* team = getCurrentActorTeam();
        std::cout << "\nCurrent Turn Team: " << (team ? team->getName() : "None") << "\n";
    } else if (m_state == BattleState::FINISHED && m_winningTeam) {
        std::cout << "\nWinner: " << m_winningTeam->getName() << "\n";
    }
    std::cout << "===============================================\n\n";
}
