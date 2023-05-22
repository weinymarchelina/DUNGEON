#pragma once
#include <string>
#include "Item.h"

class Curry : public Item {
private:
    int sBoost;

public:
    Curry();
    Curry(int x, int y);
    Curry(const Curry& ref);
    void update(Hero& hero) const override;
};