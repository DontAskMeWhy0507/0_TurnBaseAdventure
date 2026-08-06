#ifndef TURN_BASE_ADVENTURE_TEAM_PERSISTENCE_HPP
#define TURN_BASE_ADVENTURE_TEAM_PERSISTENCE_HPP

#include <string>

#include "ICharacterExistenceChecker.hpp"
#include "TeamManager.hpp"

/**
 * Converts between data/teams.txt and TeamManager.
 * Format per line: teamId|teamName|characterId1,characterId2,...
 */
namespace TeamPersistence {

/// Loads teams from filePath into manager (which should start empty).
/// Malformed lines are skipped with a reason printed to stderr; the
/// function still returns true — a bad line is not a fatal error.
/// If checker is non-null, member ids not present in the roster are
/// skipped (with a warning) instead of being added blindly.
/// If the file does not exist, manager is left empty and true is returned.
bool loadTeams(const std::string& filePath, TeamManager& manager,
                const ICharacterExistenceChecker* checker = nullptr);

/// Writes every team in manager to filePath in the format above.
/// Returns false (and prints a clear error) if the file cannot be opened
/// for writing.
bool saveTeams(const std::string& filePath, const TeamManager& manager);

} // namespace TeamPersistence

#endif // TURN_BASE_ADVENTURE_TEAM_PERSISTENCE_HPP
