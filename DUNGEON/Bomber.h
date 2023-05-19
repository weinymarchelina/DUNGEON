#pragma once
#include "Creature.h"
#include "Hero.h" // Assuming Hero class is included

class Bomber : public Creature {
private:
    char deadIcon;

public:
    Bomber();

    void updateDamage(Hero& hero) override;
};
