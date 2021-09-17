/*

    My copy of the Google Sheet:

    https://docs.google.com/spreadsheets/d/1K2Zn600T6FUjIZSJgzK_PKmAVIfxoC35ZxiKmzFHL7E/edit?usp=sharing

*/

#include <iostream>
#include <vector>
#include <cmath>
#include <time.h>
#include <cassert>
#include <fstream>

using namespace std;

long baseCount = 0;
long swapCount = 0;
long copyCount = 0;
long compareCount = 0;
long shortcutCount = 0;

int compare(int a, int b) {
    compareCount++;
    if (a > b) return 1;
    if (a < b) return -1;
    return 0;
}

void printSubArr(int arr[], int start, int end) {
    for (int k = start; k <= end; k++) {
        cout << arr[k] << ", ";
    }
    cout << endl;
}

bool isSortedDescending(int arr[], int start, int end) {
    for (int i = start; i < end; i++) {
        if (arr[i] < arr[i + 1])
            return false;
    }
    return true;
}

void mySwap(int a[], int i, int j) {
    swapCount++;
    int temp = a[i];
    a[i] = a[j];
    a[j] = temp;
}

void printStats(long N) {
    cout << "\nPrint Stats" << endl;
    cout << "--------------------------------------" << endl;
    cout << "\nResults:" << endl;
    cout << "Number of elements (N) = " << N << endl;
    cout << "Log(N) = " << log2(N) << endl;
    cout << "NLog(N) = " << N * log2(N) << endl;
    cout << "N^2 = " << N * N << endl;
    cout << "(N-1)^2 = " << (N - 1) * (N - 1) << endl;
    cout << "N(N-1)/2 = " << (N) * (N - 1) / 2 << endl;
    cout << "N(N-1)/4 = " << (N) * (N - 1) / 4 << endl;
    cout << "Number of compares = " << compareCount << endl;
    // may be zero if you are just counting copies via copyCount below
    cout << "Number of swaps = " << swapCount << endl;
    // may be zero if you are just counting swapping via mySwap() above
    cout << "Number of copies = " << copyCount << endl;
    cout << "Number of calls to base merge = " << baseCount << endl;
}

// a modified version of shell sort
void mergeBaseInPlace(int arr[], int start, int mid, int end) {
    int g = (int)ceil((end - start + 1) / 2.0);

    // go until gap is 0 (have to force 0 bc/ will never reach it on its own), gap gets / 2 each iteration
    for (g; g > 0; g = (g <= 1) ? 0 : (int)ceil(g / 2.0)) {
        // stop when right number gets out of range 
        for (int i = start; (i + g) <= end; i++) {
            int j = i + g;
            // swap if num on left side of gap is larger
            if (compare(arr[i], arr[j]) == -1) {
                mySwap(arr, i, j);
            }
        }
    }

    // keep track of how many times we call the mergeBaseInPlace() function
    baseCount++;
    // verify that the merge worked... you can comment this out if you want...
    // but if it is failing, it indicates your final solution will be incorrect.
    if (!isSortedDescending(arr, start, end)) {
        cout << "mergeBaseInPlace() ABORT: merged base is not descending for range " << start << " to " << end << endl;
        exit(0);
    }
}

void mergeSort(int arr[], int start, int end) {
    if (start >= end) return;
    // find midpoint
    int mid = (start + end) / 2;
    // sort left side
    mergeSort(arr, start, mid);
    // sort right side
    mergeSort(arr, mid + 1, end);
    // combine left and right
    mergeBaseInPlace(arr, start, mid, end);
}

int main() {
    std::cout << "Merge Sort - HW2 - In-place base merge (STARTER Fall 21)\n";

    // open a data file to store out output data
    ofstream myFile("data.csv");
    myFile << "N (X Axis), Count (Y Axis), Category (Lines)" << endl;
    // new seed each time
    srand(time(NULL));

    bool debug = true;

    // power loop to grow N by doubling: 2, 4, 8, 16
    for (int pwr = 1; pwr < 10; pwr++) {
        // size of array, 2 raised to a power from our outer loop
        int N = pow(2, pwr);

        int a[N];

        // build a random list with duplicates
        for (int i = 0; i < N; i++) {
            a[i] = rand() % N + 1;
        }

        if (debug && N < 200) {
            cout << "\nOur original list of numbers (unsorted):" << endl;
            // print from zero to N-1 of array a
            printSubArr(a, 0, N - 1);
        }

        // mergeSort from zero to N-1 of array a
        mergeSort(a, 0, N - 1);

        if (debug && N < 200) {
            cout << "\nOur Merge Sorted numbers:" << endl;
            // print from zero to N-1 of array a
            printSubArr(a, 0, N - 1);
        }

        if (!isSortedDescending(a, 0, N - 1)) {
            cout << "\nmain(): ABORT: MergeSort() did not produce a descending sort." << endl;
            exit(0);
        }

        printStats(N);

        myFile << N << ", " << std::fixed << compareCount << ", compares " << endl;
        myFile << N << ", " << std::fixed << baseCount << ", baseCount " << endl;
        myFile << N << ", " << std::fixed << swapCount << ", swaps " << endl;
        myFile << N << ", " << std::fixed << copyCount << ", copies, " << endl;
        myFile << N << ", " << std::fixed << log2(N) << ", logarithmic " << endl;
        myFile << N << ", " << std::fixed << N * log2(N) << ", ________________linearithmic " << endl;
        //myFile << N << ", " << std::fixed << N*N                         <<  ", quadratic "               << endl;
        //myFile << N << ", " << sampleCount  << ", cubic, "           << std::fixed << long(N*N*N)                 << endl;
    }

    // close a data file to store out output data
    myFile.close();
}