#include <iostream>
#include <string>
#include <limits>
#include <cmath>
#include <iomanip>

using namespace std;

//
// CLASS: NODE 
//

class Node {
public:
    int value = 0;           // our node holds an integer
    Node* left = nullptr;    // our node has a pointer to the left child
    Node* right = nullptr;    // our node has a pointer to the left child

    Node(int i) {             // constructor for our Node class
        value = i;                // store a copy of argument "i" in "value"
        left = nullptr;           // be sure next Node is null
        right = nullptr;           // be sure next Node is null
    }
};

//
// CLASS:  MAX PRIORITY QUEUE
//

class MaxPriorityQueueBinaryHeap {
private:
    Node* head = nullptr;
    int currentSize = 0;   // # of items in the stack, also next avaialble stack position
    int compareCount = 0;  // count of how many compares during the lifetime of this MPQ
    int swapCount = 0;    // count of how many swaps during the lifetime of this MPQ

public:
    MaxPriorityQueueBinaryHeap() {
        //cout << "Created new Max Priority Queue." << endl;

        if (!isEmpty()) { // new queues should be empty
            cout << "WARNING !!!: Is your isEmpty() function working properly?" << endl;
        }
        head = nullptr;
    }

    //
    // COMPARE
    //

    int comparePQ(int a, int b) {
        compareCount++;
        if (a > b) return 1;
        if (a < b) return -1;
        return 0;  // they must be equal
    }

    //
    // swap i and j and increment counter
    //
    void swapPQ(int& a, int& b) { // note: these are references, so we can change them
        swapCount++;
        int temp = a;
        a = b;
        b = temp;
    }

    //
    // SWAP UP
    //

    void swapUp(Node* child, Node* parent) {
        if (comparePQ(child->value, parent->value) == 1) { // if child is larger its parent
            swapPQ(child->value, parent->value); // bubble up the larger value (not the node)
        }
    }

    //
    // BUBBLE DOWN (called by pop)
    //

    // recursively swap the smaller value down, starting with the head
    void bubbleDown(Node* start = nullptr, Node* parent = nullptr) {

        if (start == nullptr)
            return; // nothing to bubble down to

        if (parent != nullptr) // if we are not at the head
            swapUp(start, parent); // swap start with parent if start is bigger than parent

          // pick the larger of the 2 children that exist and bubble down if needed

        Node* pickChild = nullptr;   // max PQ: pick largest

        if (start->left == nullptr) // only a right child
            pickChild = start->right;

        else if (start->right == nullptr) // only a left child
            pickChild = start->left;

        else { // pick the larger of two children
            int cmp = comparePQ(start->left->value, start->right->value); // max PQ: pick largest
            pickChild = (cmp == 1 ? start->left : start->right); // choose left if cmp == 1, else right
        }

        bubbleDown(pickChild, start); // consider largest Child
          // note: largestChild could be nullptr if start has no children.
          //       this case is handled by the null check at top
    }

    //
    // PUSH - push a new Node instance into the Priority Queue
    //

    //  start: default value for "start==nullptr" supports recursion
    //  parent:  the pointer to the parent of "start", if any
    void push(int i, Node* start = nullptr, Node* parent = nullptr) {

        if (start == nullptr) {
            // start was not specified, so assume the head
            start = head;
            // a precaution - parent must always be null for the head
            parent = nullptr;
        }
        // if the start is still null, we insert the new node at the top
        if (start == nullptr) {
            head = new Node(i);
            // insert left if left is null
        } else if (start->left == nullptr) {
            start->left = new Node(i);
            // swap if heap invariant is not satisfied
            swapUp(start->left, start);
            // insert right if right is null
        } else if (start->right == nullptr) {
            start->right = new Node(i);
            // swap if heap invariant is not satisfied
            swapUp(start->right, start);
            // we need to recurse either left or right to find a place to insert
        } else {
            // randomly choose 0 or 1
            int randNum = rand() % 2;
            if (randNum == 0) {
                // recurse left, pass the left child
                push(i, start->left, start);
            } else {
                // recurse right, pass the right child
                push(i, start->right, start);
            }
        }

        // preserve heap invariant
        if (parent != nullptr) {
            swapUp(start, parent);
        }

        currentSize++;
    }

    //
    // POP 
    //

    // remove the top node in the PQ (largest if Max PQ, else smallest )
    //  start: default value for "start==nullptr" supports recursion
    //  ptrToStart: the address of the left or right pointer that refers to start
    int pop(Node* start = nullptr, Node** ptrToStart = nullptr) {

        // start was not specified, so assume the head
        if (start == nullptr) {
            start = head;
            // a precaution - parent must always be null for the head
            ptrToStart = nullptr;
        }

        // PQ is empty
        if (start == nullptr) {
            return -1;
        }

        // when start is a leaf node (has no children)
        if (start->left == nullptr && start->right == nullptr) {
            // keep hold of node we will delete
            Node temp = *head;
            // make initial swap of head with the selected leaf node
            swapPQ(start->value, head->value);
            // remove value we are popping from tree
            delete start;
            // update pointer that was pointing to start
            // when tree has more nodes than just a head
            if (ptrToStart != nullptr) {
                *ptrToStart = nullptr;
                bubbleDown(head);
            } else {
                // if we popped and deleted the head (last value in tree)
                head = nullptr;
            }

            currentSize--;
            return temp.value;
        }

        // maintain a balanced tree by randomly choosing a direction
        int randNum = rand() % 2;
        // traverse down left side of tree
        if (randNum == 0) {
            // pass new value for start until we find a leaf node and a pointer to it to nullify later
            return pop(start->left, &start->left);
        }
        // traverse down right side of tree
        return pop(start->right, &start->right);
    }

    //
    // IS EMPTY
    //

    bool isEmpty() {
        // Return true if the queue is empty, else false
        return (currentSize == 0); // return true if emtpy, otherwise false
    }

    //
    // GET SIZE
    //

    int getSize() {
        // Return the current size of the Max Priority Queue (how many nodes does it have?)
        return currentSize; // return the count of items in the stack, not the maxSize
    }

    //
    // PRINT QUEUE
    //

    void print(Node* start = nullptr) {

        if (start == nullptr) // if nullptr, then we should start from the head
            start = head;

        if (start == nullptr) {
            cout << "Empty tree" << endl;
            return; // head is null, so nothing to print
        }

        // current
        cout << start->value << ", ";  // print the current node

        // LEFT
        if (start->left != nullptr)   // only recurse if we have a left child
            print(start->left);

        // RIGHT
        if (start->right != nullptr)   // only recurse if we have a right child
            print(start->right);
    }

    //
    // PRINT TREE GEEKS
    //   FROM https://www.geeksforgeeks.org/print-binary-tree-2-dimensions/
    //

    // Function to print binary tree in 2D  
    // It does reverse inorder traversal  
    // count = allocation of space per node horizontally - spreads out the tree left to right
    void printTreeGeeks(int count = 10, int space = 0, char prefix = '<', Node* start = nullptr) {

        if (start == nullptr)
            start = head;         // start was not specified, so assume the head

        if (start == nullptr) {
            cout << "\nPrinting Tree (Geeks): Empty tree" << endl;
            return; // head is null, so nothing to print
        }

        if (space == 0) cout << "\nPrinting Tree (Geeks) ****************" << endl; // print if we are at the first level

        // Increase distance between levels  
        space += count;

        // Process right child first  
        if (start->right != nullptr)
            printTreeGeeks(count, space, '/', start->right);

        // Print current node after space  
        // count  
        cout << endl;
        // for (int i = count; i < space; i++)  
        //     cout<<" ";  
        cout << setw(space - count) << prefix << start->value;
        if (start->left == nullptr && start->right == nullptr)
            cout << ":\n";
        else if (start->left == nullptr)
            cout << ".\n";
        else if (start->right == nullptr)
            cout << "`\n";
        else
            cout << "<\n";

        // Process left child  
        if (start->left != nullptr)
            printTreeGeeks(count, space, '\\', start->left);
    }

    //
    // PRINT STATS
    //

    void printStats() {

        int N = currentSize;

        cout << "\nResults:" << endl;
        cout << "Number of elements (N) = " << N << endl;
        cout << "Log(N) = " << log2(N) << endl;
        cout << "NLog(N) = " << N * log2(N) << endl;
        cout << "N^2 = " << N * N << endl;
        cout << "(N-1)^2 = " << (N - 1) * (N - 1) << endl;
        cout << "N(N-1)/2 = " << (N) * (N - 1) / 2 << endl;
        cout << "N(N-1)/2 = " << (N) * (N - 1) / 2 << endl;
        cout << "Average Number of compares = " << compareCount << endl;
        cout << "Average Number of swaps = " << swapCount << endl;

    }

};

//
// TEST ME - this routine will test your program and grade your submission
//

void testMe() {

    MaxPriorityQueueBinaryHeap mpq; // create an instance of a Max Priority Queue

    int totalScore = 0;

    cout << "------------------------------" << endl;
    cout << "        TESTING HW" << endl;
    cout << "------------------------------" << endl;

    bool isEmptyOk = true;     // ismEmpty() working: 5 points
    bool getSizeOk = true;     // getSize() working: 5 points

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

    mpq.pop(); // now we should have 1 item

    if (mpq.getSize() != 1) {
        cout << "testMe() ERROR: getSize() did not return a value of 1 for a Priority Queue with one item after a pop." << endl;
        getSizeOk = false;
    }

    mpq.pop(); // now we should have zero items

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

    //
    // TEST POP A
    //

    cout << "testMe() Pop Test A: Testing push(1), pop() with new queue..." << endl;

    {
        MaxPriorityQueueBinaryHeap mpq; // create an instance of a Max Priority Queue
        mpq.push(1);
        int num = mpq.pop();

        if (num != 1) {
            cout << "testMe() testMe() ERROR: pop() did not return a value of 1 after a push(1)." << endl;
            //exit(-1);
        } else {
            cout << "         Pop Test A: looks good.  +10 of 10 possible points" << endl;
            totalScore += 10; // push/pop working for simple test
        }
    }

    //
    // TEST POP B
    //

    {
        cout << "testMe() Pop Test B: Testing push(1), push(2),pop() with new queue..." << endl;
        MaxPriorityQueueBinaryHeap mpq; // create an instance of a Max Priority Queue
        mpq.push(1);
        mpq.push(2);
        int num = mpq.pop();
        if (num != 2) {
            cout << "testMe() ERROR: pop() did not return a value of 2 after a push(1), push(2), pop()." << endl;
            cout << "         Pop Test B failed.  0 of 10 possible points" << endl;
        } else {
            cout << "         Pop Test B: looks good.  +10 of 10 possible points" << endl;
            totalScore += 10; // push/pop working for simple test
        }

    }

    //
    // TEST POP C
    //

    {
        cout << "testMe() Pop Test C: Testing pop() on empty queue == -1..." << endl;
        MaxPriorityQueueBinaryHeap mpq; // create an instance of a Max Priority Queue

        int num = mpq.pop();
        if (num != -1) {
            cout << "testMe() ERROR: pop() did not return a value of -1 on an empty queue" << endl;
            cout << "         Pop Test C failed.  0 of 5 possible points" << endl;
        } else {
            cout << "         Pop Test C: looks good.  +5 of 5 possible points" << endl;
            totalScore += 5; // push/pop working for simple test
        }

    }

    //
    // TEST PUSH N / POP N
    //

    {

        cout << "testMe() Push N/Pop N: Testing N pushes and N pops with new queue..." << endl;
        MaxPriorityQueueBinaryHeap mpq; // create an instance of a Max Priority Queue
        bool finishedOk = true;
        int maxCount = 10;
        for (int i = 0; i < maxCount; i++) mpq.push(i); // load up a bunch of values
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

    //
    // STRESS TEST
    //

    {
        int maxCount = 1000;
        cout << "testMe() Stress test: Pushing and Popping randomly " << maxCount << "  times..." << endl;
        MaxPriorityQueueBinaryHeap mpq; // create an instance of a Max Priority Queue

      // random push/pop test
        srand(time(NULL));

        int myCount = 0;
        for (int i = 0; i < maxCount; i++) {
            int rnum = rand() % maxCount * 2 - maxCount; // goes from approx -maxCount to maxCount
            if (rnum < 0) { // pop a value
                int returnValue = mpq.pop();
                if (returnValue >= 0)
                    myCount--; // decrement our counter; else it was already empty
            } else { // push a avlue
                mpq.push(rnum); // push the random value
                myCount++; // increment our counter
            }
        }

        if (myCount != mpq.getSize()) {
            cout << "         ERROR: test count " << myCount << " does not match mpq getSize() of " << mpq.getSize() << endl;
            cout << "         Stress Test failed.  0 of 10 possible points" << endl;
        } else {
            totalScore += 10; // stress test ok
            cout << "         Stress Test looks good.  +10 of 10 possible points" << endl;
        }
    }

    //
    // SORT TEST
    //

    {
        int maxCount = 1000;
        cout << "testMe() Sort test: Pushing " << maxCount << "  times, then popping " << maxCount << " times.. should produce a descending sorted list... " << endl;
        MaxPriorityQueueBinaryHeap mpq; // create an instance of a Max Priority Queue

        bool isDescending = true;


        // random push/pop test
        srand(time(NULL));
        int myCount = 0;
        for (int i = 0; i < maxCount; i++) {
            int rnum = rand() % maxCount; // goes from approx 0 to maxCount-1
            mpq.push(rnum); // push the random value
            myCount++; // increment our counter
            if (myCount != mpq.getSize())
                isDescending = false;
        }

        int lastValue = INT32_MAX; // big positive value
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
                bool isDescending = false; // values are not being returned from largest to smallest
                break;
            }
            lastValue = popValue; // save for next loop iteration
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

    //
    // TEST BONUS TEST
    //

    {

        cout << "testMe() Push 1K(random)/Pop 1K: Testing 1K pushes and 1K pops with new queue..." << endl;
        MaxPriorityQueueBinaryHeap mpq; // create an instance of a Max Priority Queue
        bool bonusOk = true;
        int maxCount = 10000;
        for (int i = 0; i < maxCount; i++) mpq.push(rand() % maxCount); // load up a bunch of random values
        cout << " Stats after push 1K: " << endl;
        mpq.printStats();
        for (int i = 0; i < maxCount; i++) {
            if (mpq.isEmpty()) {
                cout << "         ERROR: isEmpty() returned true too soon. " << endl;
                bonusOk = false;
                break;
            }
            mpq.pop();
        }
        cout << " Stats after pop 1K: " << endl;
        mpq.printStats();

        if (bonusOk) {
            cout << "         Push 1K/Pop 1K looks good - used for bonus." << endl;
        } else
            cout << "         Push 1K/Pop 1K failed - used for bonus" << endl;

    }

    cout << "-----------------------------------------------------------" << endl;
    cout << "testMe()  DONE! Your preliminary score is: " << totalScore << " out of 80 points" << endl;
    cout << "-----------------------------------------------------------" << endl;



}

//
// Get an integer from input (until success)
// (this function does a bit of extra work to avoid input errors)
//

int getIntegerInput() {
    int num;
    cin >> num;     // try to get an integer

    while (true) { // keep trying until we get a good integer
        if (cin.fail()) { // that was not an integer
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "That's not an integer - please try again: ";
            cin >> num;
        }

        if (!cin.fail())  // else if we got the integer, break out of the loop
            break;
    }
    return num; // return the input integer

}

//
// MAIN
//

int main() {
    std::cout << "HW BONUS HINT: Max Priority Queue using a Binary Tree\n";
    cout << "Enter any positive number to push it, or enter a negative value to pop the max value (then hit return) " << endl;

    // test mode below - uncomment when you want to see your grade

       // testMe();

    // interactive mode below

    MaxPriorityQueueBinaryHeap mpq; // create an instance of a Max Priority Queue

    // "mpq" is the instance of our Max Priority Queue
    // now we can fill or empty it by pushing and popping

    while (true) { // loop forever

        int num;
        cout << "Enter an integer value: ";
        num = getIntegerInput(); // get an integer from input until success

        if (num < 0) {
            if (!mpq.isEmpty())
                cout << "Popped " << mpq.pop() << endl; // pop the max value from the mp queue
            else
                cout << "Sorry - Max Priority Queue is empty. Please push a positive value first." << endl;
        }

        else mpq.push(num); // push num into the max priority queue

        // mpq.print(); // print contents of the max priority queue
        mpq.printTreeGeeks(); // print as a tree

    }
}
