#pragma once
#include <string>
#include "Position.h"
#include "Hero.h"

class Hero; // Forward declaration of Hero class

class Item {
protected:
    Position sPos;
    char sIcon;
    std::string sImgUrl;
    bool sIsTriggered;

public:
    Item();
    Item(int x, int y);
    Item(const Item& ref);
    virtual void update(Hero& hero) const;
    void setPos(const Position& pos);
    void setPos(int x, int y);
    Position getPos() const;
    char getIcon() const;
    std::string getImgUrl();
    void setImgUrl(std::string url);
    void setIcon(char& icon);
    char getIcon();
};
