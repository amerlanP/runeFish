#include <iostream>
#include <ostream>
#include <string>

using namespace std;

class Ability {
private:
	string name;
	int damage;
	int cooldown;
	int currentCooldown = 0;
	int useTime;
public:
	// Constructor
	Ability(string name, int damage, int cooldown, int useTime):
		name(name),
		damage(damage),
		cooldown(cooldown),
		useTime(useTime)
	{}

	// Copy constructor
	Ability(Ability* other):
		name(other->name),
		damage(other->damage),
		cooldown(other->cooldown),
		useTime(other->useTime),
		currentCooldown(other->currentCooldown)
	{}

	int getUseTime() {
		return this->useTime;
	}

	// Function to use the ability. Returns damage if ability is valid, -1 if on cooldown
	int use() {
		if (currentCooldown > 0) {
			return -1;
		}

		currentCooldown = cooldown;
		return damage;
	}

	void reduceCooldown(int time) {
		if (currentCooldown > 0) {
			currentCooldown = max(currentCooldown - time, 0);
		}
	}

	string getName() {
		return this->name;
	}

	int getCurrentCooldown() {
		return this->currentCooldown;
	}

	void printInfo() {
		cout << "Name: " << name << endl;
		cout << "Damage: " << damage << endl;
		cout << "Cooldown: " << cooldown << endl;
		cout << "Current Cooldown: " << currentCooldown << endl;
		cout << endl;
	}
};
