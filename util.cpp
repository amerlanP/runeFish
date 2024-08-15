#include "Ability.cpp"
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <vector>

vector<Ability*> parseAndLoad(ifstream& f) {
	vector<Ability*> abilities;
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
		abilities.push_back(temp);
	}

	return abilities;
}

vector<Ability*> copyAbilities(vector<Ability*>& other) {
	vector<Ability*> newVec;
	for (Ability* a : other) {
		newVec.push_back(new Ability(a));
	}
	return newVec;
}

