#include "../include/CharacterRoster.hpp"
#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

namespace {
std::string trim(const std::string& value) {
    std::string::size_type first = 0;
    while (first < value.size() && std::isspace(static_cast<unsigned char>(value[first]))) {
        ++first;
    }

    std::string::size_type last = value.size();
    while (last > first && std::isspace(static_cast<unsigned char>(value[last - 1]))) {
        --last;
    }

    return value.substr(first, last - first);
}

std::vector<std::string> splitCsvLine(const std::string& line) {
    std::vector<std::string> fields;
    std::istringstream stream(line);
    std::string field;

    while (std::getline(stream, field, ',')) {
        fields.push_back(trim(field));
    }

    if (!line.empty() && line[line.size() - 1] == ',') {
        fields.push_back("");
    }

    return fields;
}

int parseIntField(const std::string& value, const std::string& fieldName) {
    std::istringstream stream(value);
    int parsed = 0;
    char extra = '\0';

    if (!(stream >> parsed) || (stream >> extra)) {
        throw std::runtime_error("Invalid " + fieldName);
    }

    return parsed;
}

void requirePositive(int value, const std::string& fieldName) {
    if (value <= 0) {
        throw std::runtime_error(fieldName + " must be positive");
    }
}

std::unique_ptr<Character> parseCharacterLine(const std::string& line) {
    const std::vector<std::string> fields = splitCsvLine(line);
    if (fields.empty() || fields[0].empty()) {
        throw std::runtime_error("Missing character type");
    }

    const std::string& type = fields[0];

    if (type == "WARRIOR") {
        if (fields.size() != 5) {
            throw std::runtime_error("WARRIOR format must be WARRIOR,id,name,maxHp,attackPower");
        }

        const int id = parseIntField(fields[1], "id");
        const std::string name = fields[2];
        const int maxHp = parseIntField(fields[3], "maxHp");
        const int attackPower = parseIntField(fields[4], "attackPower");

        requirePositive(id, "id");
        if (name.empty()) {
            throw std::runtime_error("name cannot be empty");
        }
        requirePositive(maxHp, "maxHp");
        requirePositive(attackPower, "attackPower");

        return std::unique_ptr<Character>(new Warrior(id, name, maxHp, attackPower));
    }

    if (type == "MAGE") {
        if (fields.size() != 8) {
            throw std::runtime_error("MAGE format must be MAGE,id,name,maxHp,maxMana,spellDamage,manaCost,fallbackDamage");
        }

        const int id = parseIntField(fields[1], "id");
        const std::string name = fields[2];
        const int maxHp = parseIntField(fields[3], "maxHp");
        const int maxMana = parseIntField(fields[4], "maxMana");
        const int spellDamage = parseIntField(fields[5], "spellDamage");
        const int manaCost = parseIntField(fields[6], "manaCost");
        const int fallbackDamage = parseIntField(fields[7], "fallbackDamage");

        requirePositive(id, "id");
        if (name.empty()) {
            throw std::runtime_error("name cannot be empty");
        }
        requirePositive(maxHp, "maxHp");
        requirePositive(maxMana, "maxMana");
        requirePositive(spellDamage, "spellDamage");
        requirePositive(manaCost, "manaCost");
        requirePositive(fallbackDamage, "fallbackDamage");

        return std::unique_ptr<Character>(
            new Mage(id, name, maxHp, maxMana, spellDamage, manaCost, fallbackDamage));
    }

    throw std::runtime_error("Unknown character type '" + type + "'");
}
}

CharacterRoster::CharacterRoster() {}

bool validCharacterStats(const Character* character);

Character* CharacterRoster::findCharacterById(int id) {
    auto it = std::find_if(m_characters.begin(), m_characters.end(),
        [id](const std::unique_ptr<Character>& ch) {
            return ch->getId() == id;
        });
    return (it != m_characters.end()) ? it->get() : nullptr;
}

const Character* CharacterRoster::findCharacterById(int id) const {
    auto it = std::find_if(m_characters.begin(), m_characters.end(),
        [id](const std::unique_ptr<Character>& ch) {
            return ch->getId() == id;
        });
    return (it != m_characters.end()) ? it->get() : nullptr;
}

bool CharacterRoster::addCharacter(std::unique_ptr<Character> character) {
    if (!character) {
        std::cerr << "Error: Cannot add a null character.\n";
        return false;
    }

    if (!validCharacterStats(character.get())) {
        std::cerr << "Error: Character data contains invalid stats.\n";
        return false;
    }

    if (hasCharacter(character->getId())) {
        std::cerr << "Error: Character with ID " << character->getId() << " already exists.\n";
        return false;
    }

    m_characters.push_back(std::move(character));
    return true;
}

bool validCharacterStats(const Character* character) {
    if (!character || character->getId() <= 0 || character->getName().empty() ||
        character->getMaxHp() <= 0) return false;
    if (const Warrior* warrior = dynamic_cast<const Warrior*>(character))
        return warrior->getAttackPower() > 0;
    if (const Mage* mage = dynamic_cast<const Mage*>(character))
        return mage->getMaxMana() > 0 && mage->getSpellDamage() > 0 &&
               mage->getManaCost() > 0 && mage->getFallbackDamage() > 0;
    return false;
}

std::string lowerCopy(const std::string& value) {
    std::string result = value;
    std::transform(result.begin(), result.end(), result.begin(),
        [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
    return result;
}

bool CharacterRoster::replaceCharacter(std::unique_ptr<Character> character) {
    if (!validCharacterStats(character.get())) {
        return false;
    }
    for (auto& existing : m_characters) {
        if (existing->getId() == character->getId()) {
            existing = std::move(character);
            return true;
        }
    }
    return false;
}

Character* CharacterRoster::getCharacterById(int id) {
    return findCharacterById(id);
}

const Character* CharacterRoster::getCharacterById(int id) const {
    return findCharacterById(id);
}

std::vector<Character*> CharacterRoster::findByName(const std::string& query) {
    std::vector<Character*> matches;
    const std::string needle = lowerCopy(query);
    for (auto& character : m_characters) {
        if (lowerCopy(character->getName()).find(needle) != std::string::npos)
            matches.push_back(character.get());
    }
    return matches;
}

std::vector<const Character*> CharacterRoster::findByName(const std::string& query) const {
    std::vector<const Character*> matches;
    const std::string needle = lowerCopy(query);
    for (const auto& character : m_characters) {
        if (lowerCopy(character->getName()).find(needle) != std::string::npos)
            matches.push_back(character.get());
    }
    return matches;
}

const std::vector<std::unique_ptr<Character>>& CharacterRoster::getAllCharacters() const {
    return m_characters;
}

bool CharacterRoster::removeCharacterById(int id) {
    auto it = std::find_if(m_characters.begin(), m_characters.end(),
        [id](const std::unique_ptr<Character>& ch) {
            return ch->getId() == id;
        });
    
    if (it != m_characters.end()) {
        m_characters.erase(it);
        return true;
    }
    return false;
}

void CharacterRoster::clear() {
    m_characters.clear();
}

size_t CharacterRoster::getCharacterCount() const {
    return m_characters.size();
}

bool CharacterRoster::hasCharacter(int id) const {
    return findCharacterById(id) != nullptr;
}

bool CharacterRoster::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "[CharacterRoster] " << filename
                  << " not found/readable; starting with an empty roster.\n";
        m_characters.clear();
        return true;
    }

    CharacterRoster loadedRoster;

    std::string line;
    int lineNumber = 0;

    while (std::getline(file, line)) {
        ++lineNumber;

        const std::string cleanedLine = trim(line);
        if (cleanedLine.empty() || cleanedLine[0] == '#') {
            continue;
        }

        try {
            std::unique_ptr<Character> character = parseCharacterLine(cleanedLine);
            const int id = character->getId();
            const std::string name = character->getName();
            const std::string type = character->getTypeName();

            if (loadedRoster.addCharacter(std::move(character))) {
                std::cout << "Loaded: " << type << " - ID:" << id << " Name:" << name << "\n";
            } else {
                std::cerr << "Warning: Skipped duplicate/invalid character at line " << lineNumber << "\n";
            }
        } catch (const std::exception& e) {
            std::cerr << "Warning: Skipped line " << lineNumber << ": " << e.what() << "\n";
        }
    }

    m_characters = std::move(loadedRoster.m_characters);
    std::cout << "Successfully loaded " << getCharacterCount() << " characters from '" << filename << "'.\n";
    return true;
}

bool CharacterRoster::saveToFile(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open file '" << filename << "' for writing.\n";
        return false;
    }

    // Write header comment
    file << "# Turn-Based Adventure Game - Character Roster\n";
    file << "# Format: TYPE,id,name,maxHp,param1,param2,...\n";
    file << "#\n";

    for (const auto& character : m_characters) {
        if (character->getTypeName() == std::string("WARRIOR")) {
            // Cast to Warrior to access attack power
            const Warrior* warrior = dynamic_cast<const Warrior*>(character.get());
            if (warrior) {
                file << "WARRIOR," << character->getId() << ","
                     << character->getName() << ","
                     << character->getMaxHp() << ","
                     << warrior->getAttackPower() << "\n";
            }
        }
        else if (character->getTypeName() == std::string("MAGE")) {
            // Cast to Mage to access mage-specific data
            const Mage* mage = dynamic_cast<const Mage*>(character.get());
            if (mage) {
                file << "MAGE," << character->getId() << ","
                     << character->getName() << ","
                     << character->getMaxHp() << ","
                     << mage->getMaxMana() << ","
                     << mage->getSpellDamage() << ","
                     << mage->getManaCost() << ","
                     << mage->getFallbackDamage() << "\n";
            }
        }
    }

    file.close();
    std::cout << "Successfully saved " << getCharacterCount() << " characters to '" << filename << "'.\n";
    return true;
}
