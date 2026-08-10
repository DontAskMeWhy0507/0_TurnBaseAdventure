#ifndef TURN_BASE_ADVENTURE_TEAM_ERROR_HPP
#define TURN_BASE_ADVENTURE_TEAM_ERROR_HPP

/// Outcome of a Team/TeamManager mutating operation.
enum class TeamError {
    None,                  // success
    InvalidId,              // team id is not a positive integer
    DuplicateId,             // team id already exists
    EmptyName,               // team name is empty
    DuplicateName,            // team name already used by another team
    TeamNotFound,             // no team with the given id
    CharacterNotInRoster,      // character id does not exist in the Character Roster
    DuplicateMember,           // character already a member of this team
    TeamFull,                   // team already has MAX_MEMBERS members
    MemberNotInTeam              // character id is not a member of this team
};

#endif // TURN_BASE_ADVENTURE_TEAM_ERROR_HPP
