// Call Centre - SDFD202 - Alex Mounsey (2018)
// A program that uses stacks to imitate the holding of calls at a Call Centre

#include <string>
#include <iostream>
#include <conio.h>
#include <vector>
#include <time.h>
#include <Windows.h>

#define MAX_CALLERS 20

using namespace std;

void	push(string n);
void	pop(void);
void	formatS(int s);
void	mainMenu(void);
void	drawMenu(void);
string	generateCaller(void);
void	setLoc(int x, int y);
void clearConsole(void);

time_t	timer; // Timer
int		pointer = -1; // Stack pointer
int		idCount = 0; // Tracks all-time number of callers
int		elapsedT, elapsedH, elapsedM, elapsedS; // Used for converting seconds to HH:MM:SS format

vector<int>		idV; // Stores caller ids
vector<string>	numberV; // Stores phone numbers
vector<int>		timeV; // Stores the time each caller connects

void main(void) {
	mainMenu();
}

// Push a new caller to the stack
void push(string n) {
	if (idV.size() != MAX_CALLERS - 1) {
		pointer++;

		idV.push_back(idCount);
		numberV.push_back(n);
		timeV.push_back(time(NULL));

		idCount++;
	} else { /* Stack is full */ }
}

// Pop the oldest caller from the stack (simulating pushing call to an operator)
void pop(void) {
	if (pointer != -1) {
		idV.erase(idV.begin());
		numberV.erase(numberV.begin());
		timeV.erase(timeV.begin());
	} else { /* Stack is empty */ }
}

// Format seconds to HH:MM:SS
void formatS(int s) {
	// Hours
	elapsedH = s / 3600;
	s = s % 3600;

	// Minutes
	elapsedM = s / 60;
	s = s % 60;

	// Seconds
	elapsedS = s;
}

// Run the menu
void mainMenu(void) {
	drawMenu();

	switch (_getch()) {
	case 49: // '1' is pressed - add a new call
		push("07296 185236");
		mainMenu();
		break;

	case 50: // '2' is pressed - push call to operator
		pop();

		mainMenu();
		break;

	case 51: // '3' is pressed - disconnect call (randomly disconnect a caller)
		if (pointer != -1) {
			int rng = rand() % idV.size() + 1;

			idV.erase(idV.begin() + rng);
			numberV.erase(numberV.begin() + rng);
			timeV.erase(timeV.begin() + rng);
		} else { /* Stack is empty */ }

		mainMenu();
		break;

	case 52: // '4' is pressed - quit application
		break;

	default:
		mainMenu();
		break;
	}
}

// Draw the menu graphics (c1/c2/c3 are console colours)
void drawMenu(void) {
	clearConsole();

	// Print Queue
	for (int i = 0; i < idV.size(); ++i) {
		formatS(time(NULL) - timeV.at(i));

		// Print
		cout << "ID: " << idV.at(i) << " | ";
		cout << "Number: " << numberV.at(i) << " | ";
		cout << "Time: " << elapsedH << ":" << elapsedM << ":" << elapsedS << endl;
	}
}

// Generate a phone number
string generateCaller(void) {
	return "unfinished";
}

// Set console cursor location
void setLoc(int x, int y) {
	static HANDLE hStdout = NULL;
	COORD coord;

	coord.X = x;
	coord.Y = y;

	if (!hStdout) { hStdout = GetStdHandle(STD_OUTPUT_HANDLE); }
	SetConsoleCursorPosition(hStdout, coord);
}

// Clears the screen
void clearConsole(void) {
	static HANDLE hStdout = NULL;
	static CONSOLE_SCREEN_BUFFER_INFO csbi;
	const COORD startLoc = { 0,0 };
	DWORD dummy;

	if (!hStdout) {
		hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
		GetConsoleScreenBufferInfo(hStdout, &csbi);
	}

	FillConsoleOutputCharacter(hStdout, ' ', csbi.dwSize.X * csbi.dwSize.Y, startLoc, &dummy);
	setLoc(0, 0);
}