#pragma once
#include "Character.hpp"

class Warrior : public Character {
private:
    const int attackPower;

    void attack(Character& target);

public:
    Warrior(int id, const std::string& name, int maxHp, int attackPower);
    
    // Getter for attackPower (needed for file I/O)
    int getAttackPower() const;
    
    void performAction(Character& target) override;
};