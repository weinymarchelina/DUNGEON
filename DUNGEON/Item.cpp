#include "Item.h"
 
// The constructor sIcon to 'T
Item::Item() {
    sIcon = 'T';
    this->sIsTriggered = false;
}

// The constructor takes x and y as parameters, sets sPos.x to x, sPos.y to y, and sIcon to 'T'
Item::Item(int x, int y) {
    sPos.x = x;
    sPos.y = y;
    sIcon = 'T';
    this->sIsTriggered = false;
}

// The copy constructor creates a new Item object by copying the values from ref
Item::Item(const Item& ref) {
    *this = ref;
}

// Update template
void Item::update(Hero& hero) {
}

// Get isTriggered
bool Item::getHasTriggered() {
    return this->sIsTriggered;
}

// Set isTriggered 
void Item::setHasTriggered(bool isTriggered) {
    this->sIsTriggered = isTriggered;
}

// The setPos function sets the sPos member variable to pos
void Item::setPos(const Position& pos) {
    this->sPos = pos;
}

// The setPos function sets sPos.x to x and sPos.y to y
void Item::setPos(int x, int y) {
    this->sPos.x = x;
    this->sPos.y = y;
}

// The getPos function returns a copy of sPos
Position Item::getPos() const {
    return this->sPos;
}

// The getIcon function returns the value of sIcon
char Item::getIcon() const {
    return this->sIcon;
}

// Get the imgUrl
std::string Item::getImgUrl() {
    return this->sImgUrl;
}

// Set the imgUrl
void Item::setImgUrl(std::string url) {
    this->sImgUrl = url;
}

// Set icon
void Item::setIcon(char& icon)
{
    this->sIcon = icon;
}

// Get Icon
char Item::getIcon()
{
    return this->sIcon;
}