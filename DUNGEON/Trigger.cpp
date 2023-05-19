#include "Trigger.h"

void Trigger::update(Hero& hero) const {
	if (hero.getPos().x == _pos.x && hero.getPos().y == _pos.y) {
		hero.gainEXP(_exp);
	}
}