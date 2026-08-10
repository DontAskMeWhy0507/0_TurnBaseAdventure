#include "../include/Warrior.hpp"
#include <iostream>

Warrior::Warrior(int id, const std::string& name, int maxHp, int attackPower)
    : Character(id, name, maxHp, "WARRIOR"), attackPower(attackPower) {}

int Warrior::getAttackPower() const {
    return attackPower;
}

void Warrior::attack(Character& target) {
    std::cout << getName() << " (WARRIOR) chem " << target.getName()
              << " gay " << attackPower << " sat thuong!\n";
    target.takeDamage(attackPower);
}

void Warrior::performAction(Character& target) {
    attack(target);
}