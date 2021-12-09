#include <iostream>
#include <cassert>
#include <queue>
#include <limits>
#include <stack>

#include <fstream> // for file io
#include <cstdlib> // for random




using namespace std;

class Node {
public:
    int id = -1; // id of our Vertex; -1 is illegal - reset by constructor
    Node* next = nullptr; // next vertex in the list

    Node(int _id) { // constructor
        id = _id;
        next = nullptr;
    }
};

class Graph {

private:
    Node** adj;             // array of Node pointers: one linked list for each vertex 0 to N-1
    int p_edgeCount = 0;    // count of total edges in the graph
    int p_vertexCount = 0;  // count of total vertices in the graph (nodes)
    bool isDirected = false; // true if this is a directed graph (a DIGRAPH)
      // NOTE: used by various methods in this class when edges go in only one direction

public:
    Graph(int N, bool _isDirected = false) {
        adj = new Node * [N];  // allocate an array of Node pointers

        p_vertexCount = N;   // N unconnected vertices
        isDirected = _isDirected; // assign the default value or whatever is pass in to the constructor
        init();       // init member arrays adj[] and onStack[]
    }

    Graph(int N, int P, int edges[], bool _isDirected = false) { // use P pairs of ints to build edges
      // WARNING: edges[] must have exactly P pairs of ints that is 2*P

        adj = new Node * [N]; // allocate an array of N Node pointers - these will be linked lists of adjacent vertices
        p_vertexCount = N; // N unconnected vertices
        isDirected = _isDirected; // assign the default value or whatever is pass in to the constructor
        init(); // init member arrays adj[] and onStack[]
        for (int i = 0; i < P; i = i + 2) { // loop through P pairs of edge descriptions
          // edge[i], edge[i+1] represents a single edge
            addEdge(edges[i], edges[i + 1]);
        }
    }

    ~Graph() { // our destructor
        for (int i = 0; i < p_vertexCount; i++)
            for (Node* t = adj[i]; t != nullptr; ) { // loop through the linked adj list for this vertex
                Node* t2 = t; // temp pointer so we can delete
                t = t->next; // advance to next item before we delete
                delete(t2); // now we can delete t2
            }
        delete[] adj; // delete dynamic array created in constructor
    }

    void init() {
        for (int i = 0; i < p_vertexCount; i++) {
            adj[i] = nullptr;
        }
    }

    //
    // ADD EDGE
    //

    void addEdge(int va, int vb) { // add an edge to the graph

        if (va < 0 || vb < 0 || va >= vertexCount() || vb >= vertexCount()) {
            //cout << "addEdge: edge " << va << ", " << vb << " is out of bounds 0, " << vertexCount()-1 << ". Abort." << endl;
            return;
        }

        if (va == vb) {
            //cout << "addEdge: edge " << va << ", " << vb << " -referencing loop. Abort." << endl;
            return;
        }

        addAdj(va, vb); // add Adjacency from va to vb
        if (!isDirected) addAdj(vb, va); // add the reverse Adjacency only if the graph is directed
        p_edgeCount++;
    }


    //
    // ADD ADJACENCY
    //

    void addAdj(int va, int vb) { // add vertex b as adjacent to vertex a

        if (va < 0 || vb < 0 || va >= vertexCount() || vb >= vertexCount()) {
            //cout << "addAdj: edge " << va << ", " << vb << " is out of bounds 0, " << vertexCount()-1 << ". Abort." << endl;
            return;
        }

        // Node *aPtr = new Node(va); // create a new node for a; assign pointer
        Node* bPtr = new Node(vb); // create a new node b; assign the pointer

        if (adj[va] == nullptr) {
            adj[va] = bPtr; // first neighbor of va
            return;
        }

        Node* temp = adj[va]; // head of our linked list

        // search the entire list to be sure there is not a duplicate of vb
        while (temp != nullptr) {
            if (temp->id == vb) { // vb is already listed as a neighbor of va, so return
              //cout << "addAdj(): duplicate adjacency " << vb << " already exists for " << va << ". Ignorning add for " << vb << endl;
                return;
            }
            if (temp->next == nullptr) { // found the end
                temp->next = bPtr; // add bPtr at end of list
                return;
            }
            temp = temp->next; // else advance to the next vertex in list of neighbors for va
        }
    } // end addAdj()


    //
    // DELETE ADJACENCY  (TODO): you must write this function
    //

    bool delAdj(int va, int vb) { // delete the adjacency vb, (if it exists ,return true, else false)

        cout << "delAdj(): removing adjacency from " << va << " to " << vb << "(if it exists)" << endl;

        // TODO: YOUR CODE GOES HERE

        return false; // Assume false, unless we have return true or false above

    } // end delAdj()

    //
    // GET ADJACENCY - what vertices are neighbors of vertex with id vid?
    //

    Node* getAdjVertices(int vid) {
        bool debug = false;
        // return a vector with ids of all vertices which are adjacent to vertex a

        if (debug) cout << "getAdjVertices() find vertices adjacent to " << vid << ": ";

        if (vid < 0 || vid >= vertexCount()) {
            if (debug) cout << "getAdjVertices: vertex " << vid << " is out of bounds 0, " << vertexCount() - 1 << ". Abort." << endl;
            return {}; // return an empty vector
        }

        Node* tv = adj[vid]; // temp vertex in list if adjacencies for va

        if (debug) { // print the adjacencies if we are debugging
            while (tv != nullptr) {
                cout << tv->id << ", ";
                tv = tv->next;
            }
            cout << endl;
        }

        return adj[vid]; // return the adjancency list

    } // end getAdjVertices()

    //
    // Get number of edges in the graph (includes duplicates, if any)
    //

    int edgeCount() {
        return p_edgeCount; // private edge count value
    }

    //
    // Get number of vertices in the graph
    //

    int vertexCount() {
        return p_vertexCount; // private vertex count value
    }

    //
    // Get degree of a particular vertex v in the graph (how many neighbors it has)
    //

    int degree(int vid) {

        if (vid < 0 || vid >= vertexCount()) {
            cout << "degree: vertex " << vid << " is out of bounds 0, " << vertexCount() - 1 << ". Abort." << endl;
            return {}; // return an empty vector
        }

        int count = 0;
        Node* t = adj[vid];

        while (t != nullptr) {
            count++; // count how many vertices are neighbors of vertex vid
            t = t->next;
        }
        return count;
    }


    //
    // Print all adjacencies in the graph
    //

    void printAdj() {
        cout << "\nAdjacencies: All Vertices and their respective adj lists:" << endl;

        for (int i = 0; i < vertexCount(); i++) {
            Node* temp = adj[i];                  // get a temp node pointer to the head of the adjacency list for vertex i
            cout << i << "-> ";                    // print the vertex id that owns this list
            while (temp != nullptr) {                // while our temp node pointer is not nullptr
                cout << temp->id << ", ";           // print the id member of this temp node
                temp = temp->next;                  // advance temp to the next member of the adjacency list
            }
            cout << endl;
        }
    }

}; // end Graph

//
// DIRECTED CYCLE - a class to determine if an instance of a Graph class has any cycles or not
//

class DirectedCycle {
private:
    bool* wasVisited;        // array of bools for each vertex we have visited from 
    int* fromVertex;         // array of vertex ids that lead to each vert (form edges)
    stack<int> cycleStack;   // a working stack we can use to determine if this Graph has any cycles
    bool* onStack;           // array of booleans that indicate if a specific vertex is on the stack or not

public:
    DirectedCycle(Graph* g) {  // constructor

        wasVisited = new bool[g->vertexCount()];  // allocate dynamic array wasVisited[]
        fromVertex = new int[g->vertexCount()];   // allocate dynamic array fromVertex[]
        onStack = new bool[g->vertexCount()];    // allocate an array of bools to indicate if a vertex was on the stack or not

        for (int i = 0; i < g->vertexCount(); i++) { // init our new arrays before we search
            wasVisited[i] = false; // not wasVisited yet
            fromVertex[i] = -1; // -1 means "no edge to vertex i"
            onStack[i] = false; // vertex i is not on the stack yet
        }

        for (int i = 0; i < g->vertexCount(); i++) {
            if (!wasVisited[i]) dfs(g, i);
        }
    } // end constructor

    //
    // DEPTH FIRST SEARCH
    //

    void dfs(Graph* g, int vid) {

        if (vid < 0 || vid >= g->vertexCount()) {
            cout << "dfs(): vertex " << vid << " is out of bounds 0, " << g->vertexCount() - 1 << ". Abort." << endl;
        }

        onStack[vid] = true;                                 // we'll put this vertex on the stack
        wasVisited[vid] = true;                              // indicate that this vertex was "visited"
        Node* vList = g->getAdjVertices(vid);              // use G to get a linked list of all neighbors of "vid"

        for (Node* tv = vList; tv != nullptr; tv = tv->next) {   // for each neighbor of tv

            if (this->hasCycle())
                return;                     // return if the stack is not empty, because we have a cycle

            else if (!wasVisited[tv->id]) {      // if not visited, recurse through this child of vid
                fromVertex[tv->id] = vid;          // store the path we took to get to v[i] (from vid])
                dfs(g, tv->id);                    // recurse: search starting from the vertex tv->id
            }

            else if (onStack[tv->id]) {                       // else if visited and tv->id is on the stack
                for (int x = vid; x != tv->id; x = fromVertex[x])   // search backwards through "from" path
                    cycleStack.push(x);                          // push x onto the cycle stack to indicate the order it was visited

                cycleStack.push(tv->id);                       // push the last vertex in the cycle onto the cycle stack
                cycleStack.push(vid);                          // push the parent vid vertex onto the cycle stack
            }

        } // done processing all neighbors of vid; unwind recursion from here

        onStack[vid] = false;

    } // now wasVisited[] and fromVertex[] have a path from rootVid to vid, if one exists

    //
    // HAS CYCLE
    //

    bool hasCycle() {
        return (!cycleStack.empty()); // return true if the stack is not empty (has cycles) else false
    }

    //
    // print CYCLE
    //

    void printCycle() {
        stack<int> tstack = cycleStack;

        cout << "Cycle consists of the following vertices: ";
        while (!tstack.empty()) {
            int vid = tstack.top();
            tstack.pop();
            cout << vid << ", ";
        }
        cout << endl;
    }

    //
    // GET CYCLE - returns a vector (array) of integers that represent a directed cycle, where the first and last vertex are the same
    // the last directed edge will be represented by the 2nd-to-last vertex and the last vertex in the list
    //

    vector<int> getCycle() {
        vector<int> vlist; // our vector list that will be returned 
        stack<int> tstack = cycleStack; // copy so we can deconstruct it without affecting cycleStack eata member

        while (!tstack.empty()) {
            vlist.push_back(tstack.top());
            tstack.pop();
        }
        return vlist; // return a complete copy of the vector
    }

}; // end DirectedCycle class



//
// DEPTH FIRST ORDER - 
// return a reversePostOrder traversal of the graph g, starting at vertex 0 (used for topo sort)
//

class DepthFirstOrder {
public:
    bool* wasVisited;              // array of bools for each vertex we have visited from 
    stack<int> reversePostStack;   // output: a stack of vertices in reverse post order (for topological sort)

    DepthFirstOrder(Graph* g) {
        //cout << "DepthFirstOrder() Constrctor here..." << endl;
        wasVisited = new bool[g->vertexCount()];  // allocate dynamic array wasVisited[]
        for (int i = 0; i < g->vertexCount(); i++)     // init our new arrays before we search
            wasVisited[i] = false;                  // not wasVisited yet

        for (int vid = 0; vid < g->vertexCount(); vid++)
            if (!wasVisited[vid])                     // TODO: only recurse on vid in g if vid was never visited
                dfs(g, vid);
    }

    //
    // DEPTH FIRST SEARCH = builds a stack to be used by the Topologcal Class
    //

    void dfs(Graph* g, int vid) { // Traverse the graph from vertex vid in all possible directions that have not been seen before

        wasVisited[vid] = true;                              // indicate that this vertex was "visited"
        Node* vList = g->getAdjVertices(vid);              // use G to get a linked list of all neighbors of "vid"

        for (Node* tv = vList; tv != nullptr; tv = tv->next)    // for each neighbor of vid
            if (!wasVisited[tv->id])
                dfs(g, tv->id);                               // TODO: recurse: search starting from the vertex tv->id, if not visited

        reversePostStack.push(vid);       // TODO: push vid for reverse post requests (topological sort) onto the stack
        //cout << "PO:" << vid << ", ";
    }

    stack<int> reversePostOrder() {      // return a copy of the stack when requested
        return reversePostStack;
    }

    //
    //  PRINT TOPO ORDER
    //

    void printTopoOrder() { // print the contents of the stack so we can see what topo order it contains (top down)

        stack<int> copyStack = reversePostStack; // TODO: make a copy of the reversePostStack member of df so we can unpack it without changing the original

        cout << "The Topological order is: " << endl;
        while (!copyStack.empty()) {
            cout << copyStack.top() << ", ";  // print the top
            copyStack.pop();   // TODO: pop the top of the stack
        }
        cout << endl;
    } // end printTopoOrder()  

}; // end DepthFirstOrder


Graph* readDiGraph(string fname) {

    int N;
    int E;

    fstream myFile;
    myFile.open(fname, fstream::in);

    if (!myFile) {
        cout << "readDiGraph() Sorry I could not open file '" << fname << "'. Abort." << endl;
        return nullptr;
    }

    myFile >> N;
    myFile >> E;

    bool isDirected = true;

    Graph* g = new Graph(N, isDirected);

    int va, vb;

    while (myFile >> va >> vb) {
        //for( int ecount=0; ecount<E; ecount++ ){
            //myFile >> va >> vb;
        g->addEdge(va, vb);
    }

    myFile.close();

    g->printAdj();

    return g;
} // end readDiGraph()


//
// TEST delAdj  - this function can be placed below your removeCycles() function and can be called from main like this:
//   testDelAdj(); 
//

void testDelAdj() {

    int V = 10; // increase this number if you want a bigger test
    int E = V * V * V; // this will create a very dense graph with many edges (all created in random order)
    Graph* g = new Graph(V, true); // directed graph

    // build a random graph
    for (int i = 0; i < E; i++) {
        int va = rand() % V;
        int vb = rand() % V;
        g->addEdge(va, vb);
    }

    cout << "testDelAdj(): Creating random graph with " << g->vertexCount() << " vertices and " << g->edgeCount() << " edges " << endl;
    g->printAdj();

    cout << "\ntestDelAdj(): Start deleting graph, one random adjacency at a time...." << endl;
    for (int va = 0; va < g->vertexCount(); va++) { // for each adjacency slot in the adj[] array
        while (g->degree(va) > 0) { // as long as adjacencies exist for this va...
            int vb = rand() % V; // pick a random vb adjacency to delete based on the degree of va.. .it may or may not exit
            int degreeBeforeDelete = g->degree(va);

            if (g->delAdj(va, vb)) { // did it delete ok? (that is, did it exist?)
                cout << "testDelAdj(): Deleting random adjacency " << va << ", " << vb << endl;
                cout << "testDelAdj(): The degree of adj[" << va << "] BEFORE delete is " << degreeBeforeDelete << endl;
                cout << "testDelAdj(): Deleted Adjacency " << va << ", " << vb << "(" << g->degree(va) << " adjacencies are left)" << endl;
                int degreeAfterDelete = g->degree(va);
                cout << "testDelAdj(): The degree of adj[" << va << "] after delete is " << degreeAfterDelete << endl;
                if (degreeBeforeDelete != degreeAfterDelete + 1) { // OOPS this means something bad happened
                    g->printAdj(); // print out the adjacencies
                    cout << "testDelAdj(): ERROR: the degree of adj[" << va << "] after delete is not correct.. it should only be 1 less than before the delete.  But that is not the case.  Aborting program!" << endl;
                    exit(1); // we would never do this in "REAL LIFE", but this will stop your program immediately so you can debug
                }
            } else { // no it did not exist, pause so user can assess
               // g->printAdj(); // print out the adjacencies      // uncomment if you want to see adjancecies after each valid delete
              //int x; cout << "Enter 1 to continue: "; cin >> x;  // uncomment if you want to pause after each valid delete
            } // end else
        } // end while
    } // end 

    cout << "\n\n\n *** testDelAdj():  FINISHED OK - GREAT JOB on delAdj()!! \n\n\n";
    int x; cout << "Enter 1 to continue: "; cin >> x;
} // end testDelAdj()


//
// REMOVE CYCLES 
//
void removeCycles(Graph* g) {

    // TODO: Using graph g, create an instance of the DirectedCycle class, so we can test for cycles
    // TODO: return if the DirectedCycle class has not found any cycles

    // TODO: Via a Loop: 
      // TODO: else, get a cycle from the DirectedCycle class in the form of a vector of integer values (vertex id's for the cycle)
      // TODO: Using the returned the vector (like an array) determine the first and second vertices of the cycle, at position zero and one of the vector
      // TODO: remove directed edge from the graph for the 1st edge found in the cycle, using the new delAdj() method of the Graph class
      // TODO: repeat the above loop steps until the grahp has no more cycles

    // TODO: print the adjacency list for the graph for debuging purposes, and return (void)

}


int main() {

    std::cout << "HW 7 - Remove DiGraph Cycles, then Topo Sort " << endl;
    string fname; // our input file name

    cout << "Which file would you like to test? ";
    cin >> fname;
    Graph* gPtr = readDiGraph(fname); // read in a graph from the specified file

    if (gPtr == nullptr) {
        cout << "That file could not be read. Abort." << endl; // read the graph back in
        exit(1);
    }

    cout << "main(): Removing Cycles... " << endl;

    // TODO: call your removeCycles() function using the Graph pointer g


 // DONE FOR YOU: VERYFIY THAT THE GRAPH HAS NO CYCLES 

    DirectedCycle dc(gPtr); // Create an instance of our this class, which will help us find cycles

    if (dc.hasCycle()) {
        cout << "This graph is not a DAG" << endl;
        cout << "... because I found least one cycle: " << endl;
        dc.printCycle();
        cout << "Please review your removeCycles() function call to ensure it is working properly. Abort" << endl;
        exit(1);
    } else
        cout << "This graph does not have any cycles... so it IS a DAG (directed acyclic graph)" << endl;

    // DONE FOR YOU ABOVE


    // TODO: you must finish this below

    cout << "\nReady to perform topological sort" << endl;

    // TODO: Create an instance of the DepthFirstOrder class, which will fill the stack with the topo order
    // TODO: print the resulting topological order using the DepthFirstOrder instance you created (call its method)

    cout << "\nDONE!" << endl;
}