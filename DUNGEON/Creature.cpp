#include "Creature.h"

// Default constructor
Creature::Creature()
{
    this->sPos.x = -1;
    this->sPos.y = -1;
    this->sIcon = 'C';
    this->normalIcon = 'C';
    this->triggeredIcon = '!';
    this->power = 2;
    this->hasSeenHero = false;
    this->viewDistance = 3;
    this->chaseHeroProbability = 0;
    this->heroPrevPos = Position(-1, -1);
    this->heroPrevDir = 0;
    this->canMove = true;
    this->awareness = false;
    this->hp = 1;
    this->direction = 2;
}

// Set position
void Creature::setPos(Position pos)
{
    this->sPos = pos;
    this->heroPrevPos = pos;
}

// Set position for x y parameter
void Creature::setPos(int x, int y)
{
    this->sPos.x = x;
    this->sPos.y = y;
}

// Set icon
void Creature::setIcon(char& icon)
{
    this->sIcon = icon;
}

// Get position
Position Creature::getPos()
{
    return this->sPos;
}

// Get Icon
char Creature::getIcon()
{
    return this->sIcon;
}

// Get has seen
bool Creature::getHasSeen() {
    return this->hasSeenHero;
}

// Get creature's HP
int Creature::getHealth() {
    return this->hp;
}

// Set creature's HP
void Creature::setHealth(int hp) {
    this->hp = hp;
}

// Update creature state
void Creature::update(Hero& hero)
{
    // Check and update creature sight state
    this->updateSight(hero);

    // Let the creature chase the hero if it sees the hero, otherwise let it move randomly
    if (this->hasSeenHero) {
        // Calculate the probability of chasing the hero
        int chaseProbability = rand() % 101;

        if (chaseProbability <= this->chaseHeroProbability) {
            this->chaseHero(hero);
        }
        else if (this->canMove) {
            this->moveRandomly();
        }
    }
    else if (this->canMove) {
        this->moveRandomly();
    }

    // Update damage to the hero
    this->updateDamage(hero);

    // If the position of the creature is the same as the position of hero, let it move randomly till the position is not the same
    while (hero.getPos() == sPos) {
        this->moveRandomly();
    }
}


// Update Sight
void Creature::updateSight(Hero& hero)
{
    Position hPos = hero.getPos();

    Position dir;
    // if (canSee(this->sPos, hPos, dir))
    if (canSee(this->sPos, hPos, dir, this->viewDistance) && this->awareness)
    {
        // std::cout << "Creature has found the Hero in the (" << dir.x << ", " << dir.y << ") direction\n";
        this->sIcon = this->triggeredIcon;
        this->hasSeenHero = true;
    }
    else
    {
        // std::cout << "Creature didn't find the Hero\n";
        this->sIcon = this->normalIcon;
        this->hasSeenHero = false;
    }
}

// Update damage to hero
void Creature::updateDamage(Hero& hero)
{
    Position hPos = hero.getPos();

    if (hPos == sPos)
    {
        // std::cout << this->normalIcon << " damaged hero " << this->power << std::endl;
        hero.damage(power);
        this->moveRandomly();
    }
}

// Move randomly
void Creature::moveRandomly() {
    // Move the creature randomly to up, down, left or right and test whether the new position is valid
    int randomNum = rand() % 4;

    Position newPosition = sPos;

    if (randomNum == 0) {
        newPosition.x += 1;
        this->direction = 1;
    }
    else if (randomNum == 1) {
        newPosition.x -= 1;
        this->direction = 3;
    }
    else if (randomNum == 2) {
        newPosition.y += 1;
        this->direction = 2;
    }
    else if (randomNum == 3) {
        newPosition.y -= 1;
        this->direction = 0;
    }

    if (isPositionValid(newPosition)) {
        sPos = newPosition;
    }
}

// Update move
void Creature::chaseHero(Hero& hero) {
    // get the hero's position
    Position hPos = hero.getPos();

    // check if the hero is within viewRange steps distance from the creature
    int dx = abs(sPos.x - hPos.x);
    int dy = abs(sPos.y - hPos.y);

    int newDx = abs(sPos.x - this->heroPrevPos.x);
    int newDy = abs(sPos.y - this->heroPrevPos.y);

    bool overlapsHero = (hPos.x == sPos.x && hPos.y == sPos.y);

    // if the hero is within 1 step range of the creature on diagonal axis, let the creature move to the hero's previous spot to prevent the hero being blocked by the creature on a specific axis
    if (newDx == 1 && newDy == 1 && dx == 1 && dy == 1 && !overlapsHero && isPositionValid(this->heroPrevPos))
    {
        this->sPos = this->heroPrevPos;
        this->direction = this->heroPrevDir;
    }
    else
    {
        // let the creature move towards hero
        if (hPos.x - sPos.x > 0) {
            dx = 1;
            this->direction = 1;
        }
        else if (hPos.x - sPos.x < 0) {
            dx = -1;
            this->direction = 3;
        }
        else {
            dx = 0;
        }

        if (hPos.y - sPos.y > 0) {
            dy = 1;
            this->direction = 2;
        }
        else if (hPos.y - sPos.y < 0) {
            dy = -1;
            this->direction = 0;
        }
        else {
            dy = 0;
        }

        // test whether the new position of the creature is valid
        Position newPosition = sPos;
        newPosition.y += dy;
        newPosition.x += dx;

        // if the new position is valid, then set the creature's position to its new position
        if (isPositionValid(newPosition))
        {
            sPos = newPosition;
        }
    }

    // save hero's previous position
    this->heroPrevPos = hPos;
    this->heroPrevDir = hero.getDirection();
}

// Get the creature direction
int Creature::getDirection() { return this->direction; }

// Set the creature direction
void Creature::setDirection(int direction) { this->direction = direction; }