#pragma once
#include "Character.hpp"

class Mage : public Character {
private:
    const int maxMana;
    int currentMana;
    const int spellDamage;
    const int manaCost;
    const int fallbackDamage;

    bool hasEnoughMana() const;
    void castSpell(Character& target);
    void fallbackAttack(Character& target);

public:
    Mage(int id, const std::string& name, int maxHp, int maxMana, int spellDamage, int manaCost, int fallbackDamage);

    void resetSession() override;
    void performAction(Character& target) override;
};