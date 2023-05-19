#pragma once
#include <string>
#include "Position.h"
#include "main.h"

class Creature {//creature class 
protected:
	Position	sPos;			// Creature position
	char sIcon = 'C';
	char normalIcon = 'C';
	char triggeredIcon = '!';
	bool hasSeenHero;
	int power;
	int viewDistance;
	Position heroPrevPos;
	int chaseHeroProbability;
	bool canMove = true;
	bool awareness = false;
	bool isDead = false;
	int hp;
	void updateSight(Hero& hero);
	virtual void updateDamage(Hero& hero);
	void chaseHero(Hero& hero);
	void moveRandomly();

public:
	// Default constructor
	Creature();
	void setPos(Position pos);
	void setPos(int x, int y);
	void setIcon(char& icon);
	Position getPos(void);
	char getIcon(void);
	bool getHasSeen();
	int getHealth();
	void getHealth(int);

public:
	virtual void update(Hero& hero);
};

