#pragma once
#include <iostream>

class Position {
public:
	int x, y; // X, y Coordinate

	Position();
	Position(int x, int y);
	Position(const Position& ref);

	Position operator+(const Position& rhs) const;
	Position& operator+=(const Position& rhs);
	Position operator-(const Position& rhs) const;
	Position& operator-=(const Position& rhs);
	Position operator*(int scale) const;
	bool operator==(const Position& rhs) const;
	bool operator!=(const Position& rhs) const;

	friend std::ostream& operator<<(std::ostream& oStream, const Position& pos);
	friend std::istream& operator>>(std::istream& iStream, Position& pos);

	bool isInRange(int width, int height);
};
