#include "solutionScore.cpp"
#include <vector>
#include <iostream>
using namespace std;

int score_ordering(int *ordering, AdjList list) {
    std::vector<int> v(ordering, ordering + list.getSize());
    return scoreSolution(v, list);
}

void initialize_vertex_array(int *inp, int size) {
    int i;
    for (i=0; i < size; i++) {
        inp[i] = i+1;
    }
}

// will simply put the node with the most edges first and so on
int* naive_greedy(AdjMatrix matrix, int size) {
    int* num_edges[size];
    for (int i=0; i<size; i++){
        for (int j=0; j<size; j++){
            if (matrix.edgeExists(i,j)) {
                num_edges[i] += 1;
            }
        }
    }


}

std::vector<int> solve_instance_matt(AdjMatrix matrix, AdjList list) {
    int size = matrix.getSize();
    int vertex_array[size];

    initialize_vertex_array(vertex_array, size);
    int score = score_ordering(vertex_array, list);

    printf("Naive score is: %d\n", score);



    vector<int> best (vertex_array, vertex_array + size);
    return best;
}


int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Need to supply instance filename\n");
        exit(1);
    }
    AdjMatrix matrix (argv[1]);
    AdjList list (argv[1]);

    solve_instance_matt(matrix, list);
    return 0;
}
