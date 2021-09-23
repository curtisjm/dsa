#include <iostream>
#include <string>
#include <limits>
#include <vector>
#include <cmath>
#include <time.h>

using namespace std;

class Node {
public:
    // our node holds an integer
    int value = 0;
    // our node has a pointer to the next Node
    Node* next = nullptr;

    // contructor for our Node class
    Node(int i) {
        // store a copy of argument "i" in "value"
        value = i;
        // be sure next Node is null
        next = nullptr;
    }
};

class MaxPriorityQueueLinkedList {
private:
    Node* head = nullptr;
    // # of items in the heap, also next avaialble stack position
    int currentSize = 0;

public:
    MaxPriorityQueueLinkedList() {
        //cout << "Created new Max Priority Queue." << endl;
        currentSize = 0;
        head = nullptr;
    }

    // push a new Node instance into the Priority Queue
    void push(int i) {

        Node* temp = new Node(i); // create a node with i as the value

        // TODO: write your implementation of push here
        // don't forget to manage the "currentSize" value

    }

    // remove the Node with the largest value from the PQ
    int pop() {
        // TODO: 
        // Write your implementation of pop here
        // You will need to search the queue rooted at "head" to find the largest value
        // Don't forget to manage the "currentSize" value when you pop the Node
        // Don't forget to return -1 if your queue is empty
        // Otherwise you must return the value of the item you just popped (don't return the Node)
        // Don't forget to delete the Node you pop before returning, like:
        //      Node *temp;   // declaration of a pointer to the node that you be deleting
        //      delete temp;  // deletes the Node pointed to by "temp"
        //      

        return -1000;    // TODO: you must change this
    }

    bool isEmpty() {
        // TODO:
        // return true if the queue is empty, else false
        return (rand() % 100 < 50 ? false : true);  // TODO: you must change this! :)
    }

    int getSize() {
        // return the current size of the Max Priority Queue (how many nodes does it have?)
        return (rand() % 100 < 50 ? false : true);  // TODO: you must change this! :)
    }

    void print() {
        if (isEmpty()) {
            cout << "printQueue(): Max Prirority Queue is empty" << endl;
            return;
        }

        cout << "print(Current Size: " << currentSize << "): ";

        Node* next = head;
        while (next != nullptr) {
            cout << next->value << ", ";
            next = next->next;
        }
        cout << endl;
    }


};

// this routine will test your program and grade your submission
void testMe() {
    // create an instance of a Max Priority Queue
    MaxPriorityQueueLinkedList mpq;

    int totalScore = 0;

    cout << "------------------------------" << endl;
    cout << "        TESTING HW" << endl;
    cout << "------------------------------" << endl;

    // ismEmpty() working: 5 points
    bool isEmptyOk = true;
    // getSize() working: 5 points
    bool getSizeOk = true;

    if (!mpq.isEmpty()) {
        cout << "testMe() ERROR: isEmpty() returned false on a new Priority Queue, when it should have returned true." << endl;
        isEmptyOk = false;
    }

    mpq.push(1);

    if (mpq.isEmpty()) {
        cout << "testMe() ERROR: isEmpty() returned true on a Priority Queue with one pushed item, when it should have returned false." << endl;
        isEmptyOk = false;
    }

    mpq.push(2);

    if (mpq.getSize() != 2) {
        cout << "testMe() ERROR: getSize() did not return a value of 2 for a Priority Queue with two pushed items." << endl;
        getSizeOk = false;
    }

    // now we should have 1 item
    mpq.pop();

    if (mpq.getSize() != 1) {
        cout << "testMe() ERROR: getSize() did not return a value of 1 for a Priority Queue with one item after a pop." << endl;
        getSizeOk = false;
    }

    // now we should have zero items
    mpq.pop();

    if (mpq.getSize() != 0) {
        cout << "testMe() ERROR: getSize() did not return a value of 0 for a Priority Queue with zero items after a pop." << endl;
        getSizeOk = false;
    }

    if (!mpq.isEmpty()) {
        cout << "testMe() ERROR: isEmpty() did not return a value of true for a Priority Queue with zero items after a pop." << endl;
        isEmptyOk = false;
    }


    if (isEmptyOk) {
        cout << "testMe() isEmpty() looks good.  +5 points" << endl;
        totalScore += 5;
    }

    if (getSizeOk) {
        cout << "testMe() getSize() looks good.  +5 points" << endl;
        totalScore += 5;
    }

    cout << "testMe() Pop Test A: Testing push(1), pop() with new queue..." << endl;


    // test pop A
    {
        // create an instance of a Max Priority Queue
        MaxPriorityQueueLinkedList mpq;
        mpq.push(1);
        int num = mpq.pop();

        if (num != 1) {
            cout << "testMe() testMe() ERROR: pop() did not return a value of 1 after a push(1)." << endl;
            //exit(-1);
        } else {
            cout << "         Pop Test A: looks good.  +10 of 10 possible points" << endl;
            // push/pop working for simple test
            totalScore += 10;
        }
    }

    // test pop B
    {
        cout << "testMe() Pop Test B: Testing push(1), push(2),pop() with new queue..." << endl;
        // create an instance of a Max Priority Queue
        MaxPriorityQueueLinkedList mpq;
        mpq.push(1);
        mpq.push(2);
        int num = mpq.pop();
        if (num != 2) {
            cout << "testMe() ERROR: pop() did not return a value of 2 after a push(1), push(2), pop()." << endl;
            cout << "         Pop Test B failed.  0 of 10 possible points" << endl;
        } else {
            cout << "         Pop Test B: looks good.  +10 of 10 possible points" << endl;
            // push/pop working for simple test
            totalScore += 10;
        }

    }

    // test pop C
    {
        cout << "testMe() Pop Test C: Testing pop() on empty queue == -1..." << endl;
        // create an instance of a Max Priority Queue
        MaxPriorityQueueLinkedList mpq;

        int num = mpq.pop();
        if (num != -1) {
            cout << "testMe() ERROR: pop() did not return a value of -1 on an empty queue" << endl;
            cout << "         Pop Test C failed.  0 of 5 possible points" << endl;
        } else {
            cout << "         Pop Test C: looks good.  +5 of 5 possible points" << endl;
            // push/pop working for simple test
            totalScore += 5;
        }

    }

    // test push N / pop N
    {

        cout << "testMe() Push N/Pop N: Testing N pushes and N pops with new queue..." << endl;
        // create an instance of a Max Priority Queue
        MaxPriorityQueueLinkedList mpq;
        bool finishedOk = true;
        int maxCount = 10;
        // load up a bunch of values
        for (int i = 0; i < maxCount; i++) mpq.push(i);
        for (int i = 0; i < maxCount; i++) {
            if (mpq.isEmpty()) {
                cout << "         ERROR: isEmpty() returned true too soon. " << endl;
                finishedOk = false;
                break;
            }
            mpq.pop();
        }

        if (finishedOk) {
            cout << "         Push N/Pop N looks good.  +10 of 10 possible points" << endl;
            totalScore += 10;
        } else
            cout << "         Push N/Pop N failed.  0 of 10 possible points" << endl;

    }

    // stress test
    {
        int maxCount = 1000;
        cout << "testMe() Stress test: Pushing and Popping randomly " << maxCount << "  times..." << endl;
        // create an instance of a Max Priority Queue
        MaxPriorityQueueLinkedList mpq;

        // random push/pop test
        srand(time(NULL));
        //vector<int> v; // a vector of integers

        int myCount = 0;
        for (int i = 0; i < maxCount; i++) {
            // goes from approx -maxCount to maxCount
            int rnum = rand() % maxCount * 2 - maxCount;
            // pop a value
            if (rnum < 0) {
                int returnValue = mpq.pop();
                if (returnValue >= 0)
                    // decrement our counter; else it was already empty
                    myCount--;
                // push a value
            } else {
                // push the random value
                mpq.push(rnum);
                // increment our counter
                myCount++;
            }
        }

        if (myCount != mpq.getSize()) {
            cout << "         ERROR: test count " << myCount << " does not match mpq getSize() of " << mpq.getSize() << endl;
            cout << "         Stress Test failed.  0 of 10 possible points" << endl;
        } else {
            // stress test ok
            totalScore += 10;
            cout << "         Stress Test looks good.  +10 of 10 possible points" << endl;
        }
    }

    // sort test
    {
        int maxCount = 1000;
        cout << "testMe() Sort test: Pushing " << maxCount << "  times, then popping " << maxCount << " times.. should produce a descending sorted list... " << endl;
        // create an instance of a Max Priority Queue
        MaxPriorityQueueLinkedList mpq;

        bool isDescending = true;

        // random push / pop test
        srand(time(NULL));
        // a vector of integers
        vector<int> v;
        int myCount = 0;
        for (int i = 0; i < maxCount; i++) {
            // goes from approx 0 to maxCount-1
            int rnum = rand() % maxCount;
            // push the random value
            mpq.push(rnum);
            // increment our counter
            myCount++;
            if (myCount != mpq.getSize())
                isDescending = false;
        }

        // big positive value
        int lastValue = INT32_MAX;
        cout << "         checking descending... " << endl;

        for (int i = 0; i < maxCount; i++) {
            int popValue = mpq.pop();
            if (popValue < 0) {
                cout << "         Error: sort test failed... ran out of values to pop" << endl;
                bool isDescending = false;
                break;
            }

            if (popValue > lastValue) {
                cout << "Error: sort test failed... " << lastValue << " should be >= " << popValue << endl;
                // values are not being returned from largest to smallest
                bool isDescending = false;
                break;
            }
            // save for next loop iteration
            lastValue = popValue;
        }

        if (!isDescending) {
            cout << "         Error: sort test failed... Your pop() does not appear to be removing values  " << endl;
            cout << "         Sort Test failed.  0 of 25 possible points" << endl;
        } else {
            totalScore += 25;
            cout << "         Sort Test Looks good.  +25 of 25 possible points" << endl;
        }

        cout << "         Done with sort test" << endl;
    }

    cout << "-----------------------------------------------------------" << endl;
    cout << "testMe()  DONE! Your preliminary score is: " << totalScore << " out of 80 points" << endl;
    cout << "-----------------------------------------------------------" << endl;
}


// get an integer from input (until success)
int getIntegerInput() {
    int num;
    // try to get an integer
    cin >> num;

    // keep trying until we get a good integer
    while (true) {
        // that was not an integer
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "That's not an integer - please try again: ";
            cin >> num;
        }

        // else if we got the integer, break out of the loop
        if (!cin.fail())
            break;
    }
    // return the input integer
    return num;
}

int main() {
    std::cout << "HW Starter: Max Priority Queue using a Linked List\n";
    cout << "Enter any positive number to push it, or enter a negative value to pop the max value (then hit return) " << endl;

    // test mode below - uncomment when you want to see your grade

    //testMe();          // TODO: uncomment this when you are ready to test

    // interactive mode below

    // create an instance of a Max Priority Queue
    MaxPriorityQueueLinkedList mpq;

    // "mpq" is the instance of our Max Priority Queue
    // now we can fill or empty it by pushing and popping

    // loop forever
    while (true) {
        int num;
        cout << "Enter an integer value: ";
        // get an integer from input until success
        num = getIntegerInput();

        if (num < 0) {
            if (!mpq.isEmpty())
                // pop the max value from the mp queue
                cout << "Popped " << mpq.pop() << endl;
            else
                cout << "Sorry - Max Priority Queue is empty. Please push a positive value first." << endl;
        } else
            // push num into the max priority queue
            mpq.push(num);

        // print contents of the max priority queue
        mpq.print();
    }
}
