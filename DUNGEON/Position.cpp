#include "Position.h"

//The default constructor initializes x and y to 0
Position::Position() : x(0), y(0) {}

//The constructor takes x and y as parameters and assigns them to the respective member variables
Position::Position(int x, int y) : x(x), y(y) {}

//The copy constructor creates a new Position object by copying the values from ref
Position::Position(const Position& ref) { *this = ref; }

//Overloads the addition operator and returns a new Position object that is the sum of the current Position object and the rhs Position object
Position Position::operator+(const Position& rhs) const {
	return Position(x + rhs.x, y + rhs.y);
}

//Overloads the compound addition assignment operator and adds the rhs Position object to the current Position object, modifying it in place
Position& Position::operator+=(const Position& rhs) {
	x += rhs.x;
	y += rhs.y;
	return *this;
}

//Overloads the subtraction operator and returns a new Position object that is the difference between the current Position object and the rhs Position object
Position Position::operator-(const Position& rhs) const {
	return Position(x - rhs.x, y - rhs.y);
}

//Overloads the compound subtraction assignment operator and subtracts the rhs Position object from the current Position object, modifying it in place
Position& Position::operator-=(const Position& rhs) {
	x -= rhs.x;
	y -= rhs.y;
	return *this;
}

//Overloads the multiplication operator and returns a new Position object that is the result of scaling the current Position object by the scale value
Position Position::operator*(int scale) const {
	return Position(x * scale, y * scale);
}

//Overloads the equality operator and returns true if the x and y values of the current Position object are equal to the corresponding values of the rhs Position object
bool Position::operator==(const Position& rhs) const {
	return x == rhs.x && y == rhs.y;
}

//Overloads the inequality operator and returns true if the x and y values of the current Position object are not equal to the corresponding values of the rhs Position object
bool Position::operator!=(const Position& rhs) const {
	return !(*this == rhs);
}

//Overloads the stream insertion operator and allows a Position object to be printed to an output stream
std::ostream& operator<<(std::ostream& oStream, const Position& pos) {
	return oStream << pos.x << " " << pos.y;
}

//Overloads the stream extraction operator and allows a Position object to be read from an input stream
std::istream& operator>>(std::istream& iStream, Position& pos) {
	return iStream >> pos.x >> pos.y;
}