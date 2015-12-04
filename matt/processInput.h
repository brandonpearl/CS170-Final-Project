#ifndef _io
#define _io
#include <iostream>
#include <assert.h>
#include <stdio.h>
#endif
#include <fstream>
#include <set>
using namespace std;

/* Adjacency Matrix structure
 * - Constructor: to make a new instance called foo, use:
 *     AdjMatrix foo (filename);
 * - getSize(): returns the number of nodes in the graph
 * - edgeExists(i,j): returns TRUE if edge (i,j) exists, FALSE otherwise
 * - countIn(int i, set<int> s): number of incoming edges to node i ignoring
 *     ones from nodes in array a
 * - countOut(int i, set<int> s): number of outgoing edges from node i ignoring
 *     ones going to nodes in array a 
 */
class AdjMatrix {
    int size;
    int** matrix;

  public:
    /*
     * Used in duplicate() method.
     */
    AdjMatrix() {
        size = 0;
    }

    AdjMatrix(char* filename) {
        // Attempt to open the file
        ifstream inFile;
        inFile.open(filename);
        if (!inFile) {
            cout<<"Could not find file"<<endl;
        }
    
        // Start parsing the file, assuming its properly formatted
        // We begin by getting the number of nodes (between 0 and 100)
        char line[4];
        inFile.getline(line, 4);
    
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
        inFile.close();
    }   

    /*
     * Creates duplicate of matrix to prevent contamination. Used in topological sort.
     */
    AdjMatrix duplicate() {
        AdjMatrix retVal = AdjMatrix();
        retVal.size = size;
        retVal.matrix = new int*[size];
        for (int i=0; i<size; i++) {
            retVal.matrix[i] = new int[size];
            for (int j=0; j<size; j++) {
                retVal.matrix[i][j] = matrix[i][j];
            }
        }
        return retVal;
    }

    /*
     * Returns whether an edge exists in the graph. Used in topological sort.
     */
    bool hasEdges() {
        for (int i=0; i<size; i++) {
            for (int j=0; j<size; j++) {
                if (matrix[i][j] == 1) {
                    return true;
                }
            }
        }
        return false;
    }

    /*
     * Removes an edge from the graph. DO NOT USE UNLESS IT IS A COPY. Used in 
     * topological sort.
     */
    void removeEdge(int i, int j) {
        matrix[i][j] = 0;
    }

    AdjMatrix multiplyMatrix(AdjMatrix mat) {
        int** retMatrix = new int*[size];
        for (int i=0; i<size; i++) {
            retMatrix[i] = new int[size];
            for (int j=0; j<size; j++) {
                int sum = 0;
                for (int k=0; k<size; k++) {
                    sum = sum + matrix[i][k]*mat.matrix[j][k];
                }
                retMatrix[i][j] = sum;
            }
        }
        AdjMatrix returnValue = AdjMatrix();
        returnValue.size = size;
        returnValue.matrix = retMatrix;
        return returnValue;
    }

    int rowSum(int i) {
        int sum = 0;
        for (int j=0; j<size; j++) {
            sum = sum + matrix[i][j];
        }
        return sum;
    }

    int getSize(){
        return size;
    }

    bool edgeExists(int i, int j){
        return matrix[i][j] == 1;
    }

    int countIn(int i, set<int> s){
        int count = 0;
        for(int j = 0; j < size; j++) {
            if (s.find(j) == s.end()) { // if not on our ignore list
                count += matrix[j][i]; // incoming is (j,i)
            }
        }
        return count;
    }

    int countOut(int i, set<int> s){
        int count = 0;
        for(int j = 0; j < size; j++) {
            if (s.find(j) == s.end()) { // if not on our ignore list
                count += matrix[i][j]; // outgoing is (i,j)
            }
        }
        return count;
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
    /*
     * Used for testing purposes. Specifically, to check the output of the scoreSolution algorithm.
     */
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
        char line[4];
        inFile.getline(line, 100);
    
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
        inFile.close();
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


