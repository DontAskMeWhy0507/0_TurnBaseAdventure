// Self-contained smoke tests for Team / TeamManager / TeamPersistence
// (Hoang Tung Duong's module — T-06, T-07, T-08).
//
// No external test framework: a bad line in the assignment shouldn't block
// a team member from having something runnable to demo. Each CHECK prints
// PASS/FAIL; the process exits non-zero if anything failed.

#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <set>
#include <sstream>

#include "ICharacterExistenceChecker.hpp"
#include "TeamManager.hpp"
#include "TeamPersistence.hpp"

namespace {

int g_checks = 0;
int g_failures = 0;

void check(bool condition, const std::string& description) {
    ++g_checks;
    if (condition) {
        std::cout << "  [PASS] " << description << '\n';
    } else {
        ++g_failures;
        std::cout << "  [FAIL] " << description << '\n';
    }
}

class FakeRoster : public ICharacterExistenceChecker {
public:
    void add(int id) { m_ids.insert(id); }
    bool characterExists(int characterId) const override {
        return m_ids.count(characterId) > 0;
    }

private:
    std::set<int> m_ids;
};

void section(const std::string& name) {
    std::cout << "\n== " << name << " ==\n";
}

void testTeamCreationRules() {
    section("TeamManager::createTeam validation");
    TeamManager mgr;

    check(mgr.createTeam(201, "Red Team") == TeamError::None,
          "valid team is created");
    check(mgr.createTeam(201, "Another Name") == TeamError::DuplicateId,
          "duplicate teamId is rejected (TC-04-style)");
    check(mgr.createTeam(202, "Red Team") == TeamError::DuplicateName,
          "duplicate teamName is rejected");
    check(mgr.createTeam(0, "Zero Id Team") == TeamError::InvalidId,
          "non-positive teamId is rejected");
    check(mgr.createTeam(203, "") == TeamError::EmptyName,
          "empty teamName is rejected");
    check(mgr.teams().size() == 1, "rejected creates did not mutate the collection");
}

void testMembershipRules() {
    section("TeamManager membership rules (maps to TC-04)");
    TeamManager mgr;
    mgr.createTeam(201, "Red Team");

    FakeRoster roster;
    roster.add(101);
    roster.add(102);
    roster.add(103);
    roster.add(104);
    roster.add(105);
    roster.add(106);

    check(mgr.addCharacterToTeam(999, 101, roster) == TeamError::TeamNotFound,
          "adding to a non-existent team is rejected");
    check(mgr.addCharacterToTeam(201, 999, roster) == TeamError::CharacterNotInRoster,
          "adding an id not present in the roster is rejected");

    check(mgr.addCharacterToTeam(201, 101, roster) == TeamError::None, "add member 101");
    check(mgr.addCharacterToTeam(201, 101, roster) == TeamError::DuplicateMember,
          "duplicate member in the same team is rejected");

    mgr.addCharacterToTeam(201, 102, roster);
    mgr.addCharacterToTeam(201, 103, roster);
    mgr.addCharacterToTeam(201, 104, roster);
    mgr.addCharacterToTeam(201, 105, roster);
    check(mgr.findTeamById(201)->memberCount() == 5, "team now has 5 members");
    check(mgr.addCharacterToTeam(201, 106, roster) == TeamError::TeamFull,
          "6th member is rejected (5-member limit)");

    const Team* team = mgr.findTeamById(201);
    const std::vector<int>& ids = team->memberIds();
    check(ids.size() == 5 && ids[0] == 101 && ids[4] == 105,
          "members are kept in insertion order");

    check(mgr.removeCharacterFromTeam(201, 999) == TeamError::MemberNotInTeam,
          "removing an id not in the team is rejected");
    check(mgr.removeCharacterFromTeam(201, 103) == TeamError::None,
          "remove member 103");
    check(!mgr.findTeamById(201)->hasMember(103), "103 is no longer a member");
}

void testCascadeDelete() {
    section("TeamManager::removeCharacterFromAllTeams (TC-05)");
    TeamManager mgr;
    mgr.createTeam(201, "Red Team");
    mgr.createTeam(202, "Blue Team");

    FakeRoster roster;
    roster.add(101);
    roster.add(102);

    mgr.addCharacterToTeam(201, 101, roster);
    mgr.addCharacterToTeam(202, 101, roster); // same character can be on multiple template teams
    mgr.addCharacterToTeam(202, 102, roster);

    mgr.removeCharacterFromAllTeams(101);

    check(!mgr.findTeamById(201)->hasMember(101), "101 removed from Red Team");
    check(!mgr.findTeamById(202)->hasMember(101), "101 removed from Blue Team");
    check(mgr.findTeamById(202)->hasMember(102), "unrelated member 102 untouched");
}

void testRenameAndDelete() {
    section("TeamManager::renameTeam / deleteTeam");
    TeamManager mgr;
    mgr.createTeam(201, "Red Team");
    mgr.createTeam(202, "Blue Team");

    check(mgr.renameTeam(999, "X") == TeamError::TeamNotFound, "rename missing team rejected");
    check(mgr.renameTeam(201, "") == TeamError::EmptyName, "rename to empty name rejected");
    check(mgr.renameTeam(201, "Blue Team") == TeamError::DuplicateName,
          "rename to an existing name rejected");
    check(mgr.renameTeam(201, "Crimson Team") == TeamError::None, "valid rename succeeds");
    check(mgr.findTeamById(201)->name() == "Crimson Team", "new name is stored");

    check(mgr.deleteTeam(999) == TeamError::TeamNotFound, "delete missing team rejected");
    check(mgr.deleteTeam(201) == TeamError::None, "delete existing team succeeds");
    check(mgr.findTeamById(201) == nullptr, "deleted team no longer found");
}

void testPersistenceRoundTrip() {
    section("TeamPersistence save -> load round trip");
    const std::string path = "team_tests_roundtrip.tmp.txt";

    TeamManager original;
    original.createTeam(201, "Red Team");
    original.createTeam(202, "Blue Team");
    FakeRoster roster;
    roster.add(101);
    roster.add(102);
    roster.add(103);
    original.addCharacterToTeam(201, 101, roster);
    original.addCharacterToTeam(201, 103, roster);
    original.addCharacterToTeam(202, 102, roster);

    check(TeamPersistence::saveTeams(path, original), "save succeeds");

    TeamManager reloaded;
    check(TeamPersistence::loadTeams(path, reloaded, &roster), "load succeeds");

    check(reloaded.teams().size() == 2, "both teams reloaded");
    const Team* red = reloaded.findTeamById(201);
    check(red != nullptr && red->name() == "Red Team", "Red Team reloaded with correct name");
    check(red != nullptr && red->memberIds().size() == 2
              && red->memberIds()[0] == 101 && red->memberIds()[1] == 103,
          "Red Team members reloaded in original order");

    std::remove(path.c_str());
}

void testPersistenceRobustness() {
    section("TeamPersistence robustness (malformed lines / missing file)");
    const std::string path = "team_tests_malformed.tmp.txt";
    {
        std::ofstream out(path);
        out << "201|Red Team|101,102\n";
        out << "this line has no pipes at all\n";      // wrong field count -> skipped
        out << "abc|Bad Id Team|101\n";                 // non-numeric id -> skipped
        out << "203||101\n";                             // empty name -> skipped
        out << "204|Reserve Team|\n";                     // valid, zero members
    }

    FakeRoster roster;
    roster.add(101);
    roster.add(102);

    TeamManager mgr;
    check(TeamPersistence::loadTeams(path, mgr, &roster),
          "loadTeams does not crash / fail on malformed lines");
    check(mgr.teams().size() == 2, "only the 2 well-formed lines produced teams");
    check(mgr.findTeamById(201) != nullptr, "well-formed Red Team line loaded");
    check(mgr.findTeamById(204) != nullptr && mgr.findTeamById(204)->isEmpty(),
          "team with an empty member list loads as an empty team");

    std::remove(path.c_str());

    TeamManager missingFileMgr;
    check(TeamPersistence::loadTeams("this_file_does_not_exist.txt", missingFileMgr),
          "loading a missing file returns success (starts empty, no crash)");
    check(missingFileMgr.teams().empty(), "manager stays empty when file is missing");
}

} // namespace

int main() {
    std::cout << "Running Team module tests (Hoang Tung Duong)\n";

    testTeamCreationRules();
    testMembershipRules();
    testCascadeDelete();
    testRenameAndDelete();
    testPersistenceRoundTrip();
    testPersistenceRobustness();

    std::cout << "\n" << g_checks << " checks, " << g_failures << " failed\n";
    return g_failures == 0 ? 0 : 1;
}
