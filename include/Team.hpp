#ifndef TURN_BASE_ADVENTURE_TEAM_HPP
#define TURN_BASE_ADVENTURE_TEAM_HPP

#include <cstddef>
#include <string>
#include <vector>
#include "Character.h"
#include "TeamError.hpp"

class Team {
public:
    static const std::size_t MAX_MEMBERS = 5;
    Team(int id, const std::string& name);

    int id() const;
    int getId() const { return id(); }
    const std::string& name() const;
    const std::string& getName() const { return name(); }
    const std::vector<int>& memberIds() const;
    const std::vector<Character*>& getMembers() const { return m_members; }

    std::size_t memberCount() const;
    std::size_t getSize() const { return memberCount(); }
    bool isEmpty() const;
    bool isFull() const;
    bool hasMember(int characterId) const;
    bool hasAliveCharacters() const;

    TeamError rename(const std::string& newName);
    TeamError addMember(int characterId);
    TeamError removeMember(int characterId);

    // Battle view; pointers are non-owning and are kept in the same order as IDs.
    void addCharacter(Character* character);

private:
    int m_id;
    std::string m_name;
    std::vector<int> m_characterIds;
    std::vector<Character*> m_members;
};

#endif
