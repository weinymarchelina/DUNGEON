#include "Scarfy.h"

//The default constructor of the Scarfy class initializes the member variables
Scarfy::Scarfy() {
    this->sIcon = 'S';
    this->normalIcon = 'S';
    this->triggeredIcon = '@';
    this->power = 5;
    this->viewDistance = 10;
    this->chaseHeroProbability = 70;
    this->awareness = true;
    this->hp = 5;
}