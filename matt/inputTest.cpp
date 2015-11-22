#include <iostream>
#include <fstream>
#include <vector>
#include "processInput.h"
using namespace std;

int main(int argc, char *argv[]) {
    processInput(argc, argv);
    adjMatrix adjM = getAdjMatrix();
    adjList adjL = getAdjList();

    int size = adjM.size;

    // Prints out the adjacency matrix (to check)
    printf("The adjancency matrix is:\n");
    for (int i=0; i < size; i++) {
        for (int j=0; j < size; j++) {
            printf("%d ", adjM.matrix[i][j]);
        }
        printf("\n");
    }

    printf("\n");

    // Prints out the adjaceny list (to check)
    printf("The adjancency list is:\n");
    vector<int> v;
    for (int i=0; i < size; i++) {
        printf("%d ", i);
        v = adjL.list[i];
        for (vector<int>::iterator iter = v.begin(); iter != v.end(); iter++) {
            printf("->%d", *iter);
        }
        printf("\n");
    }

    return 0;
}
