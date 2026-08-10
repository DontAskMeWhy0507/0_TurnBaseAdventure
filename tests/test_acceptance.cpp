#include "BattleEngine.h"
#include "CharacterRoster.hpp"
#include "InputHandler.hpp"
#include "TeamManager.hpp"
#include "TeamPersistence.hpp"

#include <cassert>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>

static std::unique_ptr<Character> W(int id, const std::string& name, int damage = 20) {
    return std::unique_ptr<Character>(new Warrior(id, name, 100, damage));
}
static void pass(const char* id) { std::cout << id << " PASS\n"; }

static void tc01() {
    std::ofstream f("tc01_characters.txt");
    f << "WARRIOR,1,Ares,100,20\nMAGE,2,Luna,80,10,40,10,10\n"
      << "WARRIOR,3,Thor,100,25\nMAGE,4,Mira,70,20,30,10,5\n"
      << "WARRIOR,5,Atlas,90,15\nINVALID,6,Broken,100,10\n";
    f.close();
    CharacterRoster r;
    assert(r.loadFromFile("tc01_characters.txt") && r.getCharacterCount() == 5);
    std::remove("tc01_characters.txt"); pass("TC-01");
}

static void tc02() {
    CharacterRoster r;
    assert(r.addCharacter(W(1, "Ares")));
    assert(!r.addCharacter(W(1, "Duplicate")));
    assert(!r.addCharacter(std::unique_ptr<Character>(new Mage(2, "Bad", 80, 0, 40, 10, 10))));
    assert(r.getCharacterCount() == 1); pass("TC-02");
}

static void tc03() {
    CharacterRoster r;
    assert(r.addCharacter(W(1, "Ares")) && r.addCharacter(W(2, "ares junior")));
    assert(r.findByName("ARE").size() == 2);
    assert(r.findByName("not-found").empty()); pass("TC-03");
}

static void roster6(CharacterRoster& r) {
    for (int i = 1; i <= 6; ++i) assert(r.addCharacter(W(i, "C" + std::to_string(i))));
}

static void tc04() {
    CharacterRoster r; roster6(r); TeamManager m;
    assert(m.createTeam(10, "Red") == TeamError::None);
    assert(m.addCharacterToTeam(10, 999, r) == TeamError::CharacterNotInRoster);
    assert(m.addCharacterToTeam(10, 1, r) == TeamError::None);
    assert(m.addCharacterToTeam(10, 1, r) == TeamError::DuplicateMember);
    for (int i = 2; i <= 5; ++i) assert(m.addCharacterToTeam(10, i, r) == TeamError::None);
    assert(m.addCharacterToTeam(10, 6, r) == TeamError::TeamFull);
    assert(m.findTeamById(10)->memberCount() == 5); pass("TC-04");
}

static void tc05() {
    CharacterRoster r; assert(r.addCharacter(W(1, "Ares")));
    TeamManager m; assert(m.createTeam(10, "Red") == TeamError::None);
    assert(m.createTeam(11, "Blue") == TeamError::None);
    assert(m.addCharacterToTeam(10, 1, r) == TeamError::None);
    assert(m.addCharacterToTeam(11, 1, r) == TeamError::None);
    m.removeCharacterFromAllTeams(1); assert(r.removeCharacterById(1));
    assert(!m.findTeamById(10)->hasMember(1) && !m.findTeamById(11)->hasMember(1)); pass("TC-05");
}

static void tc06() {
    Warrior a(1, "A", 100, 30); Team empty(1, "Empty"), full(2, "Full");
    full.addCharacter(&a); BattleEngine e;
    assert(!e.setupBattle(&full, &full)); assert(e.getState() == BattleState::READY);
    assert(!e.setupBattle(&full, &empty)); assert(e.getState() == BattleState::READY); pass("TC-06");
}

static void tc07() {
    Warrior a(1, "Ares", 100, 30); Mage l(2, "Luna", 80, 10, 40, 10, 10);
    a.takeDamage(40); l.takeDamage(20); l.performAction(a); Team red(1, "Red"), blue(2, "Blue");
    red.addCharacter(&a); blue.addCharacter(&l); BattleEngine e;
    assert(e.setupBattle(&red, &blue) && e.start());
    assert(e.getState() == BattleState::IN_PROGRESS && e.getCurrentActor() == &a);
    assert(a.getCurrentHp() == 100 && l.getCurrentHp() == 80 && l.getCurrentMana() == 10); pass("TC-07");
}

static void tc08() {
    Warrior a(1, "Ares", 100, 30); Mage l(2, "Luna", 80, 10, 40, 10, 10);
    Team red(1, "Red"), blue(2, "Blue"); red.addCharacter(&a); blue.addCharacter(&l); BattleEngine e;
    assert(e.setupBattle(&red, &blue) && e.start());
    assert(!e.executeTurn(&l, &a) && e.getCurrentActor() == &a && a.getCurrentHp() == 100); pass("TC-08");
}

static void tc09() {
    Warrior a(1, "Ares", 100, 30); Mage l(2, "Luna", 80, 10, 40, 10, 10);
    Team red(1, "Red"), blue(2, "Blue"); red.addCharacter(&a); blue.addCharacter(&l); BattleEngine e;
    assert(e.setupBattle(&red, &blue) && e.start() && e.executeTurn(&a, &l));
    assert(l.getCurrentHp() == 50); pass("TC-09");
}

static void tc10() {
    Warrior a(1, "Ares", 100, 30); Mage l(2, "Luna", 80, 10, 40, 10, 10);
    Team red(1, "Red"), blue(2, "Blue"); red.addCharacter(&a); blue.addCharacter(&l); BattleEngine e;
    assert(e.setupBattle(&red, &blue) && e.start() && e.executeTurn(&a, &l));
    assert(e.executeTurn(&l, &a) && a.getCurrentHp() == 60 && l.getCurrentMana() == 0); pass("TC-10");
}

static void tc11() {
    Warrior a(1, "Ares", 100, 30); Mage l(2, "Luna", 80, 10, 40, 10, 10);
    Team red(1, "Red"), blue(2, "Blue"); red.addCharacter(&a); blue.addCharacter(&l); BattleEngine e;
    assert(e.setupBattle(&red, &blue) && e.start());
    assert(e.executeTurn(&a, &l) && e.executeTurn(&l, &a));
    assert(e.executeTurn(&a, &l) && e.executeTurn(&l, &a));
    assert(a.getCurrentHp() == 50 && l.getCurrentMana() == 0); pass("TC-11");
}

static void tc12() {
    Warrior a(1, "Ares", 100, 30), dead(2, "Dead", 100, 30), outsider(3, "Outsider", 100, 30);
    Team red(1, "Red"), blue(2, "Blue"); red.addCharacter(&a); blue.addCharacter(&dead); BattleEngine e;
    assert(e.setupBattle(&red, &blue) && e.start()); dead.takeDamage(100);
    assert(!e.executeTurn(&a, &dead) && e.getCurrentActor() == &a && a.getCurrentHp() == 100);
    assert(!e.executeTurn(&a, &outsider) && e.getCurrentActor() == &a);
    pass("TC-12");
}

static void tc13() {
    Warrior a(1, "A", 100, 50), dead(2, "Dead", 100, 50), next(3, "Next", 100, 50);
    Team x(1, "X"), y(2, "Y"); x.addCharacter(&a); y.addCharacter(&dead); y.addCharacter(&next); BattleEngine e;
    assert(e.setupBattle(&x, &y) && e.start()); dead.takeDamage(100);
    assert(e.executeTurn(&a, &next) && e.getCurrentActor() == &next); pass("TC-13");
}

static void tc14() {
    Warrior a(1, "A", 100, 100), b(2, "B", 100, 50);
    Team x(1, "X"), y(2, "Y"); x.addCharacter(&a); y.addCharacter(&b); BattleEngine e;
    assert(e.setupBattle(&x, &y) && e.start() && e.executeTurn(&a, &b));
    assert(e.getState() == BattleState::FINISHED && e.getWinningTeam() == &x); pass("TC-14");
}

static void tc15() {
    Warrior a(1, "A", 100, 100), b(2, "B", 100, 50);
    Team x(1, "X"), y(2, "Y"); x.addCharacter(&a); y.addCharacter(&b); BattleEngine e;
    assert(e.setupBattle(&x, &y) && e.start() && e.executeTurn(&a, &b));
    assert(!e.executeTurn(&a, &b) && e.getState() == BattleState::FINISHED); pass("TC-15");
}

static void tc16() {
    CharacterRoster r; assert(r.addCharacter(W(101, "Ares")));
    std::unique_ptr<Character> updated(new Warrior(101, "Ares Updated", 120, 35));
    assert(r.replaceCharacter(std::move(updated)));
    TeamManager m; assert(m.createTeam(201, "Red") == TeamError::None);
    assert(m.addCharacterToTeam(201, 101, r) == TeamError::None);
    assert(r.saveToFile("tc16_chars.txt") && TeamPersistence::saveTeams("tc16_teams.txt", m));
    CharacterRoster loadedR; TeamManager loadedM;
    assert(loadedR.loadFromFile("tc16_chars.txt"));
    assert(TeamPersistence::loadTeams("tc16_teams.txt", loadedM, &loadedR));
    assert(loadedR.getCharacterById(101)->getName() == "Ares Updated");
    assert(loadedM.findTeamById(201)->hasMember(101));
    BattleEngine fresh; assert(fresh.getState() == BattleState::READY);
    std::remove("tc16_chars.txt"); std::remove("tc16_teams.txt"); pass("TC-16");
}

static void tc17() {
    CharacterRoster r;
    assert(r.loadFromFile("missing_directory/characters.txt"));
    assert(r.getCharacterCount() == 0);
    assert(!r.saveToFile("missing_directory/characters.txt"));
    TeamManager m; assert(TeamPersistence::loadTeams("missing_directory/teams.txt", m)); pass("TC-17");
}

static void tc18() {
    std::istringstream input("letters\n99\n2\n"); std::ostringstream output;
    std::streambuf* oldIn = std::cin.rdbuf(input.rdbuf());
    std::streambuf* oldOut = std::cout.rdbuf(output.rdbuf());
    assert(InputHandler::getInt("value: ", 1, 5) == 2);
    std::cin.rdbuf(oldIn); std::cout.rdbuf(oldOut);
    assert(output.str().find("ERROR") != std::string::npos); pass("TC-18");
}

int main() {
    tc01(); tc02(); tc03(); tc04(); tc05(); tc06(); tc07(); tc08(); tc09();
    tc10(); tc11(); tc12(); tc13(); tc14(); tc15(); tc16(); tc17(); tc18();
    std::cout << "ALL 18 ACCEPTANCE TESTS PASSED\n";
    return 0;
}
