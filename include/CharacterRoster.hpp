#pragma once
#include "Character.hpp"
#include "Warrior.hpp"
#include "Mage.hpp"
#include <vector>
#include <memory>
#include <string>

/**
 * CharacterRoster - Manages collection of Character objects
 * 
 * Responsibilities:
 *   - CRUD operations (Create, Read, Update, Delete)
 *   - Store characters by ID
 *   - File I/O (load/save from characters.txt)
 *   - Parse WARRIOR and MAGE types
 */
class CharacterRoster {
private:
    std::vector<std::unique_ptr<Character>> m_characters;

    Character* findCharacterById(int id);
    const Character* findCharacterById(int id) const;

public:
    CharacterRoster();
    ~CharacterRoster() = default;

    bool addCharacter(std::unique_ptr<Character> character);

    Character* getCharacterById(int id);
    const Character* getCharacterById(int id) const;

    const std::vector<std::unique_ptr<Character>>& getAllCharacters() const;

    bool removeCharacterById(int id);

    void clear();

    size_t getCharacterCount() const;

    bool hasCharacter(int id) const;

    bool loadFromFile(const std::string& filename);

    bool saveToFile(const std::string& filename) const;
};
