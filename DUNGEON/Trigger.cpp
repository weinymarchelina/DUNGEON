#include "Trigger.h"

//
Trigger::Trigger() : _exp(10) {
    _icon = 'T';
}

//
Trigger::Trigger(int x, int y) : _exp(10) {
    _pos.x = x;
    _pos.y = y;
    _icon = 'T';
}

//
Trigger::Trigger(const Trigger& ref) {
    *this = ref;
}

//
void Trigger::update(Hero& hero) const {
    //
    if (hero.getPos().x == _pos.x && hero.getPos().y == _pos.y) {
        hero.gainEXP(_exp);
    }
}

//
void Trigger::setPos(const Position& pos) {
    this->_pos = pos;
}

//
void Trigger::setPos(int x, int y) {
    this->_pos.x = x;
    this->_pos.y = y;
}

//
Position Trigger::getPos() const {
    return this->_pos;
}

//
char Trigger::getIcon() const {
    return this->_icon;
}

//
int Trigger::getExpAmount() const {
    return this->_exp;
}

