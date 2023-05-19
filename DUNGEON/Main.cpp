// Name: B11115010 鍾詩靈	B11115014 魏美芳
// Date: May 19, 2023
// Last Update: May 20, 2023
// Problem statement: This C++ program implements a dungeon game with 3 different enemies and having other extra features

#include "main.h"
#include "Hero.h"
#include "Creature.h"
#include "Scarfy.h"
#include "Bomber.h"
#include <map>
#include <vector>
#include <fstream>

// Initialize hero
Hero gHero(2, 2);

// Constant board value
const char GWALL = 'O';
const char GNOTHING = ' ';

int GWIDTH = -1;
int GHEIGHT = -1;
const int MIN_SIZE = 4;
const double gTimeLog = 0.033;

// Distance for canSee function
const int gDistance = 4;

// Initialize game board
char** gBoard;

// Initalize enum for input key
enum ValidInput
{
	EW = 0,
	ES = 1,
	EA = 2,
	ED = 3,
	ESPACE = 4,
	EESC = 5,
	ESave = 6,
	ELoad = 7,
	INVALID,
};

// Update key press
void keyUpdate(bool key[]);

// Setup the board
void setupBoard(int rowN, int colN);

// Draw the board
void draw(void);

// Print information
void drawInfo(void);

// Update the game state after each key press
void update(bool key[]);

// Save and load game function
void saveMap();
void loadMap();

// Initialize vectors to store game's objects
std::vector<Trigger*> gTriggers;
std::vector<Creature*> gCreatures;
std::vector<Bomber*> gBombers;

// Intialize different enemy type counts 
int creatureCount = 0;
int scarfyCount = 0;
int bomberCount = 0;

int main(int argc, char** argv)
{
	// Random seed
	srand(time(NULL));

	// Initialize map of key and whether the key is pressed or not
	bool gKeyState[ValidInput::INVALID];

	// Set all pressed state to false
	for (int i = 0; i < ValidInput::INVALID; i++) {
		gKeyState[i] = false;
	}

	// Asking user input for the valid size of the custom board
	while (true) {
		std::cout << "Enter Width: ";
		std::cin >> GWIDTH;
		std::cout << "Enter Height: ";
		std::cin >> GHEIGHT;
		system("CLS");
		if (GHEIGHT < MIN_SIZE || GWIDTH < MIN_SIZE) {
			std::cout << "Illegal, both has to be larger than " << MIN_SIZE << std::endl;
		}
		else {
			break;
		}
	}

	// Setup a clear dungeon
	setupBoard(GHEIGHT, GWIDTH);

	// Draw the board and information
	draw();
	drawInfo();

	// Variable for game loop
	clock_t startT, endT;
	startT = clock();
	endT = clock();

	// Run the game loop
	while (!gKeyState[ValidInput::EESC]) {
		// Compute the time lap
		double timeFrame = (double)(endT - startT) / CLOCKS_PER_SEC;

		// Execute the game loop
		if (timeFrame >= gTimeLog) {
			update(gKeyState);
			startT = clock();
		}

		// Update the key
		keyUpdate(gKeyState);
		endT = clock();

		// Key press for saving and loading game
		if (gKeyState[ValidInput::ESave]) {
			saveMap();
			gKeyState[ValidInput::ESave] = false;
		}
		else if (gKeyState[ValidInput::ELoad]) {
			loadMap();
			gKeyState[ValidInput::ELoad] = false;
		}
	}

	//
	system("pause");

	return 0;
}

// Intent: Accept user key input and determine the pressed character based on the input
// Pre: Key parameter must be an array of booleans, representing key state
// Post: Set the pressed key value to true and the others to false
void keyUpdate(bool key[])
{
	//
	for (int i = 0; i < ValidInput::INVALID; i++) {
		key[i] = false;
	}

	//
	char input = _getch();

	//
	switch (input) {
	case 'w':
		key[ValidInput::EW] = true;
		break;
	case 's':
		key[ValidInput::ES] = true;
		break;
	case 'a':
		key[ValidInput::EA] = true;
		break;
	case 'd':
		key[ValidInput::ED] = true;
		break;
	case ' ':
		key[ValidInput::ESPACE] = true;
		break;
	case 27:
		key[ValidInput::EESC] = true;
		break;
	case '1':
		key[ValidInput::ESave] = true;
		break;
	case '2':
		key[ValidInput::ELoad] = true;
		break;
	default:
		break;
	}
}

// Intent: Check whether a position is valid and does not clash with other things
// Pre: pos Position Class as the parameter
// Post: Return true if the new position is valid and did not clash with other things, otherwise return false
bool isPositionValid(Position& pos)
{
	if (pos.y > GHEIGHT || pos.x > GWIDTH || pos.y < 0 || pos.x < 0) {
		return false;
	}

	// Check whether it is an empty space
	if (gBoard[pos.y][pos.x] != GNOTHING)
		return false;

	return true;
}

// Intent: Implement clip algorithm, which return a value is within the specified range
// Pre: Takes in three arguments, n, minimum, and maximum, and returns a float value
// Post: If n is less than minimum, it returns the value of minimum. If the value of n is greater than maximum, it returns the value of maximum. Otherwise, returns the value of n
float clip(float n, float minimum, float maximum)
{
	return std::max(minimum, std::min(n, maximum));
}

// Intent: To check whether the creature can see the hero in range using clip algorithm
// Pre : Creature position, hero position, and empty position that will act as direction are needed
// Post : Return the boolean whether the creature can see the hero or not
bool canSee(Position cPos, Position hPos, Position& dir, int viewDistance)
{
	// the dir_x and dir_y value are call-by-refernce
	dir.x = (int)clip((float)(hPos.x - cPos.x), -1.f, 1.f); // clip the value between -1 ~ 1
	dir.y = (int)clip((float)(hPos.y - cPos.y), -1.f, 1.f);

	// 
	int count = 0;

	//
	do {
		// spot the target position
		if (cPos + dir * count == hPos) {
			return true;
		}

		count++;
	} while (count < viewDistance); // check the range in 4 units

	return false;
}

// Intent: Set up the game board
// Pre : None
// Post : The game board is set up with walls on the edges and empty spaces in the center
void setupBoard(int rowN, int colN)
{
	// Allocate & init game board using 2d dynamic array
	gBoard = new char* [rowN];

	//
	for (int i = 0; i < rowN; i++) {
		gBoard[i] = new char[colN];

		for (int j = 0; j < colN; j++) {
			if (i == 0 || i == rowN - 1 || j == 0 || j == colN - 1)
				gBoard[i][j] = GWALL;
			else
				gBoard[i][j] = GNOTHING;
		}
	}

	//
	std::vector<std::vector<bool>> validFlags(GHEIGHT);

	// Setup for (random) position of all elements and implementation of game board using 2d dynamic array (elements has to be randomly generated within canvas)
	for (int i = 0; i < GHEIGHT; i++) {
		//
		validFlags[i].resize(GWIDTH);

		// 
		for (int j = 0; j < GWIDTH; j++) {
			validFlags[i][j] = gBoard[i][j] == GWALL ? false : true;
		}
	}

	//
	auto getRandomPos = [&rowN, &colN]() {
		return Position((int)(rand() % colN), (int)(rand() % rowN));
	};

	//
	auto getValidRandomPos = [&validFlags, &getRandomPos]() {
		//
		while (true) {
			Position pos = getRandomPos();

			//
			if (validFlags[pos.y][pos.x]) {
				return pos;
			}
		}
	};

	// 
	Position hPos = getValidRandomPos();
	validFlags[hPos.y][hPos.x] = false;
	gHero.setPos(hPos);

	//
	Creature* waddleDee = new Creature();
	gCreatures.push_back(waddleDee);
	creatureCount++;

	//
	Creature* scarfy = new Scarfy();
	gCreatures.push_back(scarfy);
	scarfyCount++;

	//
	Creature* bomber = new Bomber();
	gCreatures.push_back(bomber);
	bomberCount++;

	//
	for (auto& creature : gCreatures) {
		Position cPos = getValidRandomPos();
		validFlags[cPos.y][cPos.x] = false;
		creature->setPos(cPos);
	}

	//
	for (int i = 0; i < 2; i++) {
		Trigger* trigger = new Trigger();
		Position tPos = getValidRandomPos();
		validFlags[tPos.y][tPos.x] = false;
		trigger->setPos(tPos);
		gTriggers.push_back(trigger);
	}
}

// Intent: Draw the whole board
// Pre: None
// Post: Show the dungeon map on screen
void draw()
{
	// Add the hero into the board
	std::vector<std::vector<char>> drawBoard(GHEIGHT);

	// Setup and implementation of drawing board using 2d vector
	for (int i = 0; i < GHEIGHT; i++) {
		drawBoard[i].resize(GWIDTH);
		for (int j = 0; j < GWIDTH; j++) {
			drawBoard[i][j] = gBoard[i][j];
		}
	}

	// Draw the triggers using for loop on drawBoard
	for (int i = 0; i < gTriggers.size(); i++) {
		Position t = gTriggers[i]->getPos();
		drawBoard[t.y][t.x] = gTriggers[i]->getIcon();
	}

	// Draw the creatures using for loop on drawBoard
	for (int i = 0; i < gCreatures.size(); i++) {
		Position c = gCreatures[i]->getPos();
		drawBoard[c.y][c.x] = gCreatures[i]->getIcon();
	}

	// Draw the bomber damage radius range on drawBoard when it has spotted the hero
	for (auto& creature : gCreatures) {
		// Set condition to find Bomber type of creature
		bool isBomber = creature->getIcon() == 'B' || creature->getIcon() == '&';

		// Check whether bomber is still alive and seen the hero
		if (isBomber && creature->getHasSeen() && creature->getHealth()) {
			// Get the position of the active bomber
			Position b = creature->getPos();

			// Set radius to 1
			int radius = 1;

			// Draw the damage radius around the bomber on drawBoard
			for (int i = b.y - radius; i <= b.y + radius; i++) {
				for (int j = b.x - radius; j <= b.x + radius; j++) {
					// Set up test position
					Position testPosition(j, i);

					// If the position on the board is empty and valid, then set that position as the radius mark
					if (isPositionValid(testPosition) && drawBoard[i][j] == GNOTHING) {
						drawBoard[i][j] = '.';
					}
				}
			}

		}
	}

	// Update the hero information
	Position h = gHero.getPos();
	drawBoard[h.y][h.x] = gHero.getIcon();

	// Draw the board
	for (int i = 0; i < GHEIGHT; i++) {
		for (int j = 0; j < GWIDTH; j++) {
			std::cout << drawBoard[i][j]; //  output
		}
		std::cout << "\n";
	}
}

// Intent: Print and output any information
// Pre: None
// Post: Print information if the creature found the hero
void drawInfo(void)
{
	std::cout << "The hero is level " << gHero.getLevel() << "(" << gHero.getExp() << "/" << gHero.getMaxExp() << " to level up)" << std::endl;
	std::cout << "The hero has " << gHero.getHP() << " hp" << std::endl;
	std::cout << "Use wsad key to move Hero " << gHero.getIcon() << std::endl;
	std::cout << "Every time you step on a trigger T, the hero gets 10 exp." << std::endl;
	std::cout << "(ESC) Exit (1) Save (2) Load" << std::endl;
}

// Intent: Update the state of the board
// Pre: The user key input as the parameter
// Post: The board is updated with the newest change of Hero and Creature based on the user key input
void update(bool key[])
{
	// 
	system("CLS");

	//
	Position delta;

	// 
	bool hasInput = false;

	//
	if (key[ValidInput::EW]) {
		delta -= Position(0, 1);
		hasInput = true;
	}
	else if (key[ValidInput::ES]) {
		delta += Position(0, 1);
		hasInput = true;
	}
	else if (key[ValidInput::EA]) {
		delta = delta - Position(1, 0);
		hasInput = true;
	}
	else if (key[ValidInput::ED]) {
		delta = delta + Position(1, 0);
		hasInput = true;
	}
	else {
		//
		bool allInvalid = true;

		//
		for (int i = 0; i < ValidInput::INVALID; i++) {
			if (key[i]) {
				allInvalid = false;
				break;
			}
		}

		//
		if (allInvalid)
			std::cout << "invalid input\n";
	}
	if (hasInput) {
		gHero.move(delta);
	}

	// Manipulate update of the triggers using while loop
	for (int i = 0; i < gTriggers.size(); i++) {
		gTriggers[i]->update(gHero);
	}

	// Manipulate update of the creatures using while loop
	for (int i = 0; i < gCreatures.size(); i++) {
		gCreatures[i]->update(gHero);
	}

	// Setup deletedIndex vector as index marker
	std::vector<int> deletedIndex;

	// Find creature with hp = 0
	for (int i = gCreatures.size() - 1; i >= 0; i--) {
		// Push the index of dead creature to the vector
		if (!gCreatures[i]->getHealth()) {
			deletedIndex.push_back(i);
		}
	}

	// Delete dead creature
	for (int& index : deletedIndex) {
		// Remove the object from memory
		delete gCreatures[index];

		// Remove the creature from the gCreatures vector
		gCreatures.erase(gCreatures.begin() + index);
	}

	// Draw map and print information
	draw();
	drawInfo();
}

// Intent: 
// Pre:
// Post: 
void saveMap() {
	// 
	system("CLS");

	//
	std::cout << "Input file name for saving or Exit to leave saving." << std::endl;
	std::cout << "Input: ";

	//
	std::string input;
	std::cin >> input;

	//
	if (input.compare("Exit") == 0)
		return;

	//
	std::ofstream oStream(input + ".txt");
	oStream << GWIDTH << " " << GHEIGHT << std::endl;

	//
	for (int i = 0; i < GHEIGHT; i++) {
		for (int j = 0; j < GWIDTH; j++) {
			oStream << gBoard[i][j];
		}
		oStream << std::endl;
	}
	oStream << std::endl;

	//
	oStream << gHero.getPos() << " " << gHero.getHP() << " " << gHero.getLevel() << " " << gHero.getExp() << " " << gHero.getMaxExp() << std::endl;

	//
	oStream << gCreatures.size() << std::endl;

	//
	for (int i = 0; i < gCreatures.size(); i++) {
		//
		char currentIcon = gCreatures[i]->getIcon();

		//
		if (currentIcon == 'C' || currentIcon == '!') {
			oStream << "C ";
		}
		else if (currentIcon == 'S' || currentIcon == '@') {
			oStream << "S ";
		}
		else if (currentIcon == 'B' || currentIcon == '&') {
			oStream << "B ";
		}

		//
		oStream << gCreatures[i]->getPos() << " " << gCreatures[i]->getHealth() << std::endl;
	}

	//
	oStream << gTriggers.size() << std::endl;

	//
	for (int i = 0; i < gTriggers.size(); i++) {
		oStream << gTriggers[i]->getPos() << std::endl;
	}

	//
	oStream.close();
}

// Intent: 
// Pre:
// Post:
void loadMap() {
	// 
	system("CLS");

	//
	std::cout << "Input file name for loading or Exit to leave loading." << std::endl;
	std::cout << "Input: ";

	//
	std::string input;
	std::cin >> input;

	//
	if (input.compare("Exit") == 0)
		return;

	//
	std::ifstream iStream(input + ".txt");
	if (!iStream.is_open())
		return;

	// Reset the dungeon board
	for (int i = 0; i < GHEIGHT; i++) {
		delete[] gBoard[i];
	}
	delete[] gBoard;

	//
	for (int i = 0; i < gCreatures.size(); i++)
		delete gCreatures[i];
	gCreatures.clear();

	// 
	for (int i = 0; i < gTriggers.size(); i++)
		delete gTriggers[i];
	gTriggers.clear();

	//
	iStream >> GWIDTH >> GHEIGHT;

	//
	char _c;
	iStream.get(_c);

	//
	gBoard = new char* [GHEIGHT];

	//
	for (int i = 0; i < GHEIGHT; i++) {
		//
		gBoard[i] = new char[GWIDTH];

		//
		for (int j = 0; j < GWIDTH; j++) {
			iStream.get(gBoard[i][j]);
		}

		//
		iStream.get(_c);
	}

	//
	Position pos;
	int hp, level, exp, maxExp;
	iStream >> pos >> hp >> level >> exp >> maxExp;

	//
	gHero.setPos(pos);
	gHero.setHP(hp);
	gHero.setLevel(level);
	gHero.setExp(exp);
	gHero.SetMaxExp(maxExp);

	//
	int creatureN;
	iStream >> creatureN;

	//
	for (int i = 0; i < creatureN; i++) {
		char iconType = ' ';
		int hp = 1;
		iStream >> iconType >> pos >> hp;

		//
		Creature* creature = nullptr;

		//
		if (iconType == 'C') {
			creature = new Creature();
		}
		else if (iconType == 'S') {
			creature = new Scarfy();
		}
		else if (iconType == 'B') {
			creature = new Bomber();
		}

		//
		creature->setPos(pos);
		creature->getHealth(hp);

		//
		gCreatures.push_back(creature);
	}

	//
	int triggerN;
	iStream >> triggerN;

	//
	for (int i = 0; i < triggerN; i++) {
		iStream >> pos;

		//
		Trigger* trigger = new Trigger();
		trigger->setPos(pos);
		gTriggers.push_back(trigger);
	}

	//
	iStream.close();
}