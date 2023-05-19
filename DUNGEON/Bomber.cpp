#include "Bomber.h"

Bomber::Bomber() {
    this->deadIcon = '0';
    this->sIcon = 'B';
    this->normalIcon = 'B';
    this->triggeredIcon = '&';
    this->power = 10;
    this->viewDistance = 3;
    this->chaseHeroProbability = 85;
    this->awareness = true;
    this->hp = 5;
}

void Bomber::updateDamage(Hero& hero) {
    if (!this->hp) {
        return;
    }

    if (!this->hasSeenHero) {
        this->hp = 5;
        return;
    }

    Position hPos = hero.getPos();

    this->hp--;

    if (this->hp == 0) {
        Position sPos = this->getPos();
        int dx = abs(hPos.x - sPos.x);
        int dy = abs(hPos.y - sPos.y);

        if (dx <= 1 || dy <= 1) {
            hero.damage(this->power);
        }
    }
}
