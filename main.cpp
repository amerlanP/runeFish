#include "util.cpp"
#include <algorithm>
#include <future>
#include <iostream>
#include <utility>
#include <vector>
#include <thread>

using namespace std;

int onUse(vector<Ability*> abilities, int useTime) {
	if (useTime != -1) {	
		for (Ability* a : abilities) {
			a->reduceCooldown(useTime);
		}
		return useTime;
	} else {
		int min = 9999;
		for (Ability* b : abilities) {
			min = std::min(min, b->getCurrentCooldown());
		}

		for (Ability* b : abilities) {
			b->reduceCooldown(min);
		}
		return min;
	}
}

void run(int budget, vector<Ability*> abilities, pair<int, vector<string>>& bestDmgAbilitiesPair, pair<int, vector<string>> currentDmgAbilitiesPair) {
	// if there is no time left, return
	if (budget <= 0) {
		return;
	}

	bool flag = false; // flag to determine if we used an ability
	for (int i = 0 ; i < abilities.size() ; i++) {
		pair<int, vector<string>> tempPairCopy(currentDmgAbilitiesPair);
		vector<Ability*> tempAbilitiesCopy = copyAbilities(abilities);
		int oldBudget = budget;
		int dmgTemp = abilities[i]->use();

		// if an ability was used -- not on cooldown.
		if (dmgTemp != -1) {
			if (!flag) {flag = true;} // we used at least one ability -- not all of them are on cooldown
			
			// add the damage and name of ability to the current damage count
			currentDmgAbilitiesPair.first += dmgTemp;
			currentDmgAbilitiesPair.second.push_back(abilities[i]->getName());

			// get the use time and reduce the budget and cooldown of all other
			// abilities by it.
			int useTime = abilities[i]->getUseTime();
			budget -= onUse(abilities, useTime);

			// compare to the current best result and replace accordingly.
			if (currentDmgAbilitiesPair.first > bestDmgAbilitiesPair.first) {
				bestDmgAbilitiesPair = currentDmgAbilitiesPair;
				for (string s : bestDmgAbilitiesPair.second) {
					cout << s << " -> ";
				}
				cout << endl;
			}

			// recursive call
			run(budget, copyAbilities(abilities), bestDmgAbilitiesPair, currentDmgAbilitiesPair);
			/*thread t(run, budget, copyAbilities(abilities), ref(bestDmgAbilitiesPair), currentDmgAbilitiesPair);*/
			/*t.detach();*/

			// replace variables so try new abilitiy and test damage
			currentDmgAbilitiesPair = tempPairCopy;
			abilities = tempAbilitiesCopy;
			budget = oldBudget;

			// else the ability was on cooldown -- try the next one.
			} else {
				continue;
			}

		/*for (string s : currentDmgAbilitiesPair.second) {*/
		/*cout << s << " ";*/
		/*}*/
		/*cout << endl;*/
	}

	// If we looped through all abilities but there is still
	if (budget > 0 && !flag) {
		budget -= onUse(abilities, -1);
		run(budget, copyAbilities(abilities), bestDmgAbilitiesPair, currentDmgAbilitiesPair);
	}
} //end run()

int main(int argc, char* argv[]) {
	vector<thread> threads;
	mutex bestDmgAbilitiesMutex;
	atomic<bool> isBestUpdated(false);

	int budget = atoi(argv[1]);
	pair<int, vector<string>> bestDmgAbilitiesPair;

	ifstream f("abilities.txt");
	vector<Ability*> abilities = parseAndLoad(f);
	run(budget, abilities, bestDmgAbilitiesPair, pair<int, vector<string>>(0, {}));

	threads.emplace_back(run, budget, abilities, ref(bestDmgAbilitiesPair), pair<int, vector<string>>(0, {}));
	threads[0].join();

	cout << "Max Damage is: " << bestDmgAbilitiesPair.first << endl;
	cout << "The order is: ";

	for (string s : bestDmgAbilitiesPair.second) {
		cout << s << " ";
	}
	cout << endl;

	return 0;
}
