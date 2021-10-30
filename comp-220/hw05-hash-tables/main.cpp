#include <iostream>
#include <chrono>
#include <string>
#include <vector>

using namespace std;

static int compareCount = 0;

//
// NODE CLASS  
//

class Node {
public:
    string key; // our string type is a C++ "container" that is more elegant than array's of characters
    int value = 0;
    Node* next = nullptr;
    Node* prev = nullptr;

    Node(string _key, int _val) { // constructor
        key = _key; // initialize the key in the node to our passed in _key argument
        value = _val; // initialize the val in the node to our passed in _val argument
    }
}; // end Node Class

//
// SYMBOL TABLE CLASS
//

class SymbolTable {

private:
    int currentSize; // number of entries in the SymbolTable (not the size of the array!)
    int maxBuckets; // max size of array (static)
    Node** hashTable; // array of pointers to Nodes
    int bucketsUsed = 0;
    int debug = false;

public:

    SymbolTable(int _maxBuckets = 64, bool _debug = false) { // DONE for you: constructor
        currentSize = 0;
        maxBuckets = _maxBuckets; // what we call "M" in our class code
        debug = _debug;
        hashTable = new Node * [maxBuckets]; // dynamic allocation of our hashTable of size maxBuckets pointers to nodes
        for (int i = 0; i < maxBuckets; i++) {
            hashTable[i] = nullptr; // initialize all table pointers to null
        }
    } // end constructor

    ~SymbolTable() { // DONE for you: our destructor
        for (int i = 0; i < maxBuckets; i++) {
            if (hashTable[i] != nullptr)
                for (Node* temp = hashTable[i]; temp != nullptr; ) {
                    Node* deleteThis = temp;
                    temp = temp->next; // delete the current node
                }
        }
        delete[] hashTable;   // free the hashTable when we leave
    } // end destructor

    //
    // HASH FUNCTION
    //

    int hashFunction(string s, int m) {  // simple summation of ascii values mod m
        // convert s into a number in the range of 0 to m-1 (so we can use that number as a location in an array of size "m" )
        int sum = 0;
        for (char c : s)
            sum += c;

        return sum % m; // return a value here in the range 0 to m-1
    }

    //
    // FIND NODE
    //

    Node* findNodeInLinkedList(Node* temp, string key) {
        Node* n = temp;
        // search the linked list of nodes started at "temp" for a node that contains the requested key
        // stop searching when temp->next is nullptr or when key is found
        while (n->key != key && n->next != nullptr)
            n = n->next;

        // return the desired node if found, if not found return null
        return n->key == key ? n : nullptr;
    }

    //
    // insertKeyVal - insert the given key, value pair into the ST; return true if a value was inserted
    //

    bool insertKeyVal(string key, int val) {
        // determine the bucketId (array location) using the hashFunction()
        int bucketId = hashFunction(key, maxBuckets);

        // if the bucket is null, insert a new node with the key and value at the head of that bucket's linked list
        if (hashTable[bucketId] == nullptr) {
            hashTable[bucketId] = new Node(key, val);
            currentSize++;
            bucketsUsed++;
            return true;
        }

        // search the linked list at that bucket to determine if the node is in the bucket
        Node* existingNode = findNodeInLinkedList(hashTable[bucketId], key);
        // if the node is not found in the bucket, insert a new node at the head of the bucket's linked list and return true
        if (existingNode == nullptr) {
            // create new node
            Node* newNode = new Node(key, val);
            // insert newNode in front of the head
            newNode->next = hashTable[bucketId];
            // update backward pointer
            hashTable[bucketId]->prev = newNode;
            // update head
            hashTable[bucketId] = newNode;
            currentSize++;
            return true;
        }

        // if the node is found in the bucket, overwrite its value and return false indicating that no new key was created
        existingNode->value = val;
        return false;
    }

    //
    // deleteKey - delete the requested key if it exists, return true if found & deleted
    //

    bool deleteKey(string key) {
        // determine the bucketId (array location) using the hashFunction()
        int bucketId = hashFunction(key, maxBuckets);

        // if the bucket is empty, return false (nothing deleted)
        if (hashTable[bucketId] == nullptr)
            return false;

        // if the bucket is not empty, search its doubly-linked list using findNodeInLinkedList() to see if the key exists
        Node* toDelete = findNodeInLinkedList(hashTable[bucketId], key);

        // if the node is not found, return false (nothing deleted)
        if (toDelete == nullptr)
            return false;

        // if the node is found at the head of the doubly-linked list, delete it and update the head and new first node's prev pointer
        if (toDelete == hashTable[bucketId]) {
            // detach old head
            hashTable[bucketId] = toDelete->next;
            // if there is still a node in the bucket
            if (toDelete->next != nullptr)
                // nullify previouse pointer for new head
                toDelete->next->prev = nullptr;
            else
                // the bucket is empty
                bucketsUsed--;
            delete toDelete;
            currentSize--;
            return true;
        }

        // if the node is found at the end of the doubly-linked list, delete it and update the new last node's next pointer
        if (toDelete->next == nullptr) {
            // detach tail node
            toDelete->prev->next = nullptr;
            delete toDelete;
            currentSize--;
            return true;
        }

        // if the node is found in the middle of the doubly-linked list, remove it, and update both prev and next pointers
        // update previous node's next pointer
        toDelete->prev->next = toDelete->next;
        // update next node's previous pointer
        toDelete->next->prev = toDelete->prev;
        delete toDelete;
        currentSize--;
        return true;
    }

    //
    // getValue  - return the value associated with a specific string key , return -1 if not found
    //        

    int getValue(string key) {
        // determine the bucketId (array location) using the hashFunction()
        int bucketId = hashFunction(key, maxBuckets);

        // if the bucket is empty, return -1 (nothing found)
        if (hashTable[bucketId] == nullptr)
            return -1;

        // if the bucket is not empty, search its doubly-linked list using findNodeInLinkedList() to see if the key exists
        Node* foundNode = findNodeInLinkedList(hashTable[bucketId], key);

        // if the node is not found, return -1
        if (foundNode == nullptr)
            return -1;

        // if the node is found, return its value
        return foundNode->value;
    }

    //
    // containsKey - Return True if the ST contains the requested key
    //

    bool containsKey(string key) {
        // determine the bucketId (array location) using the hashFunction()
        int bucketId = hashFunction(key, maxBuckets);
        
        // if the bucket is empty, return false (nothing found)
        if (hashTable[bucketId] == nullptr)
            return false;

        // if the bucket is not empty, search its linked list using findNodeInLinkedList() to see if the key exists
        Node* foundNode = findNodeInLinkedList(hashTable[bucketId], key);

        // if the node is not found, return false
        if (foundNode == nullptr)
            return false;

        // if the node is found, return true
        return true;
    } 

  //
  // isEmpty() return true if the symbol table is isEmpty
  //

    bool isEmpty() {
        // return true or false depending on whether the symbol table is empty
        return (currentSize == 0);
    }

    //
    // getSize() - return the number of members in the Symbol Table
    //

    int getSize() {
        // return the current size of the symbol table (the current number of nodes, not buckets used.)
        return currentSize;
    }

    //
    // printForward WRITTEN FOR YOU (prints in the forward direction from head to end of linked list)
    //

    void printMe() {
        for (int i = 0; i < maxBuckets; i++) {  // for each bucket
            cout << "Bucket #" << i << "| ";
            for (Node* temp = hashTable[i]; temp != nullptr; temp = temp->next) {  // for each node in this bucket
                if (temp->next != nullptr)
                    cout << temp->key << ":" << temp->value << ", ";  // add the comma
                else
                    cout << temp->key << ":" << temp->value << ".";   // add a period
            }
            cout << endl;
        }
        cout << "Load Factor: " << loadFactor() << endl << endl;
    }

    //
    // LOAD FACTOR
    //
    float loadFactor() {
        // return the floating point load factor for the hash table (ratio of buckets use to total number of buckets)
        return currentSize == 0 ? 0.0 : (float)currentSize / (float)maxBuckets;
    }

    //
    // testLinks - this method, if called, will test integrity of all next/previous links in the symbol table
    //

    bool testLinks() {
        bool passedTest = true;
        cout << "Testing Links on all buckets..." << endl;
        for (int i = 0; i < maxBuckets; i++) {  // for each bucket
            int count = 0; // how many nodes we find in this bucket
            cout << "Bucket #" << i << "| ";
            for (Node* temp = hashTable[i]; temp != nullptr; temp = temp->next) {  // for each node in this bucket
                if (temp->next != nullptr) {
                    cout << temp->key << ":" << temp->value << "(" << count++ << ")->next:";  // add the next symbol and inc the count
                }

                else { // found the end, now go backwards
                  //cout << temp->key << ":" << temp->value << "(" << count-- << ")REVERSE->";   // add the last key and dec the count

                    for (Node* temp2 = temp; temp2 != nullptr; temp2 = temp2->prev) { // go backwards
                        if (temp2->prev != nullptr)
                            cout << temp2->key << ":" << temp2->value << "(" << count-- << ")->prev:";  // add the next symbol
                        else {
                            cout << temp2->key << ":" << temp2->value << "(" << count << ")->prev";   // add a period
                        }
                    }
                    if (count != 0) {
                        cout << "==nullptr (ERROR, count!=0, prev pointer broken?!)" << endl;
                        passedTest = false;
                    } else {
                        cout << "==nullptr (OK, count==0)" << endl;
                    }
                }
            }
            cout << endl;
        }
        cout << "Load Factor: " << loadFactor() << endl << endl;
        return passedTest;
    } // end test Links

}; // end SymbolTable Class

/*
Call this global function when ready to test your entire assignment.
*/

void testMe() {
    int totalPoints = 0;
    bool doubleLinksWork = true; // assume double-linked list is implemented and works until/if we find a failure using testLinks()

    cout << "*********************************" << endl;
    cout << "testMe() here..." << endl;
    cout << "*********************************" << endl;

    {
        cout << "Test A: Load Factor:" << endl;
        bool pass = true;
        int M = 10;
        SymbolTable st(M); // create the symbol table of size N
        bool success1 = st.insertKeyVal("foo", 12);
        bool success2 = st.deleteKey("foo");
        if (!success1 || !success2 || st.loadFactor() != 0) { pass = false;  cout << "   loadFactor() failed " << endl; }
        if (st.testLinks() != true) { doubleLinksWork = false;  cout << "   NOTE: testLinks() failed." << endl; }
        cout << (pass ? "  passed (+9)" : " failed (+0)") << endl;
        totalPoints += (pass ? 9 : 0);
    }

    {
        cout << "Test B: isEmtpy()" << endl;
        bool pass = true;
        int M = 10;
        SymbolTable st(M); // create the symbol table of size N
        bool success1 = st.insertKeyVal("foo", 12);
        bool success2 = st.deleteKey("foo");
        if (!success1 || !success2 || st.isEmpty() != true) { pass = false; cout << "   isEmpty() failed " << endl; }
        if (st.testLinks() != true) { doubleLinksWork = false;  cout << "   NOTE: testLinks() failed." << endl; }
        cout << (pass ? "  passed (+9)" : " failed (+0)") << endl;
        totalPoints += (pass ? 9 : 0);
    }

    {
        cout << "Test C: getSize()" << endl;
        bool pass = true;
        int M = 10;
        SymbolTable st(M); // create the symbol table of size N
        bool success1 = st.insertKeyVal("foo", 12);
        bool success2 = st.deleteKey("foo");
        if (!success1 || !success2 || st.getSize() != 0) { pass = false;cout << "   getSize() failed " << endl; }
        if (st.testLinks() != true) { doubleLinksWork = false;  cout << "   NOTE: testLinks() failed." << endl; }
        cout << (pass ? "  passed (+9)" : " failed (+0)") << endl;
        totalPoints += (pass ? 9 : 0);
    }

    {
        cout << "Test D: insertKeyVal()" << endl;
        bool pass = true;
        int M = 10;
        SymbolTable st(M); // create the symbol table of size N
        bool success = st.insertKeyVal("foo", 12);
        if (success == false) { pass = false; cout << "   insert failed" << endl; }
        if (st.isEmpty() == true) { pass = false; cout << "   isEmpty() failed " << endl; }
        if (st.getSize() != 1) { pass = false;cout << "   getSize() failed " << endl; }
        if (st.loadFactor() != 0.1f) { pass = false;  cout << "   loadFactor() failed " << endl; }
        if (st.testLinks() != true) { doubleLinksWork = false;  cout << "   NOTE: testLinks() failed." << endl; }
        cout << (pass ? "  passed (+9)" : " failed (+0)") << endl;
        totalPoints += (pass ? 9 : 0);
    }

    {
        cout << "Test E: insertKeyVal()" << endl;
        bool pass = true;
        int M = 10;
        SymbolTable st(M); // create the symbol table of size N
        bool success1 = st.insertKeyVal("foooooooooooo", 12);
        success1 = st.insertKeyVal("foooooooooooo", 14); // exact same key, new value
        if (success1 == true) { pass = false; cout << "insert failed because it should have returned false when overwriting a duplicate key" << endl; }
        bool success2 = st.insertKeyVal("fooooooooooooo", 12); // different key
        if (success2 == false) { pass = false; cout << "   insert failed" << endl; }
        if (st.isEmpty() == true) { pass = false; cout << "   isEmpty() failed " << endl; }
        if (st.getSize() != 2) { pass = false;cout << "   getSize() failed " << endl; }
        if (st.loadFactor() != 0.2f) { pass = false;  cout << "   loadFactor() failed " << endl; }
        if (st.testLinks() != true) { doubleLinksWork = false;  cout << "   NOTE: testLinks() failed." << endl; }
        cout << (pass ? "  passed (+9)" : " failed (+0)") << endl;
        totalPoints += (pass ? 9 : 0);
    }

    {
        cout << "Test F: constainsKey() " << endl;
        bool pass = true;
        int M = 10;
        SymbolTable st(M); // create the symbol table of size N
        bool success = st.insertKeyVal("foo", 12);
        if (success == false) { pass = false; cout << "   insert failed " << endl; }
        if (st.isEmpty() == true) { pass = false; cout << "   isEmpty() failed " << endl; }
        if (st.getSize() != 1) { pass = false;cout << "   getSize() failed " << endl; }
        if (st.loadFactor() != 0.1f) { pass = false;  cout << "   loadFactor() failed " << endl; }
        if (st.containsKey("foo") == false) { pass = false; cout << "   constainsKey() failed " << endl; }
        if (st.testLinks() != true) { doubleLinksWork = false;  cout << "   NOTE: testLinks() failed." << endl; }
        cout << (pass ? "  passed (+9)" : " failed (+0)") << endl;
        totalPoints += (pass ? 9 : 0);
    }

    {
        cout << "Test G: getValue()" << endl;
        bool pass = true;
        int M = 10;
        SymbolTable st(M); // create the symbol table of size N
        bool success = st.insertKeyVal("foo", 12);
        if (success == false) { pass = false; cout << "   insert failed" << endl; }
        if (st.isEmpty() == true) { pass = false; cout << "   isEmpty() failed " << endl; }
        if (st.getSize() != 1) { pass = false;cout << "   isEmpty() failed " << endl; }
        if (st.loadFactor() != 0.1f) { pass = false;  cout << "   loadFactor() failed " << endl; }
        if (st.containsKey("foo") == false) { pass = false; cout << "   constainsKey() failed to return true" << endl; }
        if (st.getValue("foo") != 12) { pass = false; cout << "   getValue() failed to return 12" << endl; }
        success = st.insertKeyVal("foo", 15);
        if (success == true) { pass = false;cout << "   insert should have returned false when overwriting an existing key" << endl; }
        if (st.isEmpty() == true) { pass = false;cout << "   isEmpty() failed " << endl; }
        if (st.getSize() != 1) {
            pass = false;
            cout << "Fail: insertKeyVal() may have failed to detect duplicate key..." << endl;
        }
        if (st.loadFactor() != 0.1f) { pass = false;  cout << "   loadFactor() failed " << endl; }
        if (st.containsKey("foo") == false) { pass = false; cout << "   constainsKey() failed to return true" << endl; }
        if (st.getValue("foo") != 15) {
            pass = false;
            cout << "Fail: insertKeyVal() may have failed to overwrite duplicate key's value with 15..." << endl;
        }
        if (st.testLinks() != true) { doubleLinksWork = false;  cout << "   NOTE: testLinks() failed." << endl; }
        cout << (pass ? "  passed (+9)" : " failed (+0)") << endl;
        totalPoints += (pass ? 9 : 0);
    }


    {
        cout << "Test H: deleteKey()" << endl;
        bool pass = true;
        int M = 10;
        SymbolTable st(M); // create the symbol table of size N
        bool success = st.insertKeyVal("foo", 12);
        if (success == false) { pass = false; cout << "   insert failed" << endl; }
        if (st.isEmpty() == true) { pass = false; cout << "   isEmpty() failed " << endl; }
        if (st.getSize() != 1) { pass = false;cout << "   getSize() failed " << endl; }
        if (st.loadFactor() != 0.1f) { pass = false;  cout << "   loadFactor() failed " << endl; }
        if (st.containsKey("foo") == false) { pass = false; }
        if (st.getValue("foo") != 12) { pass = false; }
        success = st.deleteKey("foo");
        if (success == false) { pass = false; }
        if (st.isEmpty() == false) { pass = false; }
        if (st.getSize() != 0) { pass = false; }
        if (st.loadFactor() != 0) { pass = false; }
        if (st.containsKey("foo") == true) { pass = false; }
        if (st.getValue("foo") != -1) { pass = false; }
        if (st.testLinks() != true) { doubleLinksWork = false;  cout << "   NOTE: testLinks() failed." << endl; }
        cout << (pass ? "  passed (+9)" : " failed (+0)") << endl;
        totalPoints += (pass ? 9 : 0);
    }

    {
        cout << "Test I:  deleteKey()" << endl;
        bool pass = true;
        int M = 10;
        SymbolTable st(M); // create the symbol table of size N
        bool success = st.insertKeyVal("foo", 12);
        if (success == false) { pass = false; cout << "   insertKeyVal() failed" << endl; }
        if (st.isEmpty() == true) { pass = false; cout << "   isEmpty() failed " << endl; }
        if (st.getSize() != 1) { pass = false;cout << "   isEmpty() failed " << endl; }
        if (st.loadFactor() != 0.1f) { pass = false;  cout << "   loadFactor() failed " << endl; }
        if (st.containsKey("foo") == false) { pass = false; cout << "   contains() failed " << endl; }
        if (st.getValue("foo") != 12) { pass = false; cout << "   getValue() failed to return 12 for foo" << endl; }
        success = st.deleteKey("foo");
        if (success == false) { pass = false; cout << "   deleteKey() failed to report true when deleting a known key" << endl; }
        if (st.isEmpty() == false) { pass = false; cout << "   isEmpty() incorrectly reported false after deleting the last key" << endl; }
        if (st.getSize() != 0) { pass = false;cout << "   getSize() incorrectly reported 1 after deleting hte last key" << endl; }
        if (st.loadFactor() != 0) { pass = false;  cout << "   loadFactor() after delete failed " << endl; }
        if (st.containsKey("foo") != false) { pass = false; cout << "   containsKey() incorrectly reported true after delete of final key " << endl; }
        if (st.getValue("foo") != -1) { pass = false; cout << "   getValue() failed to return -1 when checking a non-existent key " << endl; }
        success = st.deleteKey("foo");
        if (success == true) { pass = false; cout << "   deleteKey() failed because it reported true when deleting a non-existent key" << endl; }
        if (st.isEmpty() != true) { pass = false; cout << "   isEmpty() incorrectly reported true when the table was empty" << endl; }
        if (st.getSize() != 0) { pass = false;cout << "   isEmpty() failed to report a size of zero when the table should be empty " << endl; }
        if (st.loadFactor() != 0) { pass = false;  cout << "   loadFactor() failed " << endl; }
        if (st.containsKey("foo") == true) { pass = false; cout << "   containsKey() incorrectly reported that the key existed after it was deleted " << endl; }
        if (st.getValue("foo") != -1) { pass = false; cout << "   getValue() failed to return -1 when checking a non-existent key " << endl; }
        cout << (pass ? "  passed (+9)" : " failed (+0)") << endl;
        totalPoints += (pass ? 9 : 0);
    }

    {
        cout << "Test J: stress test" << endl;
        bool pass = true;
        int M = 10;
        SymbolTable st(M, true); // create the symbol table of size N

        bool debug = false;     // set to true to help debug if you prefer

        string words[] = {
            "advertisement",
            "teeny",
            "purring",
            "new",
            "rule",
            "black",
            "question",
            "deafening",
            "books",
            "trouble",
            "sweet",
            "dirty",
            "fish",
            "coil",
            "celery",
            "trousers",
            "rainy",
            "gamy",
            "weight",
            "month"
        };

        int myCount = 0;
        for (int i = 0; i < 200; i++) {
            if (debug) cout << "\n\n Test J loop# " << i << " of " << 200 << endl;
            if (debug) cout << "****************************************" << endl;
            int r = rand() % 20;
            string myWord = words[r]; // pick a random word

            if (rand() % 2 == 0) { // insert the word
                if (debug) cout << "  getting ready to insert " << myWord << endl;
                bool didContain = st.containsKey(myWord);
                bool success = st.insertKeyVal(myWord, rand() % 100); // insert a random key
                if (success) myCount++;
                if (didContain && success) {
                    if (debug) cout << "    failed: insertKeyVal() returned true when a key already existed" << endl;
                    pass = false;
                } else if (!didContain && !success) {
                    if (debug) cout << "    failed: insertKeyVal() failed to return true when inserting a key that did not exist" << endl;
                    pass = false;
                }
            } else {  // delete the word
                if (debug) cout << "  getting ready to delete " << myWord << endl;
                bool didContain = st.containsKey(myWord);
                bool success = st.deleteKey(myWord); // insert a random key
                if (success) myCount--;
                if (didContain && !success) {
                    if (debug) cout << "    failed: deleteKey() returned false when deleting a key that already existed" << endl;
                    pass = false;
                } else if (!didContain && success) {
                    if (debug) cout << "    failed: deleteKey() returned true when deleting a key that did not exist" << endl;
                    pass = false;
                }
            }
        }

        if (st.getSize() != myCount) {
            cout << "   failed: getSize() return after stress test did not match local count of total keys in table when done." << endl;
        }

        if (st.testLinks() != true) { doubleLinksWork = false;  cout << "   NOTE: testLinks() failed." << endl; }

        st.printMe();


        cout << (pass ? "  passed (+9)" : " failed (+0)") << endl;
        totalPoints += (pass ? 9 : 0);

    }

    {
        cout << "Test K: insertKeyVal() with collisions" << endl;
        bool pass = false;
        int M = 10;
        SymbolTable st(M, false); // create the symbol table of size N
        bool success = st.insertKeyVal("one", 1);
        success &= st.insertKeyVal("two", 2);
        success &= st.insertKeyVal("three", 3);
        success &= st.insertKeyVal("four", 4);
        success &= st.insertKeyVal("five", 5);
        if (success == false) { pass = false; cout << "   insert failed" << endl; }
        if (st.isEmpty() == true) { pass = false; cout << "   isEmpty() failed " << endl; }
        if (st.getSize() != 5) { pass = false;cout << "   getSize() failed " << endl; }
        if (st.loadFactor() != 0.4f) { pass = false;  cout << "   loadFactor() failed " << endl; }

        if (st.containsKey("one") != true) { pass = false;  cout << "   containsKey(\"one\") failed " << endl; }
        if (st.containsKey("two") != true) { pass = false;  cout << "   containsKey(\"two\") failed " << endl; }
        if (st.containsKey("three") != true) { pass = false;  cout << "   containsKey(\"three\") failed " << endl; }
        if (st.containsKey("four") != true) { pass = false;  cout << "   containsKey(\"four\") failed " << endl; }
        if (st.containsKey("five") != true) { pass = false;  cout << "   containsKey(\"five\") failed. (It should be in bucket #6) " << endl; }
        if (st.testLinks() != true) { doubleLinksWork = false;  cout << "   NOTE: testLinks() failed." << endl; }
        cout << (pass ? "  passed (+9)" : " failed (+0)") << endl;
        totalPoints += (pass ? 9 : 0);
    } // end test k

    cout << "Testing Double-Linked List(next and prev): " << endl;
    if (doubleLinksWork != true) {
        cout << "    FAILED: double-linked list does not appear to work fully (+0)." << endl;
    } else {
        cout << "    SUCCESS: double-linked list passed (+10) " << endl;
        totalPoints += 10;
    }


    cout << "Your preliminary score is " << totalPoints << endl << endl;

    int i;
    cout << "Enter 1 and return to finish test";
    cin >> i;
} // end testMe


//
// MAIN
//


int main() {
    std::cout << "Symbol Table Homework 5\n";

    testMe();         // uncomment when you are ready to self-test

    cout << "Instructions:\nTo insert: Enter a key string and then an integer value, separated by a space, then hit return.\nTo Delete: Enter a key and the letter x, separated by a space, then hit return.\nTo Get a value: Enter a key and the letter g, separated by a space, then hit return." << endl;

    string key; // key string
    string sval; // value string or x to delete the specified key

    int val; // integer equivalent of sval

    int M = 10;

    cout << "Symbol Table size M (ex: 10) ? ";
    cin >> M;


    SymbolTable st(M); // create the symbol table of size N

    while (true) {
        cout << "key? "; cin >> key;
        cout << "val (or x or g)? "; cin >> sval;

        if (sval == "x") {
            st.deleteKey(key); // delete the key
        } else if (sval == "g") {
            cout << "The value for key " << key << " = " << st.getValue(key) << endl;; // delete the key
        } else {
            val = atoi(sval.c_str()); // convert string sval to int val
            st.insertKeyVal(key, val); // insert the key
        }

        st.printMe();
    }
}
