#ifndef TEAM_H
#define TEAM_H

#include "Character.h"
#include <string>
#include <vector>

class Team {
private:
    int m_teamId;
    std::string m_teamName;
    std::vector<Character*> m_members;

public:
    Team(int id, const std::string& name) : m_teamId(id), m_teamName(name) {}

    int getId() const { return m_teamId; }
    std::string getName() const { return m_teamName; }

    void addCharacter(Character* character) {
        if (character) {
            m_members.push_back(character);
        }
    }

    const std::vector<Character*>& getMembers() const { return m_members; }

    bool hasAliveCharacters() const {
        for (const auto* c : m_members) {
            if (c && c->isAlive()) {
                return true;
            }
        }
        return false;
    }

    size_t getSize() const { return m_members.size(); }
};

#endif // TEAM_H
