#include <iostream>
#include <cassert>
#include <queue>
#include <limits>

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
    int p_edgeCount = 0;    // count of total edges in the graph (p for private)
    int p_vertexCount = 0;  // count of total vertices in the graph (nodes) (p for private)

public:
    Graph(int N) {
        adj = new Node * [N];  // allocate an array of Node pointers
        p_vertexCount = N;   // N unconnected vertices (p stands for private, because this data member is private)
    }

    // Constructor #1: makes a graph from a given value of N vertices, with P pairs of vertices (edges), and an array with pairs of edge ids
    Graph(int N, int P, int edges[]) { // use P pairs of ints to build edges
      // WARNING: edges[] must have exactly P pairs of ints that is 2*P

        adj = new Node * [N]; // allocate an array of N Node pointers - these will be linked lists of adjacent vertices
        p_vertexCount = N; // N unconnected vertices 

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

    void addEdge(int va, int vb) { // add an edge to the graph

        if (va < 0 || vb < 0 || va >= vertexCount() || vb >= vertexCount()) {
            cout << "AddEdge: edge " << va << ", " << vb << " is out of bounds 0, " << vertexCount() - 1 << ". Abort." << endl;
            return;
        }

        if (va == vb) {
            cout << "AddEdge: edge " << va << ", " << vb << "is a self-referencing loop. Abort." << endl;
            return;
        }

        addAdj(va, vb); // add Adjacency
        addAdj(vb, va); // add the reverse Adjacency
        p_edgeCount++;
    }

    //
    // ADD ADJACENCY
    //

    void addAdj(int va, int vb) { // add vertex b as adjacent to vertex a

        if (va < 0 || vb < 0 || va >= vertexCount() || vb >= vertexCount()) {
            cout << "addAdj: edge " << va << ", " << vb << " is out of bounds 0, " << vertexCount() - 1 << ". Abort." << endl;
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
                cout << "addAdj(): duplicate adjacency " << vb << " already exists for " << va << ". Ignorning add for " << vb << endl;
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
    // GET ADJACENCY - what vertices are neighbors of vertex with id vid?
    //

    Node* getAdjVertices(int vid) {
        bool debug = true;
        // return a linked list from the adjacency array with ids of all vertices which are adjacent to vertex with id #vid

        if (debug) cout << "getAdjVertices() find vertices adjacent to " << vid << ": ";

        if (vid < 0 || vid >= vertexCount()) {
            if (debug) cout << "getAdjVertices: vertex " << vid << " is out of bounds 0, " << vertexCount() - 1 << ". Abort." << endl;
            return nullptr; // return nullptr
        }

        Node* tv = adj[vid]; // temp vertex in list if adjacencies for va
        while (tv != nullptr) {
            cout << tv->id << ", ";
            tv = tv->next;
        }
        if (debug) cout << endl;
        return adj[vid]; // return the adjacency list

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
            return -1; // return impossible degree
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
            cout << i << ": ";                    // print the vertex id that owns this list
            while (temp != nullptr) {                // while our temp node pointer is not nullptr
                cout << temp->id << ", ";           // print the id member of this temp node
                temp = temp->next;                  // advance temp to the next member of the adjacency list
            }
            cout << endl;
        }
    }

}; // end Graph



//
//  CONNECTED COMPONENT
//  a special data structure to tell us if ANY two vertices in a graph are connected
//

class ConnectedComponent {
public:
    int N = 0; // size of arrays, which == the number of verts in the graph
    bool* wasVisited; // array of bools for each vertex we have visited from 
    int componentCount = 0; // the number of componets we have discovered, if any
    int* compId; // array of component ids for each vertex, indicating the component is resides in (ids go from zero to ?)


    ConnectedComponent(Graph* g, bool debug = false) { // constructor of our DFP object
        N = g->vertexCount();
        wasVisited = new bool[N]; // allocate dynamic array wasVisited[]
        compId = new int[N]; // allocate dynamic array compId[]
        init(); // initialize member variables

        for (int vid = 0; vid < N; vid++) {
            if (!wasVisited[vid]) {
                dfsCC(g, vid); // do a dfs() for each unvisted vertex
                componentCount++; // increment the component count
            }
        }
    }

    ~ConnectedComponent() { // destructor
        delete[] compId;
    }

    //
    // INIT - initialize member variables before we build components
    //

    void init() {
        componentCount = 0;
        for (int i = 0; i < N; i++) { // init our new arrays before we search
            wasVisited[i] = false; // not visited yet
            compId[i] = -1; // not in any component yet
        }
    }

    //
    // DEPTH FIRST SEARCH
    //

    void dfsCC(Graph* g, int vid) { // Depth first search method

        cout << "dfs(): checking components for vid = " << vid << endl;
        if (vid < 0 || vid >= N) {
            cout << "dfsCC(): vertex " << vid << " is out of bounds 0, " << N - 1 << ". Abort." << endl;
        }

        // TODO: your code goes here
    }


    //
    // IS CONNECTED COMPONENT
    // determine if vertex a and b are part of the same connected COMPONENT
    //

    bool isConnectedComponent(int va, int vb) {

        if (va < 0 || vb < 0 || va >= N || vb >= N) {
            cout << "isConnectedComponent: edge " << va << ", " << vb << " is out of bounds 0, " << N - 1 << ". Abort." << endl;
            return false;
        }

        // TODO: your implementation goes here

    }

    //
    // GET COMPONENT ID - get the int id of the component to which vertex belongs
    //

    int getComponentId(int vid) {
        if (vid < 0 || vid >= N) {
            cout << "getComponentId(): vertex " << vid << " is out of bounds 0, " << N - 1 << ". Abort." << endl;
            return -1;
        }
        // TODO: your implementation goes here
    }

    void printComponents() {
        cout << "Components:" << endl;
        cout << "-------------" << endl;

        for (int x = 0; x < N; x++) {
            cout << x << " : " << compId[x] << endl;
        }
    }

private:

}; // end ConnectedComponent class

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

} // end getIntegerInput()

//
// MAIN
//

int main() {
    std::cout << "Undirected Graphs - Connected Components (CS220 HW7) \n";

    int N = 5;   // 5 vertices
    cout << " How many nodes do you want in your graph(ex: 10)? ";
    cin >> N;
    cout << "NOTE: All vertex IDs must be from zero to " << (N - 1) << " (inclusive)" << endl;
    Graph g(N);  // build a graph that can handle N vertices, but has no initial connections

    cout << "Enter pairs of ids, or any negative pair of values when done entering pairs" << endl;


    // interactive mode below


    while (true) { // loop forever
        int x, y;
        cout << "\nEnter 2 vertex IDs (an edge pair), each between 0 and " << (N - 1) << " (separated by a space, then return): ";
        x = getIntegerInput(); // get an integer from input until success
        y = getIntegerInput(); // get an integer from input until success

        if (x >= 0 && y >= 0)
            g.addEdge(x, y);    // add edge from vertex x to vertex y (x becomes adjacent to y, and y becomes adjacent to x)
        else
            break;               // break out of loop so we can perform path searching
        g.printAdj();         // print out adjacencies
    } // end while()


    int vRoot = -1;  // this will be the vertex id we want to study


    cout << "Building Connected Component Object " << endl;

    ConnectedComponent cc(&g);
    cc.printComponents();

    while (true) { // loop forever
        int x, y;
        cout << "\nEnter 2 vertex IDs each between 0 and " << (N - 1) << " to see if they are in the same component: ";
        x = getIntegerInput(); // get an integer from input until success
        y = getIntegerInput(); // get an integer from input until success

        if (x >= 0 && y >= 0) {
            if (cc.isConnectedComponent(x, y))
                cout << "Yes, " << x << " and " << y << " are in the same component " << endl;
            else
                cout << "No, " << x << " and " << y << " are NOT the same component " << endl;
        }     else
            cout << "Sorry - one of those ids is out of range.  Please enter a value between 0 and " << (N - 1) << endl;
        //g.printAdj();         // print out adjacencies
    } // end while()
}