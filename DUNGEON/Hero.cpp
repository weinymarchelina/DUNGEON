#include "Hero.h"

void Hero::move(Position delta) {
	// Compute the next position
	Position next = this->sPos + delta;

	if (isPositionValid(next))
		this->sPos = next;
	else {
		std::cout << "Invalid location\n";
	}
}

void Hero::gainEXP(int points) {
	int number = points;
	while (number > 0) {
		if (sCurrentExp + number >= sMaxExp) {//level up
			number -= (sMaxExp - sCurrentExp);
			sCurrentExp = 0;
			sMaxExp = (int)((float)sMaxExp * 1.2f);
			sLevel++;
		}
		else {
			sCurrentExp += number;
			number = 0;
		}
	}
}

void Hero::damage(int points) {
	hp = hp - points;
	if (hp < 0)
		hp = 0;
}