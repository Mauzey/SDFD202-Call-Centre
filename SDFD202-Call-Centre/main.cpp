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
void	setLoc(int x, int y);
void	clearConsole(void);
void	textColour(int colour);

time_t	timer; // Timer
int		pointer = -1; // Stack pointer
int		idCount = 0; // Tracks all-time number of callers
int		elapsedT, elapsedH, elapsedM, elapsedS; // Used for converting seconds to HH:MM:SS format
string	notification; // Used to display important information in the bottom right of the interface
int		averageWait; // Used to track average wait time

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
	} else { notification = "Error: Cannot add call (Queue is full)."; }
}

// Pop the oldest caller from the stack (simulating pushing call to an operator)
void pop(void) {
	if (idV.size() != 0) {
		notification = "Removed call ID: " + to_string(idV.at(0)) + " from the queue (pushed to operator).";

		idV.erase(idV.begin());
		numberV.erase(numberV.begin());
		timeV.erase(timeV.begin());
	} else { notification = "Error: Cannot remove call (Queue is empty)."; }
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
		notification = "Added call ID: " + to_string(idCount) + " to the queue.";
		push("00000 000000");
		
		mainMenu();
		break;

	case 50: // '2' is pressed - push call to operator
		pop();

		mainMenu();
		break;

	case 51: // '3' is pressed - disconnect call (randomly disconnect a caller)
		if (idV.size() != 0) {
			int rng = rand() % idV.size();

			notification = "Removed call ID: "  + to_string(idV.at(rng)) +  " from the queue (disconnected).";

			idV.erase(idV.begin() + rng);
			numberV.erase(numberV.begin() + rng);
			timeV.erase(timeV.begin() + rng);
		} else { notification = "Error: Cannot remove call (Queue is empty)."; }

		mainMenu();
		break;
	case 52: // '4' is pressed - update queue
		notification = "Updated queue.";

		mainMenu();
		break;

	case 53: // '5' is pressed - quit application
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
		
		textColour(10); cout << "ID: " << idV.at(i);
		textColour(15); cout <<  " | " << "Number: " << numberV.at(i) << " | ";
		textColour(10); cout << "Time: " << elapsedH << ":" << elapsedM << ":" << elapsedS << endl;
	}

	// Print average wait time
	textColour(10); setLoc(65, 15); cout << "Average Waiting Time: ";

	if (idV.size() != 0) {
		averageWait = 0;
		for (int i = 0; i < idV.size(); ++i) averageWait += time(NULL) - timeV.at(i);
		averageWait = averageWait / idV.size();

		formatS(averageWait);
		
		textColour(15); cout << elapsedH << ":" << elapsedM << ":" << elapsedS;
	} else { textColour(15); cout << "N/A - Queue is empty."; }

	//setLoc

	// Print menu options
	textColour(11); setLoc(65, 5); cout << "Call Centre Application";
	setLoc(65, 6); cout << "-----------------------";

	textColour(14); setLoc(65, 8); cout << "[1] Add a new call to the queue";
	textColour(14); setLoc(65, 9); cout << "[2] Push Oldest Call to Operator";
	textColour(14); setLoc(65,10); cout << "[3] Disconnect Random Call";
	textColour(14); setLoc(65,11); cout << "[4] Update Queue";
	textColour(14); setLoc(65,12); cout << "[5] Quit Application";

	// Print notification
	textColour(12);
	setLoc(0, 25); cout << notification;
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

// Used to set the text colour
void textColour(int colour) {
	HANDLE  hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, colour);
}