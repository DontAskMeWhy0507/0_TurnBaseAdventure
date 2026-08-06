#ifndef TURN_BASE_ADVENTURE_TEAM_MANAGER_HPP
#define TURN_BASE_ADVENTURE_TEAM_MANAGER_HPP

#include <string>
#include <vector>

#include "ICharacterExistenceChecker.hpp"
#include "Team.hpp"
#include "TeamError.hpp"

/**
 * Owns the collection of every Team (composition, by value — no manual
 * new/delete needed). Enforces the cross-team rules that an individual
 * Team can't enforce on its own: unique team id, unique team name, and
 * (via ICharacterExistenceChecker) that a character id being added
 * actually exists in the Character Roster.
 */
class TeamManager {
public:
    TeamManager() = default;

    TeamError createTeam(int teamId, const std::string& teamName);
    TeamError renameTeam(int teamId, const std::string& newName);
    TeamError deleteTeam(int teamId);

    TeamError addCharacterToTeam(int teamId, int characterId,
                                  const ICharacterExistenceChecker& checker);
    TeamError removeCharacterFromTeam(int teamId, int characterId);

    /// Cascade delete hook: call this when a character is removed from the
    /// Roster so it no longer lingers in any Team (FR-01 / TC-05).
    void removeCharacterFromAllTeams(int characterId);

    const Team* findTeamById(int teamId) const;
    bool hasTeam(int teamId) const;
    const std::vector<Team>& teams() const;

private:
    Team* findTeamMutable(int teamId);
    bool nameInUse(const std::string& name, int excludingTeamId) const;

    std::vector<Team> m_teams;
};

#endif // TURN_BASE_ADVENTURE_TEAM_MANAGER_HPP
