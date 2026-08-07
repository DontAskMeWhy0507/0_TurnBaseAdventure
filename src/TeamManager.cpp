#include "TeamManager.hpp"

#include <algorithm>

TeamError TeamManager::createTeam(int teamId, const std::string& teamName) {
    if (teamId <= 0) {
        return TeamError::InvalidId;
    }
    if (hasTeam(teamId)) {
        return TeamError::DuplicateId;
    }
    if (teamName.empty()) {
        return TeamError::EmptyName;
    }
    if (nameInUse(teamName, /*excludingTeamId=*/0)) {
        return TeamError::DuplicateName;
    }

    m_teams.push_back(Team(teamId, teamName));
    return TeamError::None;
}

TeamError TeamManager::renameTeam(int teamId, const std::string& newName) {
    Team* team = findTeamMutable(teamId);
    if (team == nullptr) {
        return TeamError::TeamNotFound;
    }
    if (newName.empty()) {
        return TeamError::EmptyName;
    }
    if (nameInUse(newName, teamId)) {
        return TeamError::DuplicateName;
    }
    return team->rename(newName);
}

TeamError TeamManager::deleteTeam(int teamId) {
    auto it = std::find_if(m_teams.begin(), m_teams.end(),
                            [teamId](const Team& t) { return t.id() == teamId; });
    if (it == m_teams.end()) {
        return TeamError::TeamNotFound;
    }
    m_teams.erase(it);
    return TeamError::None;
}

TeamError TeamManager::addCharacterToTeam(int teamId, int characterId,
                                           const ICharacterExistenceChecker& checker) {
    Team* team = findTeamMutable(teamId);
    if (team == nullptr) {
        return TeamError::TeamNotFound;
    }
    if (!checker.characterExists(characterId)) {
        return TeamError::CharacterNotInRoster;
    }
    return team->addMember(characterId);
}

TeamError TeamManager::removeCharacterFromTeam(int teamId, int characterId) {
    Team* team = findTeamMutable(teamId);
    if (team == nullptr) {
        return TeamError::TeamNotFound;
    }
    return team->removeMember(characterId);
}

void TeamManager::removeCharacterFromAllTeams(int characterId) {
    for (Team& team : m_teams) {
        if (team.hasMember(characterId)) {
            team.removeMember(characterId);
        }
    }
}

const Team* TeamManager::findTeamById(int teamId) const {
    auto it = std::find_if(m_teams.begin(), m_teams.end(),
                            [teamId](const Team& t) { return t.id() == teamId; });
    return it == m_teams.end() ? nullptr : &(*it);
}

bool TeamManager::hasTeam(int teamId) const {
    return findTeamById(teamId) != nullptr;
}

const std::vector<Team>& TeamManager::teams() const {
    return m_teams;
}

Team* TeamManager::findTeamMutable(int teamId) {
    auto it = std::find_if(m_teams.begin(), m_teams.end(),
                            [teamId](const Team& t) { return t.id() == teamId; });
    return it == m_teams.end() ? nullptr : &(*it);
}

bool TeamManager::nameInUse(const std::string& name, int excludingTeamId) const {
    for (const Team& team : m_teams) {
        if (team.id() == excludingTeamId) {
            continue;
        }
        if (team.name() == name) {
            return true;
        }
    }
    return false;
}
