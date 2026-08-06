#include "BattleEngine.h"
#include <iostream>
#include <cassert>

void runTestCases() {
    std::cout << "==================================================\n";
    std::cout << "    RUNNING BATTLE ENGINE AUTOMATED TEST SUITE    \n";
    std::cout << "==================================================\n\n";

    // Setup characters according to assignment examples
    // Ares: Warrior (ID: 101, HP: 100, attackPower: 30)
    Warrior ares(101, "Ares", 100, 30);
    // Luna: Mage (ID: 102, HP: 80, mana: 10, spellDamage: 40, manaCost: 10, fallbackDamage: 10)
    Mage luna(102, "Luna", 80, 10, 40, 10, 10);
    // Extra dummy warrior for multi-character testing
    Warrior thor(103, "Thor", 100, 25);

    // ----------------------------------------------------
    // TC-06: Start with empty team or same team
    // ----------------------------------------------------
    std::cout << "\n>>> [TC-06] Test empty team / same team setup\n";
    BattleEngine engine;
    Team emptyTeam(1, "Empty Team");
    Team redTeam(201, "Red Team");
    redTeam.addCharacter(&ares);

    // Same team test
    bool setupResult1 = engine.setupBattle(&redTeam, &redTeam);
    assert(!setupResult1);
    assert(engine.getState() == BattleState::READY);

    // Empty team test
    bool setupResult2 = engine.setupBattle(&redTeam, &emptyTeam);
    assert(!setupResult2);
    assert(engine.getState() == BattleState::READY);
    std::cout << "==> PASS: TC-06 Rejected properly, state remains READY.\n";

    // ----------------------------------------------------
    // TC-07: Start Red Team (Ares) and Blue Team (Luna)
    // ----------------------------------------------------
    std::cout << "\n>>> [TC-07] Start battle with Red Team (Ares) vs Blue Team (Luna)\n";
    Team blueTeam(202, "Blue Team");
    blueTeam.addCharacter(&luna);

    bool setupOk = engine.setupBattle(&redTeam, &blueTeam);
    assert(setupOk);
    assert(engine.getState() == BattleState::READY);

    // Damage characters to verify reset upon start
    ares.takeDamage(50);
    bool startOk = engine.start();
    assert(startOk);
    assert(engine.getState() == BattleState::IN_PROGRESS);
    assert(ares.getCurrentHp() == 100); // HP reset
    assert(luna.getCurrentHp() == 80);  // HP reset
    assert(luna.getCurrentMana() == 10); // Mana reset
    assert(engine.getCurrentActor() == &ares); // Ares has first turn
    std::cout << "==> PASS: TC-07 Battle started, HP/Mana reset, Ares has 1st turn.\n";

    // ----------------------------------------------------
    // TC-08: Select Luna action when it's Ares's turn
    // ----------------------------------------------------
    std::cout << "\n>>> [TC-08] Select Luna action when it is Ares's turn\n";
    bool wrongTurnOk = engine.executeTurn(&luna, &ares);
    assert(!wrongTurnOk);
    assert(engine.getCurrentActor() == &ares); // Turn unchanged
    assert(ares.getCurrentHp() == 100);       // HP unchanged
    std::cout << "==> PASS: TC-08 Wrong turn action rejected properly.\n";

    // ----------------------------------------------------
    // TC-09: Ares (Warrior 30 AP) attacks Luna (80 HP)
    // ----------------------------------------------------
    std::cout << "\n>>> [TC-09] Ares attacks Luna (Warrior performAction)\n";
    bool turn1Ok = engine.executeTurn(&ares, &luna);
    assert(turn1Ok);
    assert(luna.getCurrentHp() == 50); // 80 - 30 = 50
    assert(engine.getCurrentActor() == &luna); // Turn switched to Luna
    std::cout << "==> PASS: TC-09 Luna HP is now 50. Warrior::performAction() executed.\n";

    // ----------------------------------------------------
    // TC-10: Luna (Mage, 10 mana, spell 40, cost 10) attacks Ares
    // ----------------------------------------------------
    std::cout << "\n>>> [TC-10] Luna attacks Ares with spell (Mage performAction)\n";
    bool turn2Ok = engine.executeTurn(&luna, &ares);
    assert(turn2Ok);
    assert(ares.getCurrentHp() == 60);  // 100 - 40 = 60
    assert(luna.getCurrentMana() == 0); // 10 - 10 = 0
    assert(engine.getCurrentActor() == &ares); // Turn switched to Ares
    std::cout << "==> PASS: TC-10 Ares HP is now 60, Luna mana is 0. Mage spell executed.\n";

    // ----------------------------------------------------
    // TC-11: Luna (0 mana, fallback 10) attacks Ares
    // ----------------------------------------------------
    // Make it Luna's turn again by performing Ares turn first
    std::cout << "\n>>> [TC-11] Testing Mage fallback damage when mana is 0\n";
    engine.executeTurn(&ares, &luna); // Ares attacks Luna: 50 - 30 = 20 HP
    assert(luna.getCurrentHp() == 20);
    assert(engine.getCurrentActor() == &luna); // Luna's turn

    bool turn3Ok = engine.executeTurn(&luna, &ares); // Luna attacks with 0 mana
    assert(turn3Ok);
    assert(ares.getCurrentHp() == 50);  // 60 - 10 = 50
    assert(luna.getCurrentMana() == 0); // Mana stays 0
    std::cout << "==> PASS: TC-11 Ares HP reduced by fallback 10 (now 50), Luna mana remains 0.\n";

    // ----------------------------------------------------
    // TC-12: Target defeated character (HP == 0)
    // ----------------------------------------------------
    std::cout << "\n>>> [TC-12] Attempt to target a character with 0 HP\n";
    // Ares attacks Luna: 20 - 30 = 0 (Luna defeated)
    engine.executeTurn(&ares, &luna);
    assert(luna.getCurrentHp() == 0);
    assert(!luna.isAlive());

    // Battle should transition to FINISHED since Blue Team is wiped out
    assert(engine.getState() == BattleState::FINISHED);
    assert(engine.getWinningTeam() == &redTeam);

    // Try targeting dead Luna in a new battle setup to test TC-12 explicit rejection
    Team teamX(301, "Team X");
    Team teamY(302, "Team Y");
    Warrior hero(1, "Hero", 100, 50);
    Warrior dummy1(2, "Dummy 1", 100, 50);
    Warrior dummy2(3, "Dummy 2", 100, 50);

    teamX.addCharacter(&hero);
    teamY.addCharacter(&dummy1);
    teamY.addCharacter(&dummy2);

    BattleEngine engine2;
    engine2.setupBattle(&teamX, &teamY);
    engine2.start();

    // Defeat dummy1 manually
    dummy1.takeDamage(100);
    assert(!dummy1.isAlive());

    // Hero attempts to execute turn on defeated dummy1
    bool deadTargetOk = engine2.executeTurn(&hero, &dummy1);
    assert(!deadTargetOk);
    assert(engine2.getCurrentActor() == &hero); // Turn NOT consumed
    std::cout << "==> PASS: TC-12 Target with HP 0 rejected, turn not consumed.\n";

    // ----------------------------------------------------
    // TC-13: Turn rotation automatically skips dead characters
    // ----------------------------------------------------
    std::cout << "\n>>> [TC-13] Automatic skipping of defeated characters during turn rotation\n";
    // Team Y has dummy1 (dead) and dummy2 (alive)
    // Hero attacks dummy2
    bool attackOk = engine2.executeTurn(&hero, &dummy2);
    assert(attackOk);
    // Team Y turn: dummy1 is dead, so engine2 should automatically pick dummy2 as active actor!
    assert(engine2.getCurrentActor() == &dummy2);
    std::cout << "==> PASS: TC-13 Defeated dummy1 skipped, active actor is dummy2.\n";

    // ----------------------------------------------------
    // TC-14: One Team has no alive characters -> FINISHED & Winner
    // ----------------------------------------------------
    std::cout << "\n>>> [TC-14] Battle transition to FINISHED when team wiped out\n";
    // dummy2 attacks hero
    engine2.executeTurn(&dummy2, &hero);
    // hero finishes dummy2: 100 - 50 = 50 -> 50 - 50 = 0
    engine2.executeTurn(&hero, &dummy2);

    assert(!teamY.hasAliveCharacters());
    assert(engine2.getState() == BattleState::FINISHED);
    assert(engine2.getWinningTeam() == &teamX);
    std::cout << "==> PASS: TC-14 State is FINISHED, winning team is Team X.\n";

    // ----------------------------------------------------
    // TC-15: Call action after Battle is FINISHED
    // ----------------------------------------------------
    std::cout << "\n>>> [TC-15] Action called after Battle is FINISHED\n";
    bool actionAfterFinished = engine2.executeTurn(&hero, &dummy2);
    assert(!actionAfterFinished);
    assert(engine2.getState() == BattleState::FINISHED);
    std::cout << "==> PASS: TC-15 Action after FINISHED rejected, state maintained.\n";

    std::cout << "\n==================================================\n";
    std::cout << "    ALL BATTLE ENGINE TEST CASES PASSED (100%)    \n";
    std::cout << "==================================================\n";
}

int main() {
    runTestCases();
    return 0;
}
