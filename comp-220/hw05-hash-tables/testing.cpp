#include <iostream>
#include <chrono>
#include <string>
#include <vector>

using namespace std;

int hashFunction(string s, int m) {  // simple summation of ascii values mod m
    // TODO: convert s into a number in the range of 0 to m-1 (so we can use that number as a location in an array of size "m" )
    int sum = 0;

    for (char c : s) {
        sum += c;
    }

    return sum % m; // TODO: you'll need to return a value here in the range 0 to m-1
}

int main() {
    // // string str = "hello test";

    // // cout << hashFunction(str, 10);
    // int currentSize = 10;
    // int maxBuckets = 55;

    // float lf = currentSize == 0 ? 0.0 : (float)currentSize / (float)maxBuckets;

    // cout << lf;
    cout << hashFunction("asdf", 10);


    return 0;
}