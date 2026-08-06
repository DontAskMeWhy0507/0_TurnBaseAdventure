#include "TeamPersistence.hpp"

#include <cctype>
#include <fstream>
#include <iostream>
#include <sstream>

namespace {

std::string trim(const std::string& s) {
    std::size_t begin = 0;
    while (begin < s.size() && std::isspace(static_cast<unsigned char>(s[begin]))) {
        ++begin;
    }
    std::size_t end = s.size();
    while (end > begin && std::isspace(static_cast<unsigned char>(s[end - 1]))) {
        --end;
    }
    return s.substr(begin, end - begin);
}

std::vector<std::string> split(const std::string& s, char delim) {
    std::vector<std::string> parts;
    std::stringstream ss(s);
    std::string token;
    while (std::getline(ss, token, delim)) {
        parts.push_back(token);
    }
    // getline drops a trailing empty field (e.g. "a|b|" -> {"a","b"}); put it back.
    if (!s.empty() && s.back() == delim) {
        parts.push_back("");
    }
    return parts;
}

bool parsePositiveInt(const std::string& token, int& out) {
    if (token.empty()) {
        return false;
    }
    for (char c : token) {
        if (!std::isdigit(static_cast<unsigned char>(c))) {
            return false;
        }
    }
    try {
        long value = std::stol(token);
        if (value <= 0 || value > 2147483647L) {
            return false;
        }
        out = static_cast<int>(value);
        return true;
    } catch (...) {
        return false;
    }
}

void reportSkippedLine(int lineNumber, const std::string& reason) {
    std::cerr << "[TeamPersistence] Skipping teams.txt line " << lineNumber
              << ": " << reason << std::endl;
}

/// Used only when loadTeams() is called without a real roster (e.g. loading
/// Team data before/without CharacterRoster being wired up). Lets member ids
/// through unchecked so Team's own duplicate/size rules still apply via the
/// normal addCharacterToTeam() path, instead of silently dropping members.
class AllowAllExistenceChecker : public ICharacterExistenceChecker {
public:
    bool characterExists(int /*characterId*/) const override { return true; }
};

} // namespace

namespace TeamPersistence {

bool loadTeams(const std::string& filePath, TeamManager& manager,
               const ICharacterExistenceChecker* checker) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cout << "[TeamPersistence] " << filePath
                  << " not found — starting with an empty Team list." << std::endl;
        return true;
    }

    std::string line;
    int lineNumber = 0;
    while (std::getline(file, line)) {
        ++lineNumber;

        std::string trimmed = trim(line);
        if (trimmed.empty()) {
            continue;
        }

        std::vector<std::string> fields = split(trimmed, '|');
        if (fields.size() != 3) {
            reportSkippedLine(lineNumber, "expected 3 fields (teamId|teamName|ids), got "
                                               + std::to_string(fields.size()));
            continue;
        }

        int teamId = 0;
        if (!parsePositiveInt(trim(fields[0]), teamId)) {
            reportSkippedLine(lineNumber, "teamId '" + fields[0] + "' is not a positive integer");
            continue;
        }

        std::string teamName = trim(fields[1]);
        if (teamName.empty()) {
            reportSkippedLine(lineNumber, "teamName is empty");
            continue;
        }

        TeamError createResult = manager.createTeam(teamId, teamName);
        if (createResult != TeamError::None) {
            reportSkippedLine(lineNumber, "could not create team id=" + std::to_string(teamId)
                                               + " name='" + teamName + "' (duplicate id or name)");
            continue;
        }

        std::string idsField = trim(fields[2]);
        if (idsField.empty()) {
            continue; // team with no members yet — valid
        }

        AllowAllExistenceChecker fallbackChecker;
        const ICharacterExistenceChecker& activeChecker =
            (checker != nullptr) ? *checker : static_cast<const ICharacterExistenceChecker&>(fallbackChecker);

        for (const std::string& rawId : split(idsField, ',')) {
            std::string idToken = trim(rawId);
            if (idToken.empty()) {
                continue;
            }
            int characterId = 0;
            if (!parsePositiveInt(idToken, characterId)) {
                std::cerr << "[TeamPersistence] Line " << lineNumber << ": member id '"
                          << idToken << "' is not a positive integer — skipped." << std::endl;
                continue;
            }
            TeamError addResult = manager.addCharacterToTeam(teamId, characterId, activeChecker);
            if (addResult != TeamError::None) {
                std::cerr << "[TeamPersistence] Line " << lineNumber
                          << ": could not add character id " << characterId
                          << " to team " << teamId << " — skipped." << std::endl;
            }
        }
    }

    return true;
}

bool saveTeams(const std::string& filePath, const TeamManager& manager) {
    std::ofstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "[TeamPersistence] ERROR: could not open " << filePath
                  << " for writing." << std::endl;
        return false;
    }

    for (const Team& team : manager.teams()) {
        file << team.id() << '|' << team.name() << '|';
        const std::vector<int>& ids = team.memberIds();
        for (std::size_t i = 0; i < ids.size(); ++i) {
            if (i > 0) {
                file << ',';
            }
            file << ids[i];
        }
        file << '\n';
    }

    if (file.fail()) {
        std::cerr << "[TeamPersistence] ERROR: failed while writing " << filePath << std::endl;
        return false;
    }

    return true;
}

} // namespace TeamPersistence
