#include "solutionScore.cpp"
#include <vector>
#include <iostream>
#include <random>
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

// orders nodes according to the following criteria:
//  - Minimum total incoming nodes is first criteria
//  - If tied, max incoming from nodes already visited
// Returns an int* with this ordering
void greedyIncoming(AdjMatrix matrix, int* inp, int size) {
    int min;
    int indexToAdd;
    set<int> ignore;
    set<int> notIgnore;
    for (int i=0; i<size; i++) {
        notIgnore.insert(i);
    }

    for (int r=0; r<size; r++) { // makes sure we repeat
        min = 1000;
        indexToAdd = 0;
        for (int i=0; i<size; i++) {
            if (ignore.find(i) == ignore.end()) { // if not on our ignore list
                if (matrix.countIn(i, ignore) < min) {
                    min = matrix.countIn(i, ignore);
                    indexToAdd = i+1;
                } else if (matrix.countIn(i, ignore) == min) {
                    int newI = matrix.countOut(i, notIgnore);
                    int oldI = matrix.countOut(indexToAdd, notIgnore);
                    if (newI > oldI) {
                        indexToAdd = i+1;
                    }
                }
            }
        }
        inp[r] = indexToAdd;
        ignore.insert(indexToAdd-1);
        notIgnore.erase(indexToAdd-1);
    }
}

// orders nodes according to the following criteria:
//  - Maximum total outgoing nodes is first criteria
//  - If tied, max incoming from nodes already visited
// Returns an int* with this ordering
void greedyOutgoing(AdjMatrix matrix, int* inp, int size) {
    int max;
    int indexToAdd;
    set<int> ignore;
    set<int> notIgnore;
    for (int i=0; i<size; i++) {
        notIgnore.insert(i);
    }

    for (int r=0; r<size; r++) { // makes sure we repeat
        max = -1;
        indexToAdd = 0;
        for (int i=0; i<size; i++) {
            if (ignore.find(i) == ignore.end()) { // if not on our ignore list
                if (matrix.countOut(i, ignore) > max) {
                    max = matrix.countOut(i, ignore);
                    indexToAdd = i+1;
                } else if (matrix.countOut(i, ignore) == max) {
                    int newI = matrix.countIn(i, notIgnore);
                    int oldI = matrix.countIn(indexToAdd, notIgnore);
                    if (newI > oldI) {
                        indexToAdd = i+1;
                    }
                }
            }
        }
        inp[r] = indexToAdd;
        ignore.insert(indexToAdd-1);
        notIgnore.erase(indexToAdd-1);
    }
}

// generates 10,000 random orders and returns the max scoring one
void randomSolver (int* inp, int size, AdjList list) {
    std::vector<int> v(inp, inp + size); 
    int score = scoreSolution(v, list);
    int newScore;
    std::vector<int> copy(v);
    for (int i=0; i<1000; i++) {
        std::random_shuffle(copy.begin(), copy.end());
        newScore = scoreSolution(copy, list); 
        if (newScore > score){
            score = newScore;
            v = copy;
        }
    }
    for (int i=0; i < size; i++) {
        inp[i] = v[i];
    }
    return;
}

// Dad's idea
//void optimalReorder(int* inp, int size, AdjMatrix matrix) {
//    bool stop;
//    set<int> in;
//    set<int> out;
//    int b = 0;
//    int e = size-1;
//    while (true) {
//        stop = true;
//        for (int i = 0; i < size; i++) {
//            if (matrix.countIn(i, in) == 0) {
//                in.insert(i);
//                int temp = inp[b];
//                inp[i]
//            }
//        }
//         
//        if (stop) {
//            break;
//        }
//    } 
//}

std::vector<int> solve_instance_matt(AdjMatrix matrix, AdjList list) {
    int size = matrix.getSize();
    int vertex_array[size];

    initialize_vertex_array(vertex_array, size);
    int score = score_ordering(vertex_array, list);
    printf("Naive score is: %d\n", score);

    randomSolver(vertex_array, size, list);
    score = score_ordering(vertex_array, list);
    printf("randomSolver score is: %d\n", score);

    greedyIncoming(matrix, vertex_array, size);
    score = score_ordering(vertex_array, list);
    printf("GreedyIncoming score is: %d\n", score);

    greedyOutgoing(matrix, vertex_array, size);
    score = score_ordering(vertex_array, list);
    printf("GreedyOutgoing score is: %d\n", score);

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
