#include <iostream>
#include <cmath>
#include <ctime>
using namespace std;

int main() {

	// this is our upper limit - change it to any value you like (powers of 2 may work best)
	int N = 1024;

	cout << "Picking a secret value between 1 and " << N << endl;

	// set the random number generator to a random starting point based on the current time
	srand(time(NULL));
	// random value from 1 to 1024, inclusive (we add 1 to shift from 0..N-1 to 1..N)
	int secretValue = rand() % N + 1;
	cout << "Shhh - my secret value is " << secretValue << endl;

	// have not guessed it yet
	bool guessedIt = false;

	// this is our initial lower limit
	int low = 1;
	// this is our initial upper
	int high = N;

	// we have not made a guess yet
	int newGuess;
	// this counts our guesses
	int guessCount = 0;

	while (true) {

		// TODO: Your code goes here

	}

	cout << "I guessed it in " << guessCount << " guesses. \nThe secret value was " << secretValue << endl;
	cout << "The log2(" << N << ") = " << log2(N) << endl;

}