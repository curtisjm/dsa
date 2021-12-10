#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <ctime>

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

struct Point {
    // x is column, y is row
    int x;
    int y;
};

struct Node {
    Point pt;
    double distance;
};

// a heuristic function that is normally used to controll A*'s behavior
// in this case, I will be using it to find the nearest garbage by finding the distance to all garbage nodes first
// reference: http://theory.stanford.edu/~amitp/GameProgramming/Heuristics.html
double heuristic(int fromX, int fromY, int toX, int toY) {
    // D is the minimum cost for moving from one space to an adjacent space
    double D = 1;
    // D2 is the cost of moving diagonally
    double D2 = 1;
    // when D, D2 = 1, this is the Chebyshev distance: 
    // https://en.wikipedia.org/wiki/Chebyshev_distance

    double dx = abs(fromX - toX);
    double dy = abs(fromY - toY);

    return D * (dx + dy) + (D2 - 2 * D) * min(dx, dy);
}

// find all garbage nodes
vector<Node> getAllDistances(int x, int y) {
    vector<Node> distances;

    // iterate over all rows and columns
    for (int i = 0; i < numrows; i++) {
        for (int j = 0; j < numcols; j++) {
            // don't include current node in distances list
            // bool sameNode = (i == y && j == x);
            if (grid[i][j] == '.') {
                // add data about each node to the distance object
                Point p;
                p.x = j;
                p.y = i;
                Node n;
                n.distance = heuristic(x, y, j, i);
                // append to the list of distances
                distances.push_back(n);
            }
        }
    }

    // sort the distances vector by distance
    sort(distances.begin(), distances.end(), [](Node a, Node b) {
        return a.distance < b.distance;
        });

    return distances;
}

// check if there is any remaining garbage
bool containsGarbage() {
    for (int i = 0; i < numrows; i++) {
        for (int j = 0; j < numcols; j++) {
            if (grid[i][j] == '.') return true;
        }
    }
    return false;
}

// check if a move is valid
bool isValid(int row, int col) {
    return (row >= 0) && (row < numrows) && (col >= 0) && (col < numcols) && (grid[row][col] != '*');
}

// if the distance to the nearest garbage is greater than 1, find a series of single moves to get there
vector<Point> BFS(Point start, Point end) {
    // movement "matrix"
    int rowNum[] = { -1, 0, 0, 1, -1, 1, -1, 1 };
    int colNum[] = { 0, -1, 1, 0, 1, 1, -1, -1 };

    bool visited[numrows][numcols];
    // memset(visited, false, sizeof visited);

    // mark start as visited
    visited[start.y][start.x] = true;

    queue<Node> q;

    // distance to start is 0
    Node n = { start, 0 };
    q.push(n);

    while (!q.empty()) {
        Node curr = q.front();
        Point pt = curr.pt;

        // found the end
        if (pt.x == end.x && pt.y == end.y) {
            vector<Point> path;
            path.push_back(curr.pt);
            while (curr.pt.x != start.x || curr.pt.y != start.y) {
                curr = q.front();
                q.pop();
                path.push_back(curr.pt);
            }
            return path;
        }

        // dequeue front
        q.pop();

        // enqueue adjacent nodes
        for (int i = 0; i < 8; i++) {
            int row = pt.y + rowNum[i];
            int col = pt.x + colNum[i];

            // if adjacent cell is valid, has path and
            // not visited yet, enqueue it.
            if (isValid(row, col) && grid[row][col] && !visited[row][col]) {
                // mark cell as visited and enqueue it
                visited[row][col] = true;
                Node adjNode = { {row, col}, curr.distance + 1 };
                q.push(adjNode);
            }
        }
    }

    // if we get here, we have not found a path
    return vector<Point>();
}

//
// SWEEP GRID TODO - you need to write this function so that it cleans grid[][] without sweeping up obstacles
//

void sweepGrid2() {
    int totalMoves = 0;
    bool continueCleaning = containsGarbage(); // check if there is any garbage left

    // TODO: make sure we can go backwards to get out of a corner

    while (continueCleaning) {
        // NOTE - you must use getRow() and getCol() to find the current location of the robot
        int myRow = getRow(); // get current row location of the robot
        int myCol = getCol(); // get current row location of the robot

        vector<Node> distances = getAllDistances(myCol, myRow); // vector of distances to all garbage nodes

        // find the closest garbage node
        Node closest = distances[0];

        // move to the closest garbage node if it is is not an obstacle
        if (moveTo(closest.pt.x, closest.pt.y)) {
            totalMoves++;
        } else {
            vector<Point> path = BFS({ myCol, myRow }, closest.pt);
            for (Point p : path) {
                if (moveTo(p.x, p.y)) {
                    totalMoves++;
                }
            }
        }

        continueCleaning = containsGarbage(); // check if there is any garbage left
    }
}

void sweepGrid() {
    int totalMoves = 0;

    // movement "matrix"
    int rowNum[] = { -1, 0, 0, 1, -1, 1, -1, 1 };
    int colNum[] = { 0, -1, 1, 0, 1, 1, -1, -1 };

    bool continueCleaning = containsGarbage(); // check if there is any garbage left

    int myRow = getRow(); // get current row location of the robot
    int myCol = getCol(); // get current row location of the robot

    while (continueCleaning) {
        bool didMove = false;
        myRow = getRow();
        myCol = getCol();

        while (!didMove) {
            // select a random direction to move in
            int randDir = rand() % 8;
            int row = myRow + rowNum[randDir];
            int col = myCol + colNum[randDir];
            if (isValid(row, col)) {
                if (moveTo(row, col)) {
                    totalMoves++;
                    didMove = true;
                    cout << "Moved to " << row << " " << col << endl;
                }
            }
        }
        continueCleaning = containsGarbage();
    }
}


int main() {
    srand(time(NULL));
    std::cout << "HW5 ALT - Robot Vacuum (starter) \n";

    initGrid();   // init the grid with garbage and obstacles
    cout << "Dirty Room before sweeping: " << endl;
    printGrid();

    sweepGrid();  // TODO Your routine!

    cout << "Clean Room after sweeping: " << endl;
    printGrid();

    printScore(); // your score
}