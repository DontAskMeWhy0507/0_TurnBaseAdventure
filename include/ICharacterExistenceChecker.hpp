#ifndef TURN_BASE_ADVENTURE_ICHARACTER_EXISTENCE_CHECKER_HPP
#define TURN_BASE_ADVENTURE_ICHARACTER_EXISTENCE_CHECKER_HPP

/**
 * Narrow interface the Team module depends on instead of the concrete
 * CharacterRoster (owned by another member). Keeps Team/TeamManager
 * compilable and testable on their own; CharacterRoster just needs to
 * implement this one method to plug in for real.
 */
class ICharacterExistenceChecker {
public:
    virtual ~ICharacterExistenceChecker() = default;

    /// True if a character with this id currently exists in the roster.
    virtual bool characterExists(int characterId) const = 0;
};

#endif // TURN_BASE_ADVENTURE_ICHARACTER_EXISTENCE_CHECKER_HPP
