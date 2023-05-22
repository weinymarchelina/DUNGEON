#include "Trigger.h"

// The constructor initializes sExp to 10 and sIcon to 'T
Trigger::Trigger() : sExp(10) {
    sIcon = 'T';
}

// The constructor takes x and y as parameters, initializes sExp to 10, sets sPos.x to x, sPos.y to y, and sIcon to 'T'
Trigger::Trigger(int x, int y) : sExp(10) {
    sPos.x = x;
    sPos.y = y;
    sIcon = 'T';
}

// The copy constructor creates a new Trigger object by copying the values from ref
Trigger::Trigger(const Trigger& ref) {
    *this = ref;
}

// The update function checks if hero's position matches Trigger's position and if so, the hero gains experience equal to sExp
void Trigger::update(Hero& hero) const {
    // If the positions match, the hero gains experience points equal to the value stored in the sExp member variable
    if (hero.getPos().x == sPos.x && hero.getPos().y == sPos.y) {
        hero.gainEXP(sExp);
    }
}

// The getExpAmount function returns the value of sExp
int Trigger::getExpAmount() const {
    return this->sExp;
}