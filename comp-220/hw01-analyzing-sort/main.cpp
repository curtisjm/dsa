#include <iostream>
#include <cmath>
#include <chrono>
#include <iomanip>

/*

  Submission:  Along with a link to ths repl, remember to copy and fill out this google spreadsheet with your results:

  https://docs.google.com/spreadsheets/d/1gGLdji0SbLiGeoUvvcCdixevRXZ5XWg2hDedQvOxumA/edit?usp=sharing 


*/

using namespace std; // select as standard library for main (CAVEAT: never use this in .h files or large projects!)

static long swapCount = 0; // used to keep track of swaps
static long compareCount = 0; // used to keep track of compares
static chrono::duration<double> time_span; // used to keep track of elapsed time

// return -1, 0, or 1 to indicate if a is < ==, or > b
int compare(int a, int b){
  compareCount++;
  if(a > b) return 1;
  if (a < b) return -1;
  return 0;  // they must be equal
}

// swap i and j and increment counter
void swap(int a[], int i, int j){
  swapCount++;
  int temp = a[i]; 
  a[i] = a[j];
  a[j] = temp;
}

//
// PRINT Values
//

void printValues(const char *msg, int a[], int N){
  cout << msg << endl;
  for(int i=0; i<N; i++)
    cout << a[i] << ", ";
  cout << endl;
}


//
// PRINT STATS
//

  void printStats(int N){
    cout << "\nPrint Stats" << endl;
    cout << "--------------------------------------" << endl;
    cout << "For value N = " << N << endl;
    cout << "Total Swaps = " << swapCount << endl;
    cout << "Total Compares = " << compareCount << endl;
    cout << "Time Duration " << fixed << setprecision(10) << time_span.count() << " seconds." << endl;
    cout << "Log(n) = " << log2(N) << endl;
    cout << "nLog(n) = " << N*log2(N) << endl;
    cout << "N^2 = " << (N)*(N) << endl;

    compareCount = 0;
    swapCount = 0;
  }


//
// mySort() 
//

void mySort(int a[], int N){

  chrono::steady_clock::time_point start_time = chrono::steady_clock::now(); // start time

  // TODO Write your sort function here


  chrono::steady_clock::time_point end_time = chrono::steady_clock::now(); // end time

  time_span = chrono::duration_cast<chrono::duration<double>>(end_time - start_time); // duration
}

//
// isSortedAscending() - called after sorting to verify results
//

bool isSortedAscending(int a[], int N){

  for(int i=0; i< N-1; i++){

    if (a[i]>a[i+1]) // the order of these 2 are inverted, so the array is not ascending
      return false;
  }

  return true; // if we got this afar, the array must be sorted in ascending order
}

int main() {
  std::cout << "HW My Sort\n";

    int N=64; // number of items we are sorting
    int a[N]; // array of N items

    for(int i=0; i<N; i++){
      a[i]  = rand()%N+1; // generate random values in the range of 1 to N, inclusive
    }

    if (N < 100) printValues("Initial Values: ", a, N); // only print if N is small, to avoid clogging up the console

    mySort(a, N); // sort array a containing N values (TODO - you need to write this!)
 
    if (N < 100) printValues("\nSorted Values: ", a, N); // only print if N is small, to avoid clogging up the console

    printStats(N); // print out the statistics for mySort() in terms of compares and swaps

    if ( !isSortedAscending(a, N)){ // did the sort work?  this is an error check to be sure your sort worked ok
      cout << "ERROR: mySort() failed - the array is not in ascending order" << endl;
    }


  cout << "DONE" << endl;

}