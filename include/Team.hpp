#ifndef TURN_BASE_ADVENTURE_TEAM_HPP
#define TURN_BASE_ADVENTURE_TEAM_HPP

#include <cstddef>
#include <string>
#include <vector>

#include "TeamError.hpp"

/**
 * A named roster slot: an id, a display name, and an ordered list of
 * Character IDs (not Character pointers — Team only ever stores identity,
 * per FR-02).
 *
 * Team owns its member-list invariants (no duplicate member, size limit,
 * insertion order). Cross-team rules (team id/name uniqueness, whether a
 * character id actually exists in the roster) are TeamManager's job, since
 * they require knowledge Team itself doesn't have.
 */
class Team {
public:
    static const std::size_t MAX_MEMBERS = 5;

    Team(int id, const std::string& name);

    int id() const;
    const std::string& name() const;
    const std::vector<int>& memberIds() const;

    std::size_t memberCount() const;
    bool isEmpty() const;
    bool isFull() const;
    bool hasMember(int characterId) const;

    /// Structural rename (non-empty check only; cross-team dup check is TeamManager's job).
    TeamError rename(const std::string& newName);

    /// Appends characterId if not already present and under MAX_MEMBERS.
    /// Does NOT check whether characterId exists in the Character Roster.
    TeamError addMember(int characterId);

    /// Removes characterId if present; MemberNotInTeam otherwise.
    TeamError removeMember(int characterId);

private:
    int m_id;
    std::string m_name;
    std::vector<int> m_characterIds;
};

#endif // TURN_BASE_ADVENTURE_TEAM_HPP
