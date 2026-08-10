#ifndef CHARACTER_H
#define CHARACTER_H

#include <string>

enum class CharacterType {
    WARRIOR,
    MAGE
};

/**
 * Abstract Base Class representing a game character.
 * Adheres to FR-04: Character must be abstract base class.
 */
class Character {
protected:
    int m_id;
    std::string m_name;
    int m_maxHp;
    int m_currentHp;
    CharacterType m_type;

public:
    Character(int id, const std::string& name, int maxHp, CharacterType type)
        : m_id(id), m_name(name), m_maxHp(maxHp), m_currentHp(maxHp), m_type(type) {}

    virtual ~Character() = default;

    // Pure virtual method for polymorphism (FR-04)
    virtual void performAction(Character& target) = 0;

    // Virtual reset method for battle initialization
    virtual void resetHpMana() {
        m_currentHp = m_maxHp;
    }

    // Damage intake method
    void takeDamage(int damage) {
        m_currentHp -= damage;
        if (m_currentHp < 0) {
            m_currentHp = 0;
        }
    }

    // Virtual display helper for polymorphic status printing
    virtual std::string getStatsString() const {
        return "HP: " + std::to_string(m_currentHp) + "/" + std::to_string(m_maxHp);
    }

    // Getters & Status

    int getId() const { return m_id; }
    std::string getName() const { return m_name; }
    int getMaxHp() const { return m_maxHp; }
    int getCurrentHp() const { return m_currentHp; }
    CharacterType getType() const { return m_type; }
    bool isAlive() const { return m_currentHp > 0; }
};

/**
 * Warrior Class (Derived from Character)
 */
class Warrior : public Character {
private:
    int m_attackPower;

public:
    Warrior(int id, const std::string& name, int maxHp, int attackPower)
        : Character(id, name, maxHp, CharacterType::WARRIOR), m_attackPower(attackPower) {}

    int getAttackPower() const { return m_attackPower; }

    void performAction(Character& target) override;
};

/**
 * Mage Class (Derived from Character)
 */
class Mage : public Character {
private:
    int m_maxMana;
    int m_currentMana;
    int m_spellDamage;
    int m_manaCost;
    int m_fallbackDamage;

public:
    Mage(int id, const std::string& name, int maxHp, int maxMana, int spellDamage, int manaCost, int fallbackDamage)
        : Character(id, name, maxHp, CharacterType::MAGE),
          m_maxMana(maxMana), m_currentMana(maxMana),
          m_spellDamage(spellDamage), m_manaCost(manaCost),
          m_fallbackDamage(fallbackDamage) {}

    int getMaxMana() const { return m_maxMana; }
    int getCurrentMana() const { return m_currentMana; }
    int getSpellDamage() const { return m_spellDamage; }
    int getManaCost() const { return m_manaCost; }
    int getFallbackDamage() const { return m_fallbackDamage; }

    std::string getStatsString() const override {
        return "HP: " + std::to_string(m_currentHp) + "/" + std::to_string(m_maxHp)
             + ", Mana: " + std::to_string(m_currentMana) + "/" + std::to_string(m_maxMana);
    }

    void resetHpMana() override {

        Character::resetHpMana();
        m_currentMana = m_maxMana;
    }

    void performAction(Character& target) override;
};

#endif // CHARACTER_H
