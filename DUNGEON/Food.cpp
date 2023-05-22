#include "Food.h"

// The constructor initializes sBoost to 10 and sIcon to 'T
Food::Food() : sBoost(10) {
    sIcon = 'F';
    sTomatoRate = 10;
    this->generateFoodType();
}

// The constructor takes x and y as parameters, initializes sExp to 10, sets sPos.x to x, sPos.y to y, and sIcon to 'T'
Food::Food(int x, int y) : sBoost(10) {
    sPos.x = x;
    sPos.y = y;
    sIcon = 'F';
    sTomatoRate = 10;
    this->generateFoodType();
}

// The copy constructor creates a new Food object by copying the values from ref
Food::Food(const Food& ref) {
    *this = ref;
}

// The update function checks if hero's position matches Food's position and if so, the hero gains + 10 to its HP
void Food::update(Hero& hero) const {
    // If the positions match, the hero gains + 10 hp recovery
    if (hero.getPos().x == sPos.x && hero.getPos().y == sPos.y) {
        if (this->sIcon == 'M') {
            hero.setHP(100);
        }
        else {
            int newHp = hero.getHP() + sBoost;

            if (newHp > 100) {
                newHp = 100;
            }

            hero.setHP(newHp);
        }
    }
}

// Get tomato rate
int Food::getTomatoRate() {
    return this->sTomatoRate;
}

// Set tomato rate
void Food::setTomatoRate(int rate) {
    this->sTomatoRate = rate;
}

// Generate food type
void Food::generateFoodType() {
    int foodProbability = rand() % 101;

    if (foodProbability <= this->sTomatoRate) {
        this->sIcon = 'M';
    }
    else {
        foodProbability = rand() % 5;

        switch (foodProbability) {
        case 0:
            this->sIcon = '0';
            break;

        case 1:
            this->sIcon = '1';
            break;

        case 2:
            this->sIcon = '2';
            break;

        case 3:
            this->sIcon = '3';
            break;

        case 4:
            this->sIcon = '4';
            break;
        }
    }
}