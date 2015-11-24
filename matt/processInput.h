#include <iostream>
#include <fstream>
#include <set>
using namespace std;

/* Adjacency Matrix structure
 * - Constructor: to make a new instance called foo, use:
 *      AdjMatrix foo (filename);
 * - getSize(): returns the number of nodes in the graph
 * - edgeExists(i,j): returns TRUE if edge (i,j) exists, FALSE otherwise
 */
class AdjMatrix {
    int size;
    int** matrix;

  public:
    AdjMatrix(char* filename) {
        // Attempt to open the file
        ifstream inFile;
        inFile.open(filename);
        if (!inFile) {
            cout<<"Could not find file"<<endl;
        }
    
        // Start parsing the file, assuming its properly formatted
        // We begin by getting the number of nodes (between 0 and 100)
        char line[3];
        inFile.getline(line, 3);
    
        size = atoi(line);
        matrix = new int*[size];
    
        // We then construct our adjacency matrix
        char ch;
        for (int i=0; i<size; i++) {
            matrix[i] = new int[size];
            for (int j=0; j<size; j++) {
                inFile.get(ch);
    
                // Insert into adjacency matrix
                int e = ch - '0';
                matrix[i][j] = e;
    
                inFile.get(ch);
            }
        }
    }   

    int getSize(){
        return size;
    }

    bool edgeExists(int i, int j){
        return matrix[i][j] == 1;
    }
} ;




/* Adjanceny List structure
 * - Constructor: to make a new instance called foo, use:
 *      AdjList foo (filename);
 * - getSize(): returns the number of nodes in the graph
 * - edgeExists(i, j): returns TRUE if edge (i,j) exists, FALSE otherwise
 * - allEdges(i): returns a set<int> of all edges coming from node i
 */
class AdjList {
    int size;
    set<int>* list;

  public:
    AdjList (int s){
        size = s;
        list = new set<int>[size];
        for (int i=0; i<size; i++) {
            for (int j=0; j<size; j++) {
                int num = rand() % 2;
                list[i].insert(j);
            }
        }
    }
    AdjList (char* filename){
        // Attempt to open the file
        ifstream inFile;
        inFile.open(filename);
        if (!inFile) {
            cout<<"Could not find file"<<endl;
        }
    
        // Start parsing the file, assuming its properly formatted
        // We begin by getting the number of nodes (between 0 and 100)
        char line[3];
        inFile.getline(line, 3);
    
        size = atoi(line);
        list = new set<int>[size];
    
        // We then construct our adjacency list
        char ch;
        for (int i=0; i<size; i++) {
            for (int j=0; j<size; j++) {
                inFile.get(ch);
    
                int e = ch - '0';
                if (e == 1) {
                    list[i].insert(j);
                }
                
                inFile.get(ch);
            }
        }
    }


    int getSize() {
        return size;
    }

    bool edgeExists(int i, int j) {
        return list[i].find(j) != list[i].end();
    }

    set<int> allEdges(int i) {
        set<int> copy (list[i]);
        return copy;
    }
} ;


