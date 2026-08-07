#include "Team.hpp"

#include <algorithm>

Team::Team(int id, const std::string& name)
    : m_id(id)
    , m_name(name)
{
}

int Team::id() const {
    return m_id;
}

const std::string& Team::name() const {
    return m_name;
}

const std::vector<int>& Team::memberIds() const {
    return m_characterIds;
}

std::size_t Team::memberCount() const {
    return m_characterIds.size();
}

bool Team::isEmpty() const {
    return m_characterIds.empty();
}

bool Team::isFull() const {
    return m_characterIds.size() >= MAX_MEMBERS;
}

bool Team::hasMember(int characterId) const {
    return std::find(m_characterIds.begin(), m_characterIds.end(), characterId)
        != m_characterIds.end();
}

TeamError Team::rename(const std::string& newName) {
    if (newName.empty()) {
        return TeamError::EmptyName;
    }
    m_name = newName;
    return TeamError::None;
}

TeamError Team::addMember(int characterId) {
    if (hasMember(characterId)) {
        return TeamError::DuplicateMember;
    }
    if (isFull()) {
        return TeamError::TeamFull;
    }
    m_characterIds.push_back(characterId);
    return TeamError::None;
}

TeamError Team::removeMember(int characterId) {
    auto it = std::find(m_characterIds.begin(), m_characterIds.end(), characterId);
    if (it == m_characterIds.end()) {
        return TeamError::MemberNotInTeam;
    }
    m_characterIds.erase(it);
    return TeamError::None;
}
