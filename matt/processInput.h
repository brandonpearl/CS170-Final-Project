#include <iostream>
#include <fstream>
#include <set>
using namespace std;

/* Adjacency Matrix structure
 * - Size: returns the number of nodes in the graph
 * - Matrix: if matrix[i][j] is 1, node i has an edge to node j
 *           if matrix[i][j] is 0, no edge exists between i and j
 */
struct adjMatrix {
    int size;
    int** matrix;
} ;

/* Adjanceny List structure
 * - Size: returns the number of nodes in the graph
 * - List: list[i] will return a set containing all nodes i has an 
 *         edge to. The nodes will be represented by their int values.
 *         To check if edge (i,j) exists, use:
 *              <adjListName>.edgeExists(i,j)
 *         Which will evaluate to TRUE if edge (i,j) exists
 */
struct adjList {
    int size;
    set<int>* list;

    bool edgeExists(int i, int j) {
        return list[i].find(j) != list[i].end();
    }
} ;

adjMatrix adjM;
adjList adjL;

int processInput(int argc, char *argv[]) {
    // Make sure we've properly formatted our input
    if (argc != 2) {
        cout<<"Usage: "<< argv[0] <<" <filename>\n";
        return 0;
    } else {

        // Attempt to open the file
        ifstream inFile;
        inFile.open(argv[1]);
        if (!inFile) {
            cout<<"Could not find file"<<endl;
        }

        // Start parsing the file, assuming its properly formatted
        // We begin by getting the number of nodes (between 0 and 100)
        char line[3];
        int size;
        inFile.getline(line, 3);
        size = atoi(line);

        // Intialize our Adjancency Matrix
        adjM.size = size;
        adjM.matrix = new int*[size];

        // Initialize our Adjacency List
        adjL.size = size;
        adjL.list = new set<int>[size];

        // We then construct our adjacency matrix and list
        char ch;
        for (int i=0; i<size; i++) {
            adjM.matrix[i] = new int[size];
            for (int j=0; j<size; j++) {
                inFile.get(ch);

                // Insert into adjacency matrix
                int e = ch - '0';
                adjM.matrix[i][j] = e;

                // Insert into adjacency list if edge exists
                if (e == 1) {
                    adjL.list[i].insert(j);
                }
                
                inFile.get(ch);
            }
        }

        return 0;
    }
}

adjMatrix getAdjMatrix() {
    return adjM;
}

adjList getAdjList() {
    return adjL;
}


