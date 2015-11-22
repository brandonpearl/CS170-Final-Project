#include <iostream>
#include <fstream>
#include <set>
#include <assert.h>
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
    set<int> s;
    for (int i=0; i < size; i++) {
        printf("%d ", i);
        s = adjL.list[i];
        for (set<int>::iterator iter = s.begin(); iter != s.end(); iter++) {
            printf("->%d", *iter);
        }
        printf("\n");
    }

    // makes sure adjancencies in list and matrix follow
    assert(adjL.edgeExists(0,6));
    assert(!adjL.edgeExists(9,1));
    assert(adjL.edgeExists(4,7));
    assert(!adjL.edgeExists(5,5));
    assert(adjL.edgeExists(8,2));

    return 0;
}
