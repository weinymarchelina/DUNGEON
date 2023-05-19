#pragma once
#include <string>
#include "Position.h"
#include "Hero.h"

class Hero; // Forward declaration of Hero class

class Trigger {
private:
    Position _pos;
    char _icon;
    int _exp;

public:
    Trigger();
    Trigger(int x, int y);
    Trigger(const Trigger& ref);
    void update(Hero& hero) const;
    void setPos(const Position& pos);
    void setPos(int x, int y);
    Position getPos() const;
    char getIcon() const;
    int getExpAmount() const;
};
