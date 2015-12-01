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

    int countl=0;
    int countm=0;
    for (int i=0; i<size; i++) {
        for (int j=0; j<size; j++) {
            bool m = adjM.edgeExists(i, j);
            bool l = adjL.edgeExists(i, j);
            assert(m==l);
            if (l) {
                countl++;
            }
            if (m) {
                countm++;
            }
        }
    }
    printf("final list count was: %d\n", countl);
    printf("final matrix count was: %d\n", countm);
    printf("success\n");

    return 0;

}
