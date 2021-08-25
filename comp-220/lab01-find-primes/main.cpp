
// --- Explanation of Sieve of Eratosthenes ---

// Assume all numbers are unmarked.
// Start at the beginning.
// If we find a number that's unmarked, we know it's prime.
//  - ex: we hit two and know it's unmarked
// eliminate all multiples of that number because we know they're composite
//  - start from square of that number
//      - ex: when you hit three, start from nine
// restart, keep mobing along until you hit unmarked number
// stop when you hit a number thats square is larger than the limit

#include <iostream>
#include <cmath>

using namespace std;

int main() {
    int limit = 10;

    cout << "What is your upper limit (inclusive, >= 2)? ";
    cin >> limit;

    // minimum upper limit of 2
    if (limit < 2) limit = 2;

    bool* isPrime = new bool[limit + 1];

    // assume number is prime until proven otherwise
    for (int i = 0; i <= limit; i++) {
        isPrime[i] = true;
    }

    // start at 2, go until square of number is larger than the limit
    for (int i = 2; i * i <= limit; i++) {
        // if we hit an "unmarked number," it must be prime
        if (isPrime[i] == true) {
            // start at square of last prime, find all composites
            for (int j = i * i; j <= limit; j += i) {
                // composites are not prime
                isPrime[j] = false;
            }
        }
    }

    bool* ptr = isPrime;
    int totalPrimes = 0;
    cout << "Primes <= " << limit << " are: " << endl;

    for (int i = 2; i <= limit; i++) {
        if (isPrime[i] == true) {
            totalPrimes++;
            cout << i << ", ";
        }
        ptr++;
    }
    cout << endl;

    cout << "Total Primes = " << totalPrimes << endl;

    delete[] isPrime;
    delete ptr;

}
