#pragma once
#include <string>
#include "Item.h"
#include "Hero.h"

class Hero; // Forward declaration of Hero class

class Trigger : public Item {
private:
    int sExp;

public:
    Trigger();
    Trigger(int x, int y);
    Trigger(const Trigger& ref);
    void update(Hero& hero) const override;
    int getExpAmount() const;
};
