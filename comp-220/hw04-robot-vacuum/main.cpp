#include <iostream>

using namespace std;

const int numcols = 20;  // number of columns in our grid (width)
const int numrows = 20;  // number of rows in our grid (height)
char grid[numrows][numcols]; // declare our grid
char original[numrows][numcols]; // declare our a duplicate of our grid for reference

int currentRow = 0;      // starting row location of robot
int currentCol = 0;      // starting col location of robot
int totalMoves = 0;      // total count of moves made by the robot

//
// INIT THE GRID (written for you: please do not change this function)
//
void initGrid() {
    for (int r = 0; r < numrows; r++) {
        for (int c = 0; c < numcols; c++) {
            if (rand() % 100 < 25) { // 25% change of creating random obstacle
                grid[r][c] = '*'; // this is an obstacle
            } else { // else we have garbage
                grid[r][c] = '.';   // this is garbage
            }
            original[r][c] = grid[r][c]; // put in our original array for final scoring
        }
    }
}

//
// PRINT THE GRID (written for you: please do not change this function)
//

void printGrid() {

    cout << "-";
    for (int c = 0; c < numcols; c++) // print top border
        cout << "---";
    cout << "-" << endl;

    for (int i = 0; i < numrows; i++) { // print all rows
        cout << "|";
        for (int j = 0; j < numcols; j++) { // print a row
            if (i == currentRow && j == currentCol)
                cout << " " << 'V' << " "; // print a V for the vaccum
            else
                cout << " " << grid[i][j] << " ";  // print what is at this location
        }
        cout << "|" << endl;
    }
    cout << "-";
    for (int c = 0; c < numcols; c++) // print bottom border
        cout << "---";
    cout << "-" << endl;
}

//
// GET ROW (written for you: please do not change this function)
//

int getRow() {   // return the current row location of the robot
    return currentRow;
}

int getCol() {
    return currentCol;  // return the current row location of the robot
}

//
// MOVE TO (written for you: please do not change this function)
//

bool moveTo(int r, int c) { // move the robot to this location and sweep up what is there
    if (r < 0 || r >= numrows) {
        cout << "moveT0() ignoring requested move to " << r << ", " << c << " because it is out of room" << endl;
        return false; // cannot move out of the room 
    }

    if (c < 0 || c >= numcols) {
        cout << "moveT0() ignoring requested move to " << r << ", " << c << " because it is out of room" << endl;
        return false; // cannot mvoe out of the room
    }

    if (abs(r - currentRow) > 1 || (abs(c - currentCol) > 1)) {
        cout << "moveT0() ignoring requested move to " << r << ", " << c << " because it is > 1 in either direction" << endl;
        return false; // cannot move by more than 1 in each direction
    }

    currentRow = r;
    currentCol = c;
    grid[r][c] = ' '; // clean what was there even if it it was an obstacle.
    totalMoves++; // increment the number of total moves taken so far
    return true;
}





//
// PRINT SCORE
//

void printScore() { // written for you: (please do not change this function)
    int obstaclesMissing = 0;
    int obstaclesTotal = 0;
    int garbageTotal = 0;
    int garbageMissed = 0;


    for (int r = 0; r < numrows; r++) {
        for (int c = 0; c < numcols; c++) {
            if (grid[r][c] == '.') garbageMissed++; // found leftover garbage, 
            if (original[r][c] == '*') obstaclesTotal++; // total original obstacles
            if (original[r][c] == '.') garbageTotal++;   // total original garbage
            if (original[r][c] == '*' && grid[r][c] != '*') obstaclesMissing++; // someone removed an obstacle - that's bad
        }
    }

    int garbageSweptUp = (garbageTotal - garbageMissed);

    int score = numrows * numcols - garbageMissed - obstaclesMissing; // reduce score from total possible by garbage that was not picked up and obstacles that were removed
    score = (100 * score) / (numrows * numcols); // normalized based on total possible squares in grid

    cout << "SCORE:" << endl;
    cout << "----------------------" << endl;
    cout << "Total squares in grid = " << numrows * numcols << endl;
    cout << "Garbage total (original): " << garbageTotal << endl;
    cout << "Obstacles total (original): " << obstaclesTotal << endl;
    cout << "Garbage swept up: " << garbageSweptUp << endl;
    cout << "Garbage Missed = " << garbageMissed << " (minus " << garbageMissed << " points)" << endl;
    cout << "Obstacles Missing = " << obstaclesMissing << " out of " << obstaclesTotal << " (minus " << obstaclesMissing << " points)" << endl;
    cout << "Total Moves taken = " << totalMoves << endl;
    if (totalMoves > 0)
        cout << "Cleaning efficiency = garbage swept up/totalMoves = %" << ((100 * garbageSweptUp) / totalMoves) << endl;
    cout << "Final normalized score out of 100 possible = " << score << endl;

    if (score > 90) cout << "Well done!" << endl;
    else if (score > 80) cout << "Not bad - your robot is a bit sloppy though" << endl;
    else if (score > 70) cout << "So So - your robot might need a tune up" << endl;
    else if (score > 60) cout << "Not so great - your robot seems to be having a bad day" << endl;
    else  cout << "Hmmm - might want to call in some iRobot back-up for help.  Let me know if you have questions!" << endl;
}


//
// SWEEP GRID TODO - you need to write this function so that it cleans grid[][] without sweeping up obstacles
//

void sweepGrid() {
    int totalMoves = 0;

    //  NOTE - you must use getRow() and getCol() to find the current location of the robot
    int myRow = getRow(); // get current row location of the robot
    int myCol = getCol(); // get current row location of the robot

   // NOTE: you must use "moveTo()" below to move your robot - don't sweep up obstacles though!
   // NOTE: obstacles WILL be swept up (bad) so you want to avoid them at all costs (or lose points)
    bool didMove = moveTo(myRow + 1, myCol + 1);    // attempt to move to this location and sweep up whatever is there

    // NOTE: you can test whether a requested move worked or not
    if (!didMove) cout << "oops, that move did not work - maybe I hit the walls of the room???" << endl;


    // NOTE - you can use the current status of the grid[][] or the original original[][] in any way you like to aid your algorithm

    // THESE ARE JUST EXAMPLES
    if (original[myRow][myCol] == '*') cout << "oops I just swept up an obstacle!" << endl;

    // NOTE: when you check a location in the grid, be sure you do not go out of bounds!
    if ((myRow + 1 < numrows) && grid[myRow + 1][myCol] == '*') cout << "there is an obstacle in the row below me" << endl;

    // done sweeping
}


int main() {
    std::cout << "HW5 ALT - Robot Vacuum (starter) \n";

    initGrid();   // init the grid with garbage and obstacles
    cout << "Dirty Room before sweeping: " << endl;
    printGrid();

    sweepGrid();  // TODO Your routine!

    cout << "Clean Room after sweeping: " << endl;
    printGrid();

    printScore(); // your score

}