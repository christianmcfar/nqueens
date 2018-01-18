
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <ctime>

using namespace std;

int N;

int* queen;
int* rowConflicts;
int* upperDiagonalConflicts;
int* lowerDiagonalConflicts;
int* state;
vector<int> lowestIndexList;
vector<int> highestConflictCol;


void printBoard(char* filename) {

	if (N <= 1000) {
		ofstream myfile;
		myfile.open(filename);
		for (int y = 0; y < N; y++) {			//prints right then down
			for (int x = 0; x < N; x++) {
				int c = rowConflicts[y] + upperDiagonalConflicts[x+y] + lowerDiagonalConflicts[N-x+y-1];
				if (y == queen[x]) {
					if (c >= 3){
						c -= 3;
					}
					myfile << c << "Q ";
				} else {
					myfile << c << "  ";
				}
			}
			myfile << "\n";
		}
		myfile << "\n";
		myfile.close();
	}
}

void init() {

	// initialize all conflicts to 0
	for (int i = 0; i < (2*N) - 1; i++) {
		if (i < N) {
			rowConflicts[i] = 0;
		}
		upperDiagonalConflicts[i] = 0;
		lowerDiagonalConflicts[i] = 0;
	}

	// set first queen randomly on column 0 (x = 0)
	queen[0] = rand() % N;
	rowConflicts[queen[0]] += 1;						//current row conflicts = y
	upperDiagonalConflicts[0+queen[0]] += 1; 			//upperDiagonal position = x+y
	lowerDiagonalConflicts[N-0+queen[0]-1] += 1;		//lowerDiagonal position = N-x+y-1

	int lowest, next;

	// randomly assign queens for all other columns
	for (int x = 1; x < N; x++) {
		lowestIndexList.clear();
		lowest = rowConflicts[0] + upperDiagonalConflicts[x+0] + lowerDiagonalConflicts[N-x+0-1];

		// loop through columns adding lowest conflicted squares the list
		for (int y = 0; y < N; y++) {
			next = rowConflicts[y] + upperDiagonalConflicts[x+y] + lowerDiagonalConflicts[N-x+y-1];

			// if the current queen's conflicts is a new low, reset the list
			if (next < lowest) {
				lowest = next;
				lowestIndexList.clear();
				lowestIndexList.push_back(y);
			}
			else if (next == lowest) {
				lowestIndexList.push_back(y);
			}
		}

		// randomly select a queen from the list of the current lowest conflicted squares
		queen[x] = lowestIndexList.at(rand() % lowestIndexList.size());

		// record the conflicts for the newly placed queen
		rowConflicts[queen[x]] += 1;
		upperDiagonalConflicts[x+queen[x]] += 1;
		lowerDiagonalConflicts[N-x+queen[x]-1] += 1;
	}
}


int highestConflictColumn() {

	int highest, next;
	highestConflictCol.clear();

	// set the highest conflicted queen to be the square first column queen
	highest = rowConflicts[queen[0]] + upperDiagonalConflicts[0+queen[0]] + lowerDiagonalConflicts[N-0+queen[0]-1];

	// loop through columns adding highest conflicted queens the list
	for (int x = 0; x < N; x++) {
		next = rowConflicts[queen[x]] + upperDiagonalConflicts[x+queen[x]] + lowerDiagonalConflicts[N-x+queen[x]-1];

		// if the current queen's conflicts is a new high, reset the list
		if (next > highest) {
			highest = next;
			highestConflictCol.clear();
			highestConflictCol.push_back(x);
		}
		else if (next == highest) {
			highestConflictCol.push_back(x);
		}
	}

	// return queen randomly selected from the list of highest
	return highestConflictCol.at(rand() % highestConflictCol.size());
}


void minConflict(int x) {

	int lowest, next;
	lowestIndexList.clear();

	// set the lowest conflicted row to be the square with the highest conflict
	lowest = rowConflicts[queen[x]] + upperDiagonalConflicts[x+queen[x]] + lowerDiagonalConflicts[N-x+queen[x]-1];

	// loop down rows adding lowest conflicted rows the list
	for (int y = 0; y < N; y++) {
		next = rowConflicts[y] + upperDiagonalConflicts[x+y] + lowerDiagonalConflicts[N-x+y-1];

		if (next < lowest) {
			lowest = next;
			lowestIndexList.clear();
			lowestIndexList.push_back(y);
		}
		else if (next == lowest) {
			lowestIndexList.push_back(y);
		}
	}

	// decrement the conflicts at queens old position
	rowConflicts[queen[x]] -= 1;
	upperDiagonalConflicts[x+queen[x]] -= 1;
	lowerDiagonalConflicts[N-x+queen[x]-1] -= 1;

	// randomly select a new position for queen from the list of the current lowest conflicted rows
	queen[x] = lowestIndexList.at(rand() % lowestIndexList.size());

	// increment conflicts at queens new position
	rowConflicts[queen[x]] += 1;
	upperDiagonalConflicts[x+queen[x]] += 1;
	lowerDiagonalConflicts[N-x+queen[x]-1] += 1;

}

bool checkState() {

	bool solved = true;
	int x = 0;

	// look through queens positions until a conflicted queen is found
	while(solved && x < N) {
		state[x] = rowConflicts[queen[x]] + upperDiagonalConflicts[x+queen[x]] + lowerDiagonalConflicts[N-x+queen[x]-1];

		if (state[x]-3 != 0) {
			solved = false;
		}
		x++;
	}
	return solved;
}


int main() {

	srand(time(NULL));

	//check if N value is valid (N > 3)
	bool validInput = false;
	while (!validInput) {
		cout << "Enter number of queens to test: ";
		cin >> N;
		if (N >= 4) {
			validInput = true;
		} else {
			cout << "Input must be 4 or greater, please try again" << endl;
		}
		cout << endl;
	}


	// initialize the arrays for queen positions and conflict storage
	queen = new int[N];
	rowConflicts = new int[N];
	upperDiagonalConflicts = new int[(2*N-1)];
	lowerDiagonalConflicts = new int[(2*N-1)];
	state = new int[N];

	bool done = false;
	int steps = 0;

	bool initselect = false;
	int preInitBoard = 0;

	// choose initialization option
	while(!initselect) {
		cout << "Press 1 to enter own board or 2 to auto initialize: ";
		cin >> preInitBoard;
		if (cin.good() && (preInitBoard == 1 || preInitBoard == 2)) {
			initselect = true;
		} else {
			cout << "Please enter one of the options." << endl;
			cin.clear();
			cin.ignore(10000, '\n');
		}
	}

	clock_t begin = clock();

	// initialize a pre determined board
	if (preInitBoard == 1) {
		for (int i = 0; i < (2*N) - 1; i++) {
			if (i < N) {
				rowConflicts[i] = 0;
			}
			upperDiagonalConflicts[i] = 0;
			lowerDiagonalConflicts[i] = 0;
		}
		for (int x = 0; x < N; x++) {
			cout << "Queen at column " << x << ": ";
			cin >> queen[x];
			rowConflicts[queen[x]] += 1;
			upperDiagonalConflicts[x+queen[x]] += 1;
			lowerDiagonalConflicts[N-x+queen[x]-1] += 1;
		}
	}
	if (preInitBoard == 2) {
		// auto initialize the board
		init();
	}

	printBoard("initBoard.txt");

	clock_t init = clock();
	double i_elapsed_secs = double(init - begin) / CLOCKS_PER_SEC;

	done = checkState();

	// continue using minConflicts until solution is found (can be change to step count)
	while (!done) {

		// find the min conflicts of a column, which was randomly select of the highest conflicted columns
		minConflict(highestConflictColumn());
		steps++;

		// if solution is found do not repeat minConflicts
		if(checkState()) {
			done = true;
			cout << "Solution found";
		}

	}

	clock_t end = clock();
	printBoard("outputBoard.txt");
	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;

	ofstream myfile;
	myfile.open("resultTimes.txt");
	myfile << "Steps: " << steps << endl
		 << "Initialization Time: " << i_elapsed_secs << endl
		 << "Total Time: " << elapsed_secs << endl;
	myfile.close();

	return 0;
}
