#pragma once
#include "Creature.h"
#include "Hero.h"

class Bomber : public Creature {
private:
    char deadIcon;

public:
    Bomber();

    void updateDamage(Hero& hero) override;
};
