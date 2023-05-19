#include "Position.h"

Position::Position() : x(0), y(0) {}
Position::Position(int x, int y) : x(x), y(y) {}
Position::Position(const Position& ref) { *this = ref; }

Position Position::operator+(const Position& rhs) const {
	return Position(x + rhs.x, y + rhs.y);
}

Position& Position::operator+=(const Position& rhs) {
	x += rhs.x;
	y += rhs.y;
	return *this;
}

Position Position::operator-(const Position& rhs) const {
	return Position(x - rhs.x, y - rhs.y);
}

Position& Position::operator-=(const Position& rhs) {
	x -= rhs.x;
	y -= rhs.y;
	return *this;
}

Position Position::operator*(int scale) const {
	return Position(x * scale, y * scale);
}

bool Position::operator==(const Position& rhs) const {
	return x == rhs.x && y == rhs.y;
}

bool Position::operator!=(const Position& rhs) const {
	return !(*this == rhs);
}

std::ostream& operator<<(std::ostream& oStream, const Position& pos) {
	return oStream << pos.x << " " << pos.y;
}

std::istream& operator>>(std::istream& iStream, Position& pos) {
	return iStream >> pos.x >> pos.y;
}
