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

// The function of the Hero class takes an integer points parameter and reduces the hero's hit points (hp) by points
void Hero::damage(int points) {
	hp = hp - points;

	//If hp becomes less than 0, it is set to 0 to avoid negative hit points
	if (hp < 0)
		hp = 0;
}

// The function eats food or enemy that is located in front of the hero (limit two cells, based on direction)
void Hero::swallow(int width, int height, std::vector<Creature*>& creatures, std::vector<Item*>& items, int foodBoost) {
	// Set the hero position and direction as sample
	Position cell1 = this->sPos;
	Position cell2 = this->sPos;
	int heroDir = this->direction;

	// Modify the cell according to the hero's direction
	if (heroDir == 0) {
		cell1.y -= 1;
		cell2.y -= 2;
	}
	else if (heroDir == 1) {
		cell1.x += 1;
		cell2.x += 2;
	}
	else if (heroDir == 2) {
		cell1.y += 1;
		cell2.y += 2;
	}
	else if (heroDir == 3) {
		cell1.x -= 1;
		cell2.x -= 2;
	}

	// Initialize new variable
	Position eatenCell = Position(-1, -1);
	char icon = ' ';

	// Check whether the second cell is valid to set it as eatenCell and get the icon
	if (cell2.isInRange(width, height)) {
		char targetIcon = ' ';

		// Find creature that is on cell2 position
		for (auto& creature : creatures) {
			if (creature->getPos() == cell2) {
				targetIcon = creature->getIcon();
				break;
			}
		}

		// If the are no creature on cell2, then find whether there are items on that position
		if (targetIcon == ' ') {

			// Find item that is on cell2 position
			for (auto& item : items) {
				if (item->getPos() == cell2) {
					targetIcon = item->getIcon();
					break;
				}
			}
		}

		// If that position is not empty then set it as eaten cell
		if (targetIcon != ' ' && targetIcon != 'T') {
			icon = targetIcon;
			eatenCell = cell2;
		}
	}

	// Check whether the first cell is valid and overwites the eaten cell to the first one if the first cell also not empty
	if (cell1.isInRange(width, height)) {
		char targetIcon = ' ';

		// Find creature that is on cell1 position
		for (auto& creature : creatures) {
			if (creature->getPos() == cell1) {
				targetIcon = creature->getIcon();
				break;
			}
		}

		// If the are no creature on cell1, then find whether there is item on that position
		if (targetIcon == ' ') {

			// Find item that is on cell1 position
			for (auto& item : items) {
				if (item->getPos() == cell1) {
					targetIcon = item->getIcon();
					break;
				}
			}
		}

		// If that position is not empty trigger then set it as eaten cell
		if (targetIcon != ' ' && targetIcon != 'T') {
			icon = targetIcon;
			eatenCell = cell1;
		}
	}

	// If the eatenCell is not invalid, then handle the swallow
	if (eatenCell != Position(-1, -1)) {
		std::cout << icon;

		// Handle the swallow based on the swallowed class (item or enemy) from the eatenCell icon
		if (icon == 'C' || icon == 'S' || icon == '@' || icon == 'B' || icon == '&') {

			// Loop through the creatures to eat that enemy
			for (auto& creature : creatures) {

				// Check whether the creature is eatenCell creature and make sure that the hero's attack is higher than the creature's hp
				if (creature->getPos() == eatenCell && this->attack > creature->getHealth()) {
					// Set the creature hp to 0 to be deleted
					creature->setHealth(0);
					break;
				}
			}
		}
		else {
			// Loop though the items to eat that food
			for (auto& item : items) {

				// Check whether the item is the eaten cell and also make sure that the hero hp is not full
				if (item->getPos() == eatenCell && this->hp < 100) {
					// Handle different effect based on the type (tomato or not)
					if (item->getIcon() == 'M') {
						this->hp = 100;
					}
					else {
						// If the icon is not 'M', then it is a food, so just boost amount hp recovery
						int newHp = this->hp + foodBoost;

						// Set hp not exceeding 100
						if (newHp > 100) {
							newHp = 100;
						}

						// Set new hp to the hero's hp
						this->hp = newHp;
					}

					// Set isTriggered to true since the hero has eat the food
					item->setHasTriggered(true);
					break;
				}
			}
		}
	}
}

// Get the hero direction
int Hero::getDirection() { return this->direction; }

// Set the hero direction
void Hero::setDirection(int direction) { this->direction = direction; }

// Get hero attack
int Hero::getAttack() { return this->attack; }

// Set hero attack
void Hero::setAttack(int attack) { this->attack = attack; }