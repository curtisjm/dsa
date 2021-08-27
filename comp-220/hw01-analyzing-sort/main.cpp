#include <iostream>
#include <cmath>
#include <chrono>
#include <iomanip>

/*

  Submission:  Along with a link to ths repl, remember to copy and fill out this google spreadsheet with your results:

  https://docs.google.com/spreadsheets/d/1gGLdji0SbLiGeoUvvcCdixevRXZ5XWg2hDedQvOxumA/edit?usp=sharing 

*/

using namespace std;

// used to keep track of swaps
static long swapCount = 0;
// used to keep track of compares
static long compareCount = 0;
// used to keep track of elapsed time
static chrono::duration<double> time_span;

// return -1, 0, or 1 to indicate if a is < ==, or > b
int compare(int a, int b) {
	compareCount++;
	if (a > b) return 1;
	if (a < b) return -1;
	// they must be equal
	return 0;
}

// swap i and j and increment counter
void swap(int a[], int i, int j) {
	swapCount++;
	int temp = a[i];
	a[i] = a[j];
	a[j] = temp;
}

void printValues(const char* msg, int a[], int N) {
	cout << msg << endl;
	for (int i = 0; i < N; i++)
		cout << a[i] << ", ";
	cout << endl;
}

void printStats(int N) {
	cout << "\nPrint Stats" << endl;
	cout << "--------------------------------------" << endl;
	cout << "For value N = " << N << endl;
	cout << "Total Swaps = " << swapCount << endl;
	cout << "Total Compares = " << compareCount << endl;
	cout << "Time Duration " << fixed << setprecision(10) << time_span.count() << " seconds." << endl;
	cout << "Log(n) = " << log2(N) << endl;
	cout << "nLog(n) = " << N * log2(N) << endl;
	cout << "N^2 = " << (N) * (N) << endl;

	compareCount = 0;
	swapCount = 0;
}

int partition(int arr[], int left, int right) {
	int pivot = arr[right];
	// i is the index of the smaller number
	int i = left - 1;

	// j keeps track of current number that we are examining
	for (int j = left; j <= right - 1; j++) {
		if (compare(arr[j], pivot) == -1) {
			i++;
			swap(arr, i, j);
		}
		// all nums up to i are < pivot
		// all nums up to j are >= pivot
	}

	// make sure pivot is in between two groups
	swap(arr, i + 1, right);
	// return index of the pivot
	return (i + 1);
}

void quickSort(int arr[], int left, int right) {
	// already in right place
	if (left >= right) {
		return;
	}

	// beginning partition index
	int p = partition(arr, left, right);

	// left / lower side of array
	quickSort(arr, left, p - 1);
	// right / upper side of array
	quickSort(arr, p + 1, right);
}

void mySort(int a[], int N) {
	// start time
	chrono::steady_clock::time_point start_time = chrono::steady_clock::now();

	// pass array, starting index, and ending index
	quickSort(a, 0, N - 1);

	// end time
	chrono::steady_clock::time_point end_time = chrono::steady_clock::now();

	// duration
	time_span = chrono::duration_cast<chrono::duration<double>>(end_time - start_time);
}

// called after sorting to verify results
bool isSortedAscending(int a[], int N) {
	for (int i = 0; i < N - 1; i++) {
		// the order of these 2 are inverted, so the array is not ascending
		if (a[i] > a[i + 1])
			return false;
	}

	// if we got this afar, the array must be sorted in ascending order
	return true;
}

int main() {
	std::cout << "HW My Sort\n";

	// number of items we are sorting
	const int N = 64;
	// array of N items
	int a[N];

	for (int i = 0; i < N; i++) {
		// generate random values in the range of 1 to N, inclusive
		a[i] = rand() % N + 1;
	}

	// only print if N is small, to avoid clogging up the console
	if (N < 100) printValues("Initial Values: ", a, N);

	// sort array a containing N values (TODO - you need to write this!)
	mySort(a, N);

	// only print if N is small, to avoid clogging up the console
	if (N < 100) printValues("\nSorted Values: ", a, N);

	// print out the statistics for mySort() in terms of compares and swaps
	printStats(N);

	// did the sort work? this is an error check to be sure your sort worked ok
	if (!isSortedAscending(a, N)) {
		cout << "ERROR: mySort() failed - the array is not in ascending order" << endl;
	}

	cout << "DONE" << endl;
}