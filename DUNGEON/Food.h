#pragma once
#include <string>
#include "Item.h"

class Food : public Item {
private:
    int sBoost;
    int sTomatoRate;

public:
    Food();
    Food(int x, int y);
    Food(const Food& ref);
    void update(Hero& hero) const override;
    int getTomatoRate();
    void setTomatoRate(int rate);
    void generateFoodType();
};