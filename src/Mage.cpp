#include "../include/Mage.hpp"
#include <iostream>

Mage::Mage(int id, const std::string& name, int maxHp, int maxMana, int spellDamage, int manaCost, int fallbackDamage)
    : Character(id, name, maxHp, "MAGE"), maxMana(maxMana), currentMana(maxMana),
      spellDamage(spellDamage), manaCost(manaCost), fallbackDamage(fallbackDamage) {}

bool Mage::hasEnoughMana() const {
    return currentMana >= manaCost;
}

void Mage::castSpell(Character& target) {
    currentMana -= manaCost;
    std::cout << getName() << " (MAGE) niem phep len " << target.getName()
              << " gay " << spellDamage << " sat thuong!\n";
    target.takeDamage(spellDamage);
}

void Mage::fallbackAttack(Character& target) {
    std::cout << getName() << " (MAGE) het mana! Danh thuong vao " << target.getName()
              << " gay " << fallbackDamage << " sat thuong!\n";
    target.takeDamage(fallbackDamage);
}

void Mage::resetSession() {
    Character::resetSession();
    currentMana = maxMana;
}

void Mage::performAction(Character& target) {
    if (hasEnoughMana()) {
        castSpell(target);
    } else {
        fallbackAttack(target);
    }
}
