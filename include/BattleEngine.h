#ifndef BATTLE_ENGINE_H
#define BATTLE_ENGINE_H

#include "Team.hpp"
#include <iostream>
#include <string>

/**
 * Battle states as specified in the assignment:
 * READY -> IN_PROGRESS -> FINISHED
 */
enum class BattleState {
    READY,
    IN_PROGRESS,
    FINISHED
};

/**
 * BattleEngine class responsible for managing turn-based combat between two Teams.
 * Handles state transitions, turn rotation, target validation, polymorphism calls, and victory conditions.
 */
class BattleEngine {
private:
    Team* m_teamA;
    Team* m_teamB;
    BattleState m_state;
    
    int m_currentTeamTurn; // 0: Team A, 1: Team B
    size_t m_actorIndexA;  // Current character index in Team A
    size_t m_actorIndexB;  // Current character index in Team B
    
    Team* m_winningTeam;

    // Helper to find next alive character in a team starting from index
    size_t findNextAliveIndex(const Team* team, size_t startIndex) const;

    // Helper to advance turn to next actor
    void advanceTurn();

    // Helper to reset all characters' HP/Mana in a team
    void resetTeamHpMana(Team* team);

public:
    BattleEngine();
    ~BattleEngine() = default;

    /**
     * Set up battle with two distinct, non-empty teams.
     * Rejects empty teams or identical team pointers (TC-06).
     * @return true on success, false on invalid parameters.
     */
    bool setupBattle(Team* teamA, Team* teamB);

    /**
     * Start the battle.
     * Resets HP/Mana of characters, sets state to IN_PROGRESS, gives 1st turn to Team A (TC-07).
     * @return true on success, false if battle setup is not READY.
     */
    bool start();

    /**
     * Execute a turn action.
     * Validates state, turn actor, and target life status.
     * Rejects out-of-turn actor (TC-08) and dead targets (TC-12) without consuming turn.
     * Executes action via polymorphism (TC-09, TC-10, TC-11).
     * Automatically skips dead actors when advancing turn (TC-13).
     * Detects when a team is wiped out and transitions to FINISHED (TC-14).
     * Rejects actions when battle is FINISHED (TC-15).
     * @return true if action was successfully executed, false if rejected.
     */
    bool executeTurn(Character* actor, Character* target);

    /**
     * Getters
     */
    BattleState getState() const { return m_state; }
    const Team* getTeamA() const { return m_teamA; }
    const Team* getTeamB() const { return m_teamB; }
    const Team* getWinningTeam() const { return m_winningTeam; }

    /**
     * Returns pointer to current active actor whose turn it is.
     * Skips defeated characters (TC-13). Returns nullptr if match is not IN_PROGRESS.
     */
    Character* getCurrentActor();

    /**
     * Returns pointer to the team currently taking its turn.
     */
    const Team* getCurrentActorTeam() const;

    /**
     * Displays battle overview (turn info, HP/Mana of characters, battle state).
     */
    void displayBattleStatus() const;
};

#endif // BATTLE_ENGINE_H
