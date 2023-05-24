// Name: B11115010 鍾詩靈	B11115014 魏美芳
// Date: May 30, 2023
// Last Update: May 30, 2023 
// Problem statement: This C++ program implements a dungeon game with 3 different enemies and several specials items with other features

#include "main.h"
#include "Creature.h"
#include "Scarfy.h"
#include "Bomber.h"
#include "Item.h"
#include "Trigger.h"
#include "Food.h"
#include <map>
#include <vector>
#include <fstream>
#include <stdexcept> 

// Initialize hero
Hero gHero(2, 2);

// Constant board value
const char GWALL = 'O';
const char GNOTHING = ' ';

int GWIDTH = -1;
int GHEIGHT = -1;
int MIN_SIZE = 10;
const double gTimeLog = 0.033;

// Distance for canSee function
const int gDistance = 4;

// Difficulty Level
int gDifficulties = 0;

// Invisible record
bool hasNotDamaged = true;

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
	EI = 5,
	EL = 6,
	EK = 7,
	EJ = 8,
	EP = 9,
	EESC = 10,
	ESave = 11,
	ELoad = 12,
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
std::vector<Creature*> gCreatures;
std::vector<Item*> gItems;

// Intialize different enemy type counts and item type counts
int deeCount = 0;
int scarfyCount = 0;
int bomberCount = 0;
int triggerCount = 0;
int foodCount = 0;
int tomatoDropRate = 0;

// Initialize gameOver and isWin
bool isGameOver = false;
bool isWin = false;

// Store all saved file name during the game
std::vector<std::string> savedFileName;

// Initialize records
std::vector<bool> gameRecords;

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

	// Set the 5 records to false
	for (int i = 0; i < 5; i++) {
		gameRecords.push_back(false);
	}

	// Set up ifstream for previous game data records
	std::ifstream recordsFile("game_data_records");

	// Read previous records if available; otherwise create new
	if (!recordsFile.is_open()) {
		std::ofstream newRecordsFile("game_data_records");
		newRecordsFile.close();
	}
	else {
		int recordType = -1;

		// Read the data on the records
		while (recordsFile >> recordType) {
			// If the record is valid (0 - 4), then set it to true according to the index
			if (recordType >= 0 && recordType <= 4) {
				gameRecords[recordType] = true;
			}
		}

		recordsFile.close();
	}
		 
	// Asing user input for difficulty level
	while (true) {
		std::string welcomeImage = R"(
                                                                                                    
         **********        ***********                      ,*.                                     
       ***%%%%%%%%***  ,***(%%%%%%%%****                ****%%%***                                  
      **%%%%%%%%%%******%%%%%%%%%%%%****               **%%%%%%%****                                
     ***%%%%%%%%%%**%%%%%%%%%%%%%(***/%%%***   ,,     ***%%%%%%%*******,    *******.     ********   
     **%%%%%%%%%%%%%%%%%%%%%%%*****%%%%%%%/***(%%%***/%%%/**%%%%%%%%%%%******%%%%%%*** ***%%%%%%*** 
    **(%%%%%%%%%%%%%%%%%%%*********%%%%%%***%%%%%%%%%%%%%%%**%%%%%%%%%%%%(**%%%%%%%*****%%%%%%%%****
    **%%%%%%%%%%%%%%%%%%******. .**********(%%%%%%%%%%%%%***%%%**%%%%%%%%%**/%%%%%%%**(%%%%%%%******
   **%%%%%%%%%%%%%%%%%%%%%***  **(%%%%%%***%%%%%%%%*****%%%%%*/####*%%%%%%***%%%%%%%*%%%%%%%%*****  
  ,**%%%%%%%%%%%%%%%%%%%%%%%*****%%%%%%%**%%%%%%%*****%%%%%%%%*###(%%%%%%(***%%%%%%%%%%%%%%*****.   
  **%%%%%%%%%%****(%%%%%%%%%%%**%%%%%%%***%%%%%%%****%%%%%%%%%%%%%%%%%%%******%%%%%%%%%%%%*****     
 ***%%%%%%%%%#*******%%%%%%%%%**%%%%%%%**%%%%%%%*****%%%%%%%%%%%%%%%%%********%%%%%%%%%%*****       
 **%%%%%%%%%%****   ***%%%%%%%**%%%%%(***%%%%%%******%%%%%%%%%%%%%************%%%%%%%%#*****        
  ***%%%%/******      ***************************** ********************%%%%%%%%%%%%%*****          
    **********           .****    ,**        .                       **(%%%%%%%%%%*******           
                                                                      ****************              
                                                                         *********                  

)";

		std::cout << welcomeImage << std::endl;

		// Displays accomplishment according to the game_data_records
		for (int i = 0; i < gameRecords.size(); i++) {
			// Filter out accomplishment for unachieved records
			if (!gameRecords[i]) {
				continue;
			}

			// Print out accomplishment based on the records
			switch (i)
			{
			case 0:
				std::cout << "ALL LEVELS COMPLETED\t: TRUE" << std::endl;
				break;
			case 1:
				std::cout << "EASY MODE\t: COMPLETED" << std::endl;
				break;
			case 2:
				std::cout << "MEDIUM MODE\t: COMPLETED" << std::endl;
				break;
			case 3:
				std::cout << "HARD MODE\t: COMPLETED" << std::endl;
				break;
			case 4:
				std::cout << "INVISIBLE MODE\t: COMPLETED" << std::endl;
				break;
			}
		}

		std::cout << std::endl;

		// Prints info for selecting game
		std::cout << "Please select the difficulties: " << std::endl;
		std::cout << "0) Custom " << std::endl;
		std::cout << "1) Easy " << std::endl;
		std::cout << "2) Medium " << std::endl;
		std::cout << "3) Hard " << std::endl;
		std::cout << "Enter Difficulties: ";
		std::cin >> gDifficulties;

		// Keep asking for the valid input
		if (gDifficulties < 0 || gDifficulties > 3) {
			std::cout << "Please select the correct mode" << std::endl;
		}
		else {
			break;
		}
	}

	// Setup board based on the difficulties preference
	if (gDifficulties == 0) {
		// Asking user input for the valid number of the enemies
		while (true) {
			std::cout << "Enter the number of Waddle Dee, Scarfy, and Bomber: ";
			std::cin >> deeCount;
			std::cin >> scarfyCount;
			std::cin >> bomberCount;

			// Keep asking for the valid input
			if (deeCount < 1 && scarfyCount < 1 && bomberCount < 1) {
				std::cout << "There should be at least 1 of each enemy type on the game" << std::endl;
			}
			else {
				break;
			}
		}

		// Asking user input for the valid number of the items
		while (true) {
			std::cout << "Enter the number of Trigger, Food, and Maxim Tomato Rate: ";
			std::cin >> triggerCount;
			std::cin >> foodCount;
			std::cin >> tomatoDropRate;

			// Keep asking for the valid input
			if (triggerCount < 1 && foodCount < 0 && tomatoDropRate < 0) {
				std::cout << "Please enter valid positive number (there should be at least 1 trigger on the game)" << std::endl;
			}
			else {
				break;
			}
		}

		// Calculate total object on the game
		int totalObjects = (deeCount + scarfyCount + bomberCount + triggerCount + foodCount + 5);

		// Asking user input for the valid size of the custom board
		while (true) {
			std::cout << "Enter Width: ";
			std::cin >> GWIDTH;
			std::cout << "Enter Height: ";
			std::cin >> GHEIGHT;
			// system("CLS");

			// Keep asking for the valid input
			if (GHEIGHT * GWIDTH < totalObjects) {
				std::cout << "The board need to be able contains " << totalObjects << " objects" << std::endl;
			}
			else {
				break;
			}
		}
	}
	else if (gDifficulties == 1) {
		GWIDTH = 17;
		GHEIGHT = 17;
		deeCount = 3;
		scarfyCount = 1;
		bomberCount = 1;
		triggerCount = 3;
		foodCount = 5;
		tomatoDropRate = 20;
	}
	else if (gDifficulties == 2) {
		GWIDTH = 15;
		GHEIGHT = 15;
		deeCount = 3;
		scarfyCount = 2;
		bomberCount = 2;
		triggerCount = 3;
		foodCount = 3;
		tomatoDropRate = 10;
	}
	else if (gDifficulties == 3) {
		GWIDTH = 12;
		GHEIGHT = 12;
		deeCount = 3;
		scarfyCount = 3;
		bomberCount = 3;
		triggerCount = 2;
		foodCount = 2;
		tomatoDropRate = 5;
	}

	// Clearing the console screen
	system("CLS");

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
	while (!gKeyState[ValidInput::EESC] && !isGameOver) {
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

	// Delete the file that was created when saving the game
	for (auto& filename : savedFileName) {
		std::string deletedFile = filename + ".txt";
		std::remove(deletedFile.c_str());
	}

	// Setup records for being not damaged
	if (hasNotDamaged) {
		gameRecords[4] = true;
	}

	// Setup records for completing the level
	if (isGameOver && isWin) {
		gameRecords[gDifficulties] = true;
	}

	// Setup records for completing all level
	if (gameRecords[1] && gameRecords[2] && gameRecords[3]) {
		gameRecords[0] = true;
	}

	// Open game_data_records
	std::ofstream newRecordsFile("game_data_records");

	// Save the records on game_data_records
	for (int i = 0; i < gameRecords.size(); i++) {
		if (gameRecords[i]) {
			newRecordsFile << i << std::endl;
		}
	}

	// Clsoe stream
	newRecordsFile.close();

	// Initialize and print gameover mark
	std::string gameOverPict = R"(

  .-_'''-.      ____    ,---.    ,---.    .-''-.              ,-----.    ,---.  ,---.   .-''-.  .-------.     
 '_( )_   \   .'  __ `. |    \  /    |  .'_ _   \           .'  .-,  '.  |   /  |   | .'_ _   \ |  _ _   \    
|(_ o _)|  ' /   '  \  \|  ,  \/  ,  | / ( ` )   '         / ,-.|  \ _ \ |  |   |  .'/ ( ` )   '| ( ' )  |    
. (_,_)/___| |___|  /  ||  |\_   /|  |. (_ o _)  |        ;  \  '_ /  | :|  | _ |  |. (_ o _)  ||(_ o _) /    
|  |  .-----.   _.-`   ||  _( )_/ |  ||  (_,_)___|        |  _`,/ \ _/  ||  _( )_  ||  (_,_)___|| (_,_).' __  
'  \  '-   .'.'   _    || (_ o _) |  |'  \   .---.        : (  '\_/ \   ;\ (_ o._) /'  \   .---.|  |\ \  |  | 
 \  `-'`   | |  _( )_  ||  (_,_)  |  | \  `-'    /         \ `"/  \  ) /  \ (_,_) /  \  `-'    /|  | \ `'   / 
  \        / \ (_ o _) /|  |      |  |  \       /           '. \_/``".'    \     /    \       / |  |  \    /  
   `'-...-'   '.(_,_).' '--'      '--'   `'-..-'              '-----'       `---`      `'-..-'  ''-'   `'-'   
                                                                                                              
)";
	std::cout << gameOverPict << std::endl;

	// Handle game over state
	if (isGameOver) {
		if (!isWin) {
			std::cout << "You lose!" << std::endl;
			std::string losePict = R"(
                                                                                                    
                                                                                                    
                                                                                                    
                                        ..........   .                                              
                                  .........................  .                                      
                                ........,(........,(*........... . .                                
                             ..........*  %......#   /.......... ...     . ...                      
                       ......,,,,.....&. %&.....%%  %*......................      ..                
                   .,......,,,,,,,,..,&&&&/.....%%%##...................................            
                 ,,,,,,,,,,,,,,,,,,,,/#&%%......###((....................................           
                ,,,,,,,,,,,,*,,,,,,,,.((/*......#//(.....,,,.................... .,.....            
               *****,,,,,****///////*,,,,,,,,,,......,*********...........,*,*,,,,.,,.......        
              *****/*///**********,,,,,,,&&&&&&&,............................................       
             ,**,,,,,**************,,,,,,,,%%%*,,,,,,,,,,..................*,,................      
           ,,,,,,,,,,,*/****************,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,*#**,...... . ...   .    
        ...,,,,,,,,,**/(*////****************,,,,,,,,,,,,,,,,,,,,,,,,,,,,,*/((./**,,,,,.....    .   
      .......,*****//((##/////////*****************,,,,,,,,,,,,,,,,,,,,,,**/(/   (//***,,........   
      ,,....,,*///((###  .///////////***********************,,,,,,,*******///        //**,........  
     *,,....,**///((/      /////////////////*****************************//*        ,*,,**,..... .  
     ,,..,,,**,,**/(.       ////////////////////////*******************///           ./*/*,..  . .  
    ,,.....****//(,           /(/(((////////////////////////////////*,*/                 (,,......  
   *,.....,*(#,                .//(((((((((((/////////////////,....,,,..                ,,,..... .  
   *,.,....                    //*****/(((((((((((((((((((/*,,,........,               **,...... ...
  *,,....,.,                   ************/(((((((((((/**,**,,,,,,.....              ,,/  /,,*,.*,*
 *,,...,*/*,,                  */*************//((   //***,***,,,,,,...,              .*/ ,.,.*..,..
 .(/,.*   ///                 .********,,,,***//((    //**,,,***,,,,,,,*                      *,,,*.
  (/*,     (,                  **///***,,,,**////      //**,,,*********                       ,***  
    *,/                        ,/////***,,**//(.         /*********//*,                             
                                 (////******//            /////*,,*/*,                              
                                  *,**/((////.             *(/*,,,,,,,                              
                                  //***,,///*               ,///*,.,,..                             
                                  (***,//***                 (*(***,,,,.                            
                                  ///(*,*/*,,               ,*//**,,,,,,.                           
                                  //*****,,,,,              ****/**,,.,,,                           
                                  //***,,,,,,,,             /*****,,,,,,                            
                                   /*,,,*,****               //***,....                             
                                   /*,,,,**/,                  /**,,..,                             
                                   /,,,,,,*                    ,***,,..                             
                                   *...,,,                     (((((//*                             
                                  ((((//(/.                    #((((///*                            
                                ((((((((/##                    ((/((((/(/                           
                             (((((((///##(/                   ##((#(((//(/(.                        
                          .%%#%#%%##/((%###(                   ,%&%%%#*/##                          ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
)";

			std::cout << losePict << std::endl;
		}
		else {
			std::cout << "You win!" << std::endl;
			std::string winPict = R"(
                                                                                                    
                                                                                                    
                                                                                                    
                                              ..                                                    
                                             *,,,*.                                                 
                                          *,,,.,.,,,*                                               
                                           ,*,,,,,**                                                
                                           .*** ,**,                                                
                                                                                                    
                                                                                    .*,**           
                .                    ./#####//.,......*,(##((/                   *,,,,,,,**         
              *,,*,              /##(*........................./##*   (##(#((##/  *,,.,,,,*         
          ,,,,,.,,,,*.       ,(##,.................................,#(..........,/#,***,,*          
           .*,.,,,,*       .((............,...........................,#..........*#                
            ***,.,**     ((,...........(/  /*.........#*  (/.......................#(               
                       /#*............//    #,.......(/   .#,....................../(               
                      ##..............((.   (........#(   (#/......................((               
                    .((...............#(##((#,.......(#(#((#,...................../#                
                  ,(((................((((((#......../#((((((................,(..*#                 
              /(#,./#...........,...../((#((#.........#((((#(.................#/##                  
           ((/.....#*........**,*,*,....*#/,...........(###.....*,,**,*.......((.                   
         (#........#.........,*****........*#((((((((#(.........,**,,*,.......,#                    
        (/...........,..,..................((/**,,*,,/((......................,(.                   
       /#......,#(#///***///((##*...........(/,*,*,,*/#,....................../(                    
       (#....((/****,*/**/*****//(#(..........##***/#.........................((                    
       ,(*..(#**********************##/......................................((                     
         ##(#/************************((*...................................*#*                     
           (#//************************/#(.................................(#,                      
           ,#/**************************/#(...............................##.                       
            #(***************************/#/............................#((#(             ,,        
             #(*/*************************(#.........................*##(***/((*         ,,.,***,   
         ..   ((**//**********************(#......................(#((*******/*((     ,**,,,,,,**   
   .****,,,*.   ##***/***************/**/*##................/(#(#(**************((.     **,.,,,,*,  
    ,*,,.,,,**    /((*******************/(/(#((##((#(#(##((//*****//**********,*/##      .***       
   .*,,,,.,*,       .#((///***********/(#,        /(/*/*/********************/*//(#                 
        .**.             /##((////((##*             ##/**********/*************/(#.                 
                                                       /(#((((((///////////((##(                    
                                                                                                    
                                                                                                    
                                                                                                   
			)";

			std::cout << winPict << std::endl;
		}
	}

	// systenm pause
	system("pause");

	return 0;
}

// Intent: Accept user key input and determine the pressed character based on the input
// Pre: Key parameter must be an array of booleans, representing key state
// Post: Set the pressed key value to true and the others to false
void keyUpdate(bool key[])
{
	// Initializes all elements of the key array to false
	for (int i = 0; i < ValidInput::INVALID; i++) {
		key[i] = false;
	}

	// Reads a character input from the user using _getch()
	char input = _getch();

	// Check the value of the input character
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
	case 'i':
		key[ValidInput::EI] = true;
		break;
	case 'l':
		key[ValidInput::EL] = true;
		break;
	case 'k':
		key[ValidInput::EK] = true;
		break;
	case 'j':
		key[ValidInput::EJ] = true;
		break;
	case 'p':
		key[ValidInput::EP] = true;
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
	// Return false if position is out of range
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

	// Integer variable count is initialized to 0
	int count = 0;

	// Checks if the position cPos plus the direction dir multiplied by count is equal to the target position hPos
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

	// It iterates over each row and column of the game board and assigns the appropriate value to each element
	for (int i = 0; i < rowN; i++) {
		gBoard[i] = new char[colN];

		for (int j = 0; j < colN; j++) {
			if (i == 0 || i == rowN - 1 || j == 0 || j == colN - 1)
				gBoard[i][j] = GWALL;
			else
				gBoard[i][j] = GNOTHING;
		}
	}

	// Creates a 2D vector of boolean values (validFlags) with a height of GHEIGHT
	std::vector<std::vector<bool>> validFlags(GHEIGHT);

	// Setup for (random) position of all elements and implementation of game board using 2d dynamic array (elements has to be randomly generated within canvas)
	for (int i = 0; i < GHEIGHT; i++) {
		// Resizes each row to have a width of GWIDTH
		validFlags[i].resize(GWIDTH);

		// Iterates over each row and column of the game board and assigns true to the corresponding element in validFlags if the element in gBoard is not equal to GWALL. Otherwise, it assigns false
		for (int j = 0; j < GWIDTH; j++) {
			validFlags[i][j] = gBoard[i][j] == GWALL ? false : true;
		}
	}

	// Generates a random position within the specified number of rows (rowN) and columns (colN)
	auto getRandomPos = [&rowN, &colN]() {
		return Position((int)(rand() % colN), (int)(rand() % rowN));
	};

	// Defines another lambda function getValidRandomPos that repeatedly calls getRandomPos until it generates a valid position
	auto getValidRandomPos = [&validFlags, &getRandomPos]() {
		// Repeatedly generates random positions using getRandomPos and checks if the corresponding element in validFlags is true
		while (true) {
			Position pos = getRandomPos();

			// If the generated position is valid, returns the generated position
			if (validFlags[pos.y][pos.x]) {
				return pos;
			}
		}
	};

	// Generates a random position (hPos) for the hero using getValidRandomPos
	Position hPos = getValidRandomPos();
	validFlags[hPos.y][hPos.x] = false;
	gHero.setPos(hPos);

	// Creates instances of the Creature classes
	for (int i = 0; i < deeCount; i++) {
		Creature* waddleDee = new Creature();
		gCreatures.push_back(waddleDee);
	}

	// Creates instances of the Scarfy classes
	for (int i = 0; i < scarfyCount; i++) {
		Creature* scarfy = new Scarfy();
		gCreatures.push_back(scarfy);
	}

	// Creates instances of the Bomber classes
	for (int i = 0; i < bomberCount; i++) {
		Creature* bomber = new Bomber();
		gCreatures.push_back(bomber);
	}

	// It generates valid random positions for each creature in gCreatures using getValidRandomPos
	for (auto& creature : gCreatures) {
		Position cPos = getValidRandomPos();
		validFlags[cPos.y][cPos.x] = false;
		creature->setPos(cPos);
	}

	// It creates two instances of the Trigger class, and put them into the gItems vector
	for (int i = 0; i < triggerCount; i++) {
		Trigger* trigger = new Trigger();
		gItems.push_back(trigger);
	}

	// It creates five instances of the Food class, and put them into the gItems vector
	for (int i = 0; i < foodCount; i++) {
		Food* food = new Food(tomatoDropRate);
		gItems.push_back(food);
	}

	// It generates valid random positions for each item in gItems using getValidRandomPos
	for (auto& item : gItems) {
		Position iPos = getValidRandomPos();
		validFlags[iPos.y][iPos.x] = false;
		item->setPos(iPos);
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

	// Draw the items using for loop on drawBoard
	for (int i = 0; i < gItems.size(); i++) {
		Position t = gItems[i]->getPos();
		drawBoard[t.y][t.x] = gItems[i]->getIcon();
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
	std::cout << "The hero has " << gHero.getAttack() << " attack" << std::endl;
	std::cout << "Use wsad key to move Hero " << gHero.getIcon() << std::endl;
	std::cout << "Use ikjl key to change the direction of hero " << gHero.getIcon() << std::endl;
	std::cout << "Use p key to let Hero " << gHero.getIcon() << " eats food or eats enemy in front of him (2 cells range)" << std::endl;
	std::cout << "Every time you step on a trigger T, the hero gets 10 exp." << std::endl;
	std::cout << "Once the hero level up, it gets additional attack boost." << std::endl;
	std::cout << "Hero wins the game if all enemies is defated." << std::endl;


	switch (gHero.getDirection()) {
	case 0:
		std::cout << "The hero faced up" << std::endl;
		break;
	case 1:
		std::cout << "The hero faced right" << std::endl;
		break;
	case 2:
		std::cout << "The hero faced down" << std::endl;
		break;
	case 3:
		std::cout << "The hero faced left" << std::endl;
		break;
	}

	std::cout << "(ESC) Exit (1) Save (2) Load" << std::endl;
}

// Intent: Get random valid position
// Pre: rowN and colN is needed
// Post: Return a valid position
Position getRandomValidPosition(int rowN, int colN) {
	// Get random new position
	Position newPos = Position((int)(rand() % colN), (int)(rand() % rowN));

	// Keep getting new position if the position is not valid
	while (!isPositionValid(newPos)) {
		newPos = Position((int)(rand() % colN), (int)(rand() % rowN));
	}

	return newPos;
}

// Intent: Update the state of the board
// Pre: The user key input as the parameter
// Post: The board is updated with the newest change of Hero and Creature based on the user key input
void update(bool key[])
{
	// Clearing the console screen
	system("CLS");

	// Declares a delta variable of type Position to represent the change in position
	Position delta;

	// A boolean variable hasInput is initialized as false to keep track of whether any valid input has been received
	bool hasInput = false;

	// The function checks the values in the key array to determine the input and also set direction of the hero
	if (key[ValidInput::EW]) {
		delta -= Position(0, 1);
		hasInput = true;
		gHero.setDirection(0);
	}
	else if (key[ValidInput::ES]) {
		delta += Position(0, 1);
		hasInput = true;
		gHero.setDirection(2);
	}
	else if (key[ValidInput::EA]) {
		delta = delta - Position(1, 0);
		hasInput = true;
		gHero.setDirection(3);
	}
	else if (key[ValidInput::ED]) {
		delta = delta + Position(1, 0);
		hasInput = true;
		gHero.setDirection(1);
	}
	else if (key[ValidInput::EI]) {
		gHero.setDirection(0);
	}
	else if (key[ValidInput::EK]) {
		gHero.setDirection(2);
	}
	else if (key[ValidInput::EJ]) {
		gHero.setDirection(3);
	}
	else if (key[ValidInput::EL]) {
		gHero.setDirection(1);
	}
	else if (key[ValidInput::EP]) {
		// Create new instance to get the boost power according to the difficulty level
		Food* newFood = new Food(tomatoDropRate);

		// Call swallow function
		gHero.swallow(GWIDTH, GHEIGHT, gCreatures, gItems, newFood->getBoost());
	}
	else {
		// If none of the directional keys are pressed
		bool allInvalid = true;

		// Prints "Invalid input" to indicate that no valid input was detected
		try {
			// Checks if all the input values in the key array are false
			for (int i = 0; i < ValidInput::INVALID; i++) {
				if (key[i]) {
					allInvalid = false;
					break;
				}
			}

			// Throw a runtime_error exception if all invalid
			if (allInvalid) {
				throw std::runtime_error("Invalid input!"); 
			}
		}
		catch (const std::exception& e) {
			// Catch the exception and print "Invalid input"
			std::cout << e.what() << std::endl;
		}
	}
	// If valid input is detected, calls the move function of the gHero object
	if (hasInput) {
		gHero.move(delta);
	}

	// Manipulate update of the items using while loop
	for (int i = 0; i < gItems.size(); i++) {
		gItems[i]->update(gHero);
	}

	// Setup deletedIndex vector as index marker
	std::vector<int> deletedIndex;

	// Handle items different isTriggered state
	for (int i = gItems.size() - 1; i >= 0; i--) {
		// Handle hasTriggered cases
		if (gItems[i]->getHasTriggered()) {
			// For trigger, set new random location (Trigger moves when trigerred); for food, food gone when eaten (only if the hero's hp is not max)
			if (gItems[i]->getIcon() == 'T') {
				//	Set a random valid position for the new position of the trigger
				Position newPos = getRandomValidPosition(GHEIGHT, GWIDTH);
				gItems[i]->setPos(newPos);
				gItems[i]->setHasTriggered(false);
			}
			else {
				// Add the deleted food to the vector
				deletedIndex.push_back(i);
			}
		}
	}

	// Delete eaten food
	for (int& index : deletedIndex) {
		// Remove the object from memory
		delete gItems[index];

		// Remove the food from the gItems vector
		gItems.erase(gItems.begin() + index);
	}

	//	Clear the deletedIndex to be used on gCreatures
	deletedIndex.clear();

	// Manipulate update of the creatures using while loop
	for (int i = 0; i < gCreatures.size(); i++) {
		gCreatures[i]->update(gHero);
	}

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

	// Check whether the hero has ever been hurted
	if (gHero.getHP() < 100) {
		hasNotDamaged = false;
	}

	// Check current game condition
	if (gHero.getHP() <= 0) {
		isGameOver = true;
		isWin = false;
	}
	else if (gCreatures.empty()) {
		isGameOver = true;
		isWin = true;
	}
}

// Intent: Save the map data to a file
// Pre: The game board, hero, creatures, and items are initialized
// Post: The map data is saved to a file
void saveMap() {
	// Clears the console screen
	system("CLS");

	// Prints header for the user to input file name
	std::cout << "Input file name for saving or Exit to leave saving." << std::endl;
	std::cout << "Input: ";

	// Declare string called input to save the file name
	std::string input = " ";

	// Prevents the user to name game_data_records or empty as file name to prevent overlaps 
	while (input == "game_data_records" || input == " ") {
		std::cout << "Input: ";
		std::cin >> input;
	}

	// If the user enters "Exit", the function returns and exits without saving
	if (input.compare("Exit") == 0) {
		return;
	}

	// Save the file name by storing it to vector
	savedFileName.push_back(input);

	// It creates an output file stream oStream with the specified file name concatenated with ".txt"
	std::ofstream oStream(input + ".txt");

	// Writes the width and height of the board
	oStream << GWIDTH << " " << GHEIGHT << std::endl;

	// Writes the dimensions of the game board (GWIDTH and GHEIGHT) to the file
	for (int i = 0; i < GHEIGHT; i++) {
		for (int j = 0; j < GWIDTH; j++) {
			oStream << gBoard[i][j];
		}
		oStream << std::endl;
	}
	oStream << std::endl;

	// Writes the hero's position, HP, level, experience, and maximum experience to the file
	oStream << gHero.getPos() << " " << gHero.getHP() << " " << gHero.getLevel() << " " << gHero.getExp() << " " << gHero.getMaxExp() << std::endl;

	// Writes the number of creatures in the gCreatures vector to the file
	oStream << gCreatures.size() << std::endl;

	// For each creature in the vector
	for (int i = 0; i < gCreatures.size(); i++) {
		// Retrieves the current creature's icon using gCreatures[i]->getIcon()
		char currentIcon = gCreatures[i]->getIcon();

		// Based on the icon value, it determines the appropriate identifier, and writes it to the file
		if (currentIcon == 'C' || currentIcon == '!') {
			oStream << "C ";
		}
		else if (currentIcon == 'S' || currentIcon == '@') {
			oStream << "S ";
		}
		else if (currentIcon == 'B' || currentIcon == '&') {
			oStream << "B ";
		}

		// Writes the creature's position (getPos()) and health (getHealth()) to the file
		oStream << gCreatures[i]->getPos() << " " << gCreatures[i]->getHealth() << std::endl;
	}

	// Writes the number of items in the gItems vector to the file
	oStream << gItems.size() << std::endl;

	// Writes the number of items in the gItems vector to the file
	for (int i = 0; i < gItems.size(); i++) {
		// Retrieves the current item's icon using gItems[i]->getIcon()
		char currentIcon = gItems[i]->getIcon();

		// Retrieves the current item's position using gItems[i]->getPos()
		Position itemPos = gItems[i]->getPos();

		// Writes the item icon to know the item type
		oStream << currentIcon << " ";

		// Writes the creature's position (getPos()) and health (getHealth()) to the file
		oStream << itemPos << " " << std::endl;
	}

	// The output file stream is closed
	oStream.close();
}

// Intent: Load a map from a file
// Pre: None
// Post: Map is loaded from the file and game state is updated accordingly
void loadMap() {
	// Clear the screen
	system("CLS");

	// Prompt for the input file name
	std::cout << "Input file name for loading or Exit to leave loading." << std::endl;
	std::cout << "Input: ";

	// Read the input file name
	std::string input;
	std::cin >> input;

	// Check if the user wants to exit
	if (input.compare("Exit") == 0)
		return;

	// Open the input file
	std::ifstream iStream(input + ".txt");

	while (!iStream.is_open()) {
		std::cout << "File is not found! Please input valid file." << std::endl;
		std::cout << "Input: ";
		std::cin >> input;
		iStream.open(input + ".txt");
	}

	// Reset the dungeon board
	for (int i = 0; i < GHEIGHT; i++) {
		delete[] gBoard[i];
	}
	delete[] gBoard;

	// Delete existing items and clear the gCreatures vectors
	for (int i = 0; i < gCreatures.size(); i++)
		delete gCreatures[i];
	gCreatures.clear();

	// Delete existing items and clear the gItems vector
	for (int i = 0; i < gItems.size(); i++)
		delete gItems[i];
	gItems.clear();

	// Read the dimensions of the map
	iStream >> GWIDTH >> GHEIGHT;

	// Declare char, and reads a single character from the input file stream
	char _c;
	iStream.get(_c);

	// Allocate memory for the new dungeon board
	gBoard = new char* [GHEIGHT];

	// Read the map data
	for (int i = 0; i < GHEIGHT; i++) {
		// Declare dynamic array of characters is allocated for each row using gBoard[i] = new char[GWIDTH]
		gBoard[i] = new char[GWIDTH];

		// Iterates over each column of the current row
		for (int j = 0; j < GWIDTH; j++) {
			iStream.get(gBoard[i][j]);
		}

		// Consume the newline character
		iStream.get(_c);
	}

	// Read the hero's position and attributes
	Position pos;
	int hp, level, exp, maxExp;
	iStream >> pos >> hp >> level >> exp >> maxExp;

	// Sets Hero's position, HP, level, exp, maxExp
	gHero.setPos(pos);
	gHero.setHP(hp);
	gHero.setLevel(level);
	gHero.setExp(exp);
	gHero.SetMaxExp(maxExp);

	// Read the number of creatures
	int creatureN;
	iStream >> creatureN;

	// Read the creature data and create the creatures
	for (int i = 0; i < creatureN; i++) {
		char iconType = ' ';
		int hp = 1;
		iStream >> iconType >> pos >> hp;

		// A pointer to a Creature object named creature is initialized as nullptr
		Creature* creature = nullptr;

		// Based on the value of iconType, a new creature object is created and assigned to the creature pointer
		if (iconType == 'C') {
			creature = new Creature();
		}
		else if (iconType == 'S') {
			creature = new Scarfy();
		}
		else if (iconType == 'B') {
			creature = new Bomber();
		}

		// Sets the creature position and Hp
		creature->setPos(pos);
		creature->setHealth(hp);

		// Creates creature object is added to the gCreatures vector 
		gCreatures.push_back(creature);
	}

	// Read the number of items
	int itemN;
	iStream >> itemN;

	// Read the item data and create the items
	for (int i = 0; i < itemN; i++) {
		char iconType = ' ';
		iStream >> iconType >> pos;

		// A pointer to a Item object named item is initialized as nullptr
		Item* item = nullptr;

		// Based on the value of iconType, a new item object is created and assigned to the item pointer
		if (iconType == 'T') {
			item = new Trigger();
		}
		else {
			item = new Food(tomatoDropRate);
			item->setIcon(iconType);
		}
		
		// Sets the item position
		item->setPos(pos);

		// Creates items object is added to the gItems vector 
		gItems.push_back(item);
	}

	// Close the input file
	iStream.close();
}

