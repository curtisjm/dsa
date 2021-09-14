/*

  Finish Implementation of the mergeBaseInPlace() function outlines below.

  Remember to submit your data results via a copy of the google spreadsheet link provided below

  https://docs.google.com/spreadsheets/d/1PM1zjXTSZZ0pATZyeq1BJ99toRM0nAVgxYInWHjS7ks

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
// increment copy count manually if you are not doing any swaps
long copyCount = 0;
long compareCount = 0;
long shortcutCount = 0;

int compare(int a, int b) {
    compareCount++;
    if (a > b) return 1;
    if (a < b) return -1;
    // they must be equal
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
            // not descending
            return false;
    }
    return true;
}

// swap i and j and increment counter
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

    // compareCount = 0;
    // swapCount = 0;
}

//
// TODO - implement an in-place version of the base merge function, which will be called from mergeSort()
// You may NOT utilize any auxilliary array storage declared inside or outside of your function
// You MAY use temporary variables as needed.
// You MUST utilize the compare() and myswap() functions for credit.
//

// Implementation Notes for mergeBaseInPlace() below:
// 
// arr[] is an array of integers that are alraedy ordered in left and right halves between start and end
//    the left half is sorted between "start" and "mid"
//    the right half is sorted between "mid+1" and "end"
//   ....  ONLY merge in the range from "start" to "end" (inclusive)
//   remember that start and end are not the first and last positions of the array - they may be a "middle" span
//      so:  arr[] to the left of start may extend for quite a few array locations (start is not zero)
//      and: arr[] to the right of end may extend for quite a few array locations (end is not the last position of the array)

// arr[] looks like this:
//  _ _ ______________________________________________________
//      |   |   |   |   |   |   |   |   |   |   |   |   |   |
//  - - ------------------------------------------------------
//        ^           ^   ^               ^
//       start       mid  mid+1          end
//
//  assume arr[] is already sorted descending between start and mid
//  assume arr[] is already sorted descending between mid+1 and end
//
// You must "merge" these left and right halves so the entire range start->end is sorted

void singleBubblePass(int arr[], int start, int end) {
    for (int i = start; i < end - start + 1; i++) {
        if (compare(arr[i], arr[i + 1]) == 1) {
            mySwap(arr, i, i + 1);
        } else {
            return;
        }
    }
}

void mergeBaseInPlace(int arr[], int start, int mid, int end) {

    // uncomment below if/when you need help debugging your function
    //cout << "\n*mergeBaseInPlace() = " << start << " to " << end << ": ";
    //printSubArr(arr, start, end);

    // uncomment below if/when you need help debugging your function
    //cout << "\n*mergeBaseInPlace() done:";
    //printSubArr(arr, start, end);

    // TODO: Your code goes here
    // int x = 0;
    // int i = start;
    // int j = mid + 1;
    // while (i <= mid && j <= end) {
    //     if (compare(i, j) == 1) mySwap(arr, i, j);
    //     j++;
    //     i++;
    // }

    for (int i = start; i < mid + 1; i++) {
        // find min value of right array and swap it with the current element of the left array if it is smaller than that element
        if (compare(arr[i], mid + 1) == 1) {
            mySwap(arr, i, mid + 1);
            singleBubblePass(arr, mid + 1, end);
        }
    }
    // one pass of bubble sort




    // while (i <= mid) {

    // }
    // while (j <= end) {

    // }

    // do not delete - this will help us keep track of how many times we call the mergeBaseInPlace() function
    baseCount++;
    // verify that the merge worked... you can comment this out if you want...
    // but if it is failing, it indicates your final solution will be incorrect.
    if (!isSortedDescending(arr, start, end)) {
        cout << "mergeBaseInPlace() ABORT: merged base is not descending for range " << start << " to " << end << endl;
        exit(0);
    }
}

void mergeSort(int arr[], int start, int end) {
    cout << "------ IN MERGE SORT ------" << endl;
    if (start >= end) return;
    // find midpoint
    int mid = (start + mid) / 2;
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

        // int a[N];
        int* a = new int(N);
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
    // end power loop to grow N by doubling: 2, 4, 8, 16

    // close a data file to store out output data
    myFile.close();
}