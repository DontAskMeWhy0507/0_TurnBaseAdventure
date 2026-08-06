#include "../include/Character.hpp"

Character::Character(int id, const std::string& name, int maxHp, const std::string& type)
    : id(id), name(name), maxHp(maxHp), currentHp(maxHp), type(type) {}

int Character::getId() const { return id; }
const std::string& Character::getName() const { return name; }
int Character::getMaxHp() const { return maxHp; }
int Character::getCurrentHp() const { return currentHp; }
const std::string& Character::getType() const { return type; }
bool Character::isAlive() const { return currentHp > 0; }

void Character::setCurrentHp(int hp) {
    if (hp < 0) {
        currentHp = 0;
    } else if (hp > maxHp) {
        currentHp = maxHp;
    } else {
        currentHp = hp;
    }
}

void Character::takeDamage(int damage) {
    if (damage < 0) return; // Không nhận sát thương âm
    setCurrentHp(currentHp - damage);
}

void Character::resetSession() {
    setCurrentHp(maxHp); // Khi bắt đầu trận, HP reset về max[cite: 1]
}