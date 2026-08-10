#include <cassert>
#include <iostream>

#include "../include/Mage.hpp"
#include "../include/Warrior.hpp"

static void check(bool condition, const char* message) {
    if (!condition) {
        std::cerr << "Test failed: " << message << "\n";
        std::exit(EXIT_FAILURE);
    }
}

void testCharacterInitialization() {
    Warrior warrior(1, "Arthur", 100, 25);
    check(warrior.getId() == 1, "Warrior ID should be initialized correctly");
    check(warrior.getName() == "Arthur", "Warrior name should be initialized correctly");
    check(warrior.getMaxHp() == 100, "Warrior max HP should be initialized correctly");
    check(warrior.getCurrentHp() == 100, "Warrior current HP should start at max HP");
    check(warrior.getType() == "WARRIOR", "Warrior type should be WARRIOR");
    check(warrior.isAlive(), "Warrior should be alive after initialization");
}

void testMageSpellAndFallback() {
    Mage mage(1, "Merlin", 100, 50, 25, 20, 10);
    Warrior target(2, "Conan", 120, 15);

    int hpBefore = target.getCurrentHp();
    mage.performAction(target);
    check(target.getCurrentHp() == hpBefore - 25, "Mage spell should deal 25 damage on first cast");

    hpBefore = target.getCurrentHp();
    mage.performAction(target);
    check(target.getCurrentHp() == hpBefore - 25, "Mage second cast should deal 25 damage when mana remains");

    hpBefore = target.getCurrentHp();
    mage.performAction(target);
    check(target.getCurrentHp() == hpBefore - 10, "Mage should fallback deal 10 damage when mana is insufficient");
}

void testMageResetSessionRestoresHpAndMana() {
    Mage mage(1, "Merlin", 100, 40, 25, 20, 10);
    Warrior target(2, "Conan", 80, 15);

    mage.performAction(target); // mana -> 20
    mage.performAction(target); // mana -> 0
    mage.performAction(target); // fallback, mana remains 0
    int hpBefore = target.getCurrentHp();

    mage.takeDamage(50);
    check(mage.getCurrentHp() == 50, "Mage HP should drop after damage");

    mage.resetSession();
    check(mage.getCurrentHp() == mage.getMaxHp(), "Mage resetSession should restore HP to max");

    int targetHpAfterReset = target.getCurrentHp();
    mage.performAction(target);
    check(target.getCurrentHp() == targetHpAfterReset - 25,
          "Mage resetSession should restore mana so next action is a spell, not fallback");
}

void testWarriorAttackAndResetSession() {
    Warrior warrior(1, "Conan", 80, 30);
    Mage target(2, "Gandalf", 70, 30, 10, 5, 3);

    int hpBefore = target.getCurrentHp();
    warrior.performAction(target);
    check(target.getCurrentHp() == hpBefore - 30, "Warrior attack should deal attackPower damage");

    warrior.takeDamage(50);
    check(warrior.getCurrentHp() == 30, "Warrior HP should drop after damage");
    warrior.resetSession();
    check(warrior.getCurrentHp() == warrior.getMaxHp(), "Warrior resetSession should restore HP to max");
}

void testCharacterTakeDamageNoNegative() {
    Warrior warrior(1, "Hercules", 90, 20);
    int hpBefore = warrior.getCurrentHp();
    warrior.takeDamage(-10);
    check(warrior.getCurrentHp() == hpBefore, "takeDamage with negative value should not change HP");
}

void testCharacterDeathBehavior() {
    Mage mage(1, "Morgana", 60, 30, 15, 10, 5);
    mage.takeDamage(100);
    check(mage.getCurrentHp() == 0, "HP should not be negative after lethal damage");
    check(!mage.isAlive(), "Character should be dead when HP is zero");
}

int main() {
    std::cout << "Running OOP tests...\n";
    testCharacterInitialization();
    testMageSpellAndFallback();
    testMageResetSessionRestoresHpAndMana();
    testWarriorAttackAndResetSession();
    testCharacterTakeDamageNoNegative();
    testCharacterDeathBehavior();
    std::cout << "All OOP tests passed.\n";
    return 0;
}
