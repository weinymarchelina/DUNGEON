#include "Hero.h"

// Default constructor
Hero::Hero() {
	this->sPos.x = 0;
	this->sPos.y = 0;
	this->sLevel = 1;
	this->sCurrentExp = 0;
	this->sMaxExp = 15;
	this->sIcon = 'H';
	this->hp = 100;
	this->direction = 0;
	this->attack = 0;
};
// Setting constructor
Hero::Hero(int x, int y, int level, int maxExp, int currentExp, char icon, int hp) {
	this->sPos.x = x;
	this->sPos.y = y;
	this->sLevel = level;
	this->sCurrentExp = currentExp;
	this->sMaxExp = maxExp;
	this->sIcon = icon;
	this->hp = hp;
	this->direction = 0;
	this->attack = 0;
};

// Setting constructor
Hero::Hero(Position& pos, int level, int maxExp, int currentExp, char icon, int hp) {
	this->sPos = pos;
	this->sLevel = level;
	this->sCurrentExp = currentExp;
	this->sMaxExp = maxExp;
	this->sIcon = icon;
	this->hp = hp;
	this->direction = 0;
	this->attack = 0;
};

// Set position for position parameter
void Hero::setPos(Position pos) { this->sPos = pos; }

// Set position for int x and int y parameter
void Hero::setPos(int x, int y) {
	this->sPos.x = x;
	this->sPos.y = y;
}

// Set icon
void Hero::setIcon(char& icon) { this->sIcon = icon; }

// Set level
void Hero::setLevel(int level) { this->sLevel = level; }

// Set exp
void Hero::setExp(int exp) { this->sCurrentExp = exp; }

// Set MaxExp
void Hero::SetMaxExp(int maxexp) { this->sMaxExp = maxexp; }

// Get position
Position Hero::getPos(void) { return this->sPos; }

// Get Icon
char Hero::getIcon(void) { return this->sIcon; }

// Get level
int Hero::getLevel(void) { return this->sLevel; }

// Get Exp
int Hero::getExp(void) { return this->sCurrentExp; }

// Get Max
int Hero::getMaxExp(void) { return this->sMaxExp; }

// Get HP
int Hero::getHP() { return this->hp; }

// Set HP
void Hero::setHP(int hp) { this->hp = hp; }

// The function of the Hero class takes a delta parameter of type Position
void Hero::move(Position delta) {
	// Compute the next position
	Position next = this->sPos + delta;

	// If the computed next position is valid based on the isPositionValid function, the sPos is updated to the next position
	if (isPositionValid(next))
		this->sPos = next;
	else {
		try {
			// Throw a runtime_error exception if when position is invalid
			if (!isPositionValid(next)) {
				throw std::runtime_error("Invalid location!"); 
			}
		}
		catch (const std::exception& e) {
			// Catch the exception and print "Invalid location" by retrieving the error message from e.what()
			std::cout << e.what() << std::endl;
		}
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

	// Update attack based on the current level
	this->attack = 1.5 * sLevel;
}

//The function of the Hero class takes an integer points parameter and reduces the hero's hit points (hp) by points
void Hero::damage(int points) {
	hp = hp - points;

	//If hp becomes less than 0, it is set to 0 to avoid negative hit points
	if (hp < 0)
		hp = 0;
}

//
void Hero::swallow(const char** board) {

	/*
	Position cell1 = this->sPos;
	Position cell2 = this->sPos;

	if (this->direction == 0) {
		cell1.y -= 1;
		cell2.y -= 2;
	}
	else if (this->direction == 1) {
		cell1.x += 1;
		cell2.x += 2;
	}
	else if (this->direction == 2) {
		cell1.y += 1;
		cell2.y += 2;
	}
	else if (this->direction == 3) {
		cell1.x -= 1;
		cell2.x -= 2;
	}

	if (isPositionValid(cell1)) {
		char icon = board[cell1.y][cell1.x];

		if (icon == 'C' || icon == 'S' || icon == '@' || icon == 'B' || icon == '&') {

		}
		else if (icon == '0' || icon == '1' || icon == '2' || icon == '3' || icon == '4' || icon == 'M') {

		}

	}

	if (isPositionValid(cell2)) {
		char icon = board[cell2.y][cell2.x];

	}
	*/


}

// Get the hero direction
int Hero::getDirection() { return this->direction; }

// Set the hero direction
void Hero::setDirection(int direction) { this->direction = direction; }

// Get hero attack
int Hero::getAttack() { return this->attack; }

// Set hero attack
void Hero::setAttack(int attack) { this->attack = attack; }