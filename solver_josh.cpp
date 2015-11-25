#include "matt/processInput.h"
using namespace std;


int main(int argc, char *argv[]) {
    AdjMatrix matrix = AdjMatrix("small.in");
    printf("matrix size is %d\n", matrix.getSize());
    printf("spot 0,0 is %d\n", matrix.is_edge(0, 0));
    printf("spot 2,1 is %d\n", matrix.is_edge(2, 1));
    printf("spot 1,2 is %d\n", matrix.is_edge(1, 2));
    AdjList list = AdjList("small.in");
    printf("list size is %d\n", list.getSize());
    printf("spot 0,0 is %d\n", list.is_edge(0, 0));
    printf("spot 2,1 is %d\n", list.is_edge(2, 1));
    printf("spot 1,2 is %d\n", list.is_edge(1, 2));

}