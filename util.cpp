#include "Ability.cpp"
#include <cstdlib>
#include <set>
#include <fstream>
#include <sstream>

set<Ability*> parseAndLoad(ifstream& f) {
	set<Ability*> abilities;
	string line;
	getline(f, line);
	while (getline(f, line)) {
		stringstream ss(line);
		string name;
		string damage;
		string cooldown;
		string useTime;
		ss >> name;
		ss >> damage;
		ss >> cooldown;
		ss >> useTime;
		Ability* temp = new Ability(name, stoi(damage), stoi(cooldown), stoi(useTime));
		abilities.insert(temp);
	}

	return abilities;
}

set<Ability*> copyAbilities(set<Ability*> other) {
	set<Ability*> newSet;
	for (Ability* a : other) {
		newSet.insert(new Ability(a));
	}
	return newSet;
}

