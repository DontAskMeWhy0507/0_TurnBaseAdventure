#ifndef TURN_BASE_ADVENTURE_CHARACTER_H
#define TURN_BASE_ADVENTURE_CHARACTER_H

#include <string>

enum class CharacterType { WARRIOR, MAGE };

// Compatibility comparison for the original roster tests, while the type
// itself remains a strongly typed enum in the unified interface.
inline bool operator==(CharacterType type, const char* name) {
    return (type == CharacterType::WARRIOR && std::string(name) == "WARRIOR") ||
           (type == CharacterType::MAGE && std::string(name) == "MAGE");
}
inline bool operator==(const char* name, CharacterType type) {
    return type == name;
}

class Character {
protected:
    int m_id;
    std::string m_name;
    int m_maxHp;
    int m_currentHp;
    CharacterType m_type;

    void setCurrentHp(int hp) {
        m_currentHp = hp < 0 ? 0 : (hp > m_maxHp ? m_maxHp : hp);
    }

public:
    Character(int id, const std::string& name, int maxHp, CharacterType type)
        : m_id(id), m_name(name), m_maxHp(maxHp), m_currentHp(maxHp), m_type(type) {}
    virtual ~Character() = default;

    virtual void performAction(Character& target) = 0;
    virtual void resetHpMana() { m_currentHp = m_maxHp; }
    virtual void resetSession() { resetHpMana(); }

    void takeDamage(int damage) {
        if (damage > 0) setCurrentHp(m_currentHp - damage);
    }

    virtual std::string getStatsString() const {
        return "HP: " + std::to_string(m_currentHp) + "/" + std::to_string(m_maxHp);
    }

    int getId() const { return m_id; }
    const std::string& getName() const { return m_name; }
    int getMaxHp() const { return m_maxHp; }
    int getCurrentHp() const { return m_currentHp; }
    CharacterType getType() const { return m_type; }
    const char* getTypeName() const {
        return m_type == CharacterType::WARRIOR ? "WARRIOR" : "MAGE";
    }
    bool isAlive() const { return m_currentHp > 0; }
};

class Warrior : public Character {
    int m_attackPower;
public:
    Warrior(int id, const std::string& name, int maxHp, int attackPower)
        : Character(id, name, maxHp, CharacterType::WARRIOR), m_attackPower(attackPower) {}
    int getAttackPower() const { return m_attackPower; }
    void performAction(Character& target) override;
};

class Mage : public Character {
    int m_maxMana;
    int m_currentMana;
    int m_spellDamage;
    int m_manaCost;
    int m_fallbackDamage;
public:
    Mage(int id, const std::string& name, int maxHp, int maxMana, int spellDamage,
         int manaCost, int fallbackDamage)
        : Character(id, name, maxHp, CharacterType::MAGE), m_maxMana(maxMana),
          m_currentMana(maxMana), m_spellDamage(spellDamage), m_manaCost(manaCost),
          m_fallbackDamage(fallbackDamage) {}
    int getMaxMana() const { return m_maxMana; }
    int getCurrentMana() const { return m_currentMana; }
    int getSpellDamage() const { return m_spellDamage; }
    int getManaCost() const { return m_manaCost; }
    int getFallbackDamage() const { return m_fallbackDamage; }
    std::string getStatsString() const override {
        return "HP: " + std::to_string(m_currentHp) + "/" + std::to_string(m_maxHp) +
               ", Mana: " + std::to_string(m_currentMana) + "/" + std::to_string(m_maxMana);
    }
    void resetHpMana() override { Character::resetHpMana(); m_currentMana = m_maxMana; }
    void resetSession() override { resetHpMana(); }
    void performAction(Character& target) override;
};

#endif
