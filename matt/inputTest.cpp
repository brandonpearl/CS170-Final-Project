#include <iostream>
#include <fstream>
#include <set>
#include <assert.h>
#include "processInput.h"

using namespace std;

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Correct usage is: ./inputTest <filepath>\n"); 
        return 0;
    }
    char* filename = argv[1];
    AdjMatrix adjM (filename);
    AdjList adjL (filename);

    int size = adjM.getSize();

    // Prints out the adjacency matrix (to check)
    printf("The adjancency matrix is:\n");
    for (int i=0; i < size; i++) {
        for (int j=0; j < size; j++) {
            if (adjM.edgeExists(i,j)){
                printf("1 ");
            }else{
                printf("0 ");
            }
        }
        printf("\n");
    }

    printf("\n");

    // makes sure adjancencies in list and matrix follow
    assert(adjM.edgeExists(0,6));
    assert(!adjM.edgeExists(9,1));
    assert(adjM.edgeExists(4,7));
    assert(!adjM.edgeExists(5,5));
    assert(adjM.edgeExists(8,2));


    // Prints out the adjaceny list (to check)
    printf("The adjancency list is:\n");
    for (int i=0; i < size; i++) {
        printf("%d ", i);
        for (int j=0; j < size; j++) {
            if (adjL.edgeExists(i,j)) {
                printf("->%d", j);
            }
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
