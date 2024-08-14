#include "util.cpp"
#include <iostream>
#include <utility>
#include <vector>

using namespace std;

void onUse(set<Ability*> abilities, Ability* a) {
	int useTime;
	if (a != nullptr) {
		useTime = a->getUseTime();
	} else {
		useTime = 1;
	}

	for (Ability* b : abilities) {
		if (b != a) {
			b->reduceCooldown(useTime);
		}
	}
}

void run(int budget, set<Ability*> abilities, pair<int, vector<string>>& bestDmgAbilitiesPair, pair<int, vector<string>> currentDmgAbilitiesPair) {
	if (budget <= 0) {
		return;
	}

	for (Ability* a : abilities) {
		int useTime = a->getUseTime();
		/*cout << a->getCurrentCooldown() << endl;*/
		if (useTime > budget) {
			continue;
		}

		int dmgTemp = a->use();
		if (dmgTemp != -1) {
			currentDmgAbilitiesPair.first += dmgTemp;
			currentDmgAbilitiesPair.second.push_back(a->getName());

			if (currentDmgAbilitiesPair.first > bestDmgAbilitiesPair.first) {
				bestDmgAbilitiesPair = currentDmgAbilitiesPair;
			}

			budget -= useTime;
			onUse(abilities, a);
		} else {
			onUse(abilities, nullptr);
			budget -= 1;
			continue;
		}

		/*cout << dmgLocal << endl;*/
		run(budget, copyAbilities(abilities), bestDmgAbilitiesPair, currentDmgAbilitiesPair);
	}
}

int main() {
	int budget = 100;
	pair<int, vector<string>> bestDmgAbilitiesPair;

	ifstream f("abilities.txt");
	set<Ability*> abilities = parseAndLoad(f);

	run(budget, abilities, bestDmgAbilitiesPair, pair<int, vector<string>>(0, {}));

	cout << "Max Damage is: " << bestDmgAbilitiesPair.first << endl;
	cout << "The order is: ";

	for (string s : bestDmgAbilitiesPair.second) {
		cout << s << " ";
	}
	cout << endl;

	return 0;
}
