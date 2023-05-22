#include "Hero.h"

// The function of the Hero class takes a delta parameter of type Position
void Hero::move(Position delta) {
	// Compute the next position
	Position next = this->sPos + delta;

	//If the computed next position is valid based on the isPositionValid function, the sPos is updated to the next position
	if (isPositionValid(next))
		this->sPos = next;
	else {
		std::cout << "Invalid location\n";
	}
}

//The function of the Hero class takes an integer points parameter and increments the hero's experience points (sCurrentExp)
void Hero::gainEXP(int points) {
	//Intialized number as points
	int number = points;

	//while number is greater than 0
	while (number > 0) {
		//If the sum of the current experience points and number is greater than or equal to the maximum experience points (sMaxExp), the hero levels up
		if (sCurrentExp + number >= sMaxExp) {//level up
			number -= (sMaxExp - sCurrentExp);
			sCurrentExp = 0;
			sMaxExp = (int)((float)sMaxExp * 1.2f);
			sLevel++;
		}
		//If the sum of the current experience points and number is less than sMaxExp
		else {
			sCurrentExp += number;
			number = 0;
		}
	}
}

//The  function of the Hero class takes an integer points parameter and reduces the hero's hit points (hp) by points
void Hero::damage(int points) {
	hp = hp - points;

	//If hp becomes less than 0, it is set to 0 to avoid negative hit points
	if (hp < 0)
		hp = 0;
}