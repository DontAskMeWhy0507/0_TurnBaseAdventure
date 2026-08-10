#include "Character.h"
#include <iostream>

void Warrior::performAction(Character& target) {
    std::cout << "[Warrior Action] " << m_name << " attacks " << target.getName()
              << " for " << m_attackPower << " damage!\n";
    target.takeDamage(m_attackPower);
}

void Mage::performAction(Character& target) {
    if (m_currentMana >= m_manaCost) {
        m_currentMana -= m_manaCost;
        std::cout << "[Mage Spell] " << m_name << " casts spell on " << target.getName()
                  << " for " << m_spellDamage << " damage! (Mana left: " << m_currentMana << ")\n";
        target.takeDamage(m_spellDamage);
    } else {
        std::cout << "[Mage Fallback] " << m_name << " strikes " << target.getName()
                  << " for fallback " << m_fallbackDamage << " damage! (Mana left: " << m_currentMana << ")\n";
        target.takeDamage(m_fallbackDamage);
    }
}
