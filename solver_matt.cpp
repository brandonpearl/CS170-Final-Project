#include "solver_josh.cpp"
#include <vector>
#include <iostream>
#include <random>
#include <chrono>
#include <list>
#include <stack>
using namespace std;

//========================
//====== UTILITIES =======
//========================

int matt_score_ordering(int *ordering, AdjList list) {
    std::vector<int> v(ordering, ordering + list.getSize());
    return scoreSolution(v, list);
}

void matt_print_array(int *inp, int size) {
    printf("[");
    int  count=0;
    for (int i=0; i<size; i++){
        printf(" %d", inp[i]);
        count++;
    }
    printf("]\n");
    printf("size is: %d\n", count);
}

// Initializes vertex array by picking a random order and seeing whether
// it or its reverse returns a better score. Will set inp to be whichever
// one returns the higher score
void initialize_vertex_array(int *inp, int size, AdjList list) {
    for (int i=0; i < size; i++) {
        inp[i] = i+1;
    }
    std::vector<int> v(inp, inp + size);

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    auto gen = std::default_random_engine(seed);
    shuffle(v.begin(), v.end(), gen);

    // Pick either this or reverse. Whichever is bigger
    int defScore = scoreSolution(v, list);
    std::vector<int> copy(v);
    for (int i=0; i<size; i++) {
        copy[i] = v[size-i-1];
    }
    int revScore = scoreSolution(copy, list);
    if (revScore > defScore){
        v = copy;
    }

    // Print this array out and set inp equal to it
    printf("Naive ordering is: [");

    for (int i=0; i < size; i++) {
        printf("%d, ", v[i]);
        inp[i] = v[i];
    }
    printf("]\n");
}

// Copies array "from" to "to", where both arrays have "size" size
void copyIntArray (int* to, int* from, int size) {
    for (int i=0; i<size; i++) {
        to[i] = from[i];
    }
}


//========================
//====== ALGORITHMS ======
//========================

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

// generates 1,000 random orders and returns the max scoring one
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

// Compares each pair from (1,2) to (99,100) and switches
// if the switch will be better
void pairSwapForward (AdjList list, int* inp, int size) {
    std::vector<int> v(inp, inp + size);
    int score = scoreSolution(v, list);
    int newScore;
    for (int i=0; i<100; i++) {
        for (int j=i+1; j<100; j++) {
            std::vector<int> copy(v);
            int temp = copy[j];
            copy[j] = copy[i];
            copy[i] = temp;
            newScore = scoreSolution(copy, list);
            if (newScore > score){
                score = newScore;
                v = copy;
            }
        }
    }
    for (int i=0; i < size; i++) {
        inp[i] = v[i];
    }
    return;
}

// Compares each pair from (99,100) to (1,2) and switches
// if the switch will be better
void pairSwapBackward (AdjList list, int* inp, int size) {
    std::vector<int> v(inp, inp + size);
    int score = scoreSolution(v, list);
    int newScore;
    for (int i=99; i>=0; i--) {
        for (int j=i-1; j>=0; j--) {
            std::vector<int> copy(v);
            int temp = copy[j];
            copy[j] = copy[i];
            copy[i] = temp;
            newScore = scoreSolution(copy, list);
            if (newScore > score){
                score = newScore;
                v = copy;
            }
        }
    }
    for (int i=0; i < size; i++) {
        inp[i] = v[i];
    }
    return;
}

// For each node:
// - Run DFS starting from that node
// - Return the nodes in topological order
// Inspired by: http://www.geeksforgeeks.org/topological-sorting/

void topologicalSortUtil(AdjList list, int i, int size, bool* visited, stack<int> &Stack){
    // Mark the current node as visited
    visited[i] = true;

    // Recurse for all the vertices adjacent to this vertex
    for (int n=1; n<size+1; n++){
        if (list.edgeExists(i-1,n-1)&&(!visited[n])) {
            topologicalSortUtil(list, n, size, visited, Stack);
        }
    }
    Stack.push(i);
    return;
}

void topologicalSort(AdjList list, int* inp, int size) {
    stack<int> Stack;
    int bestScore = 0;
    for (int s=1; s<size+1; s++) {
        int temp[size];
        bool visited[size+1];

        for (int i=1; i<size+1; i++) {
            visited[i] = false;
        }

        for (int i=s; i<size+1; i++) {
            if (visited[i] == false) {
                topologicalSortUtil(list, i, size, visited, Stack);
            }
        }
        for (int i=1; i<s; i++) {
            if (visited[i] == false) {
                topologicalSortUtil(list, i, size, visited, Stack);
            }
        }

        int i=0;
        while (!Stack.empty()) {
            temp[i] = Stack.top();
            Stack.pop();
            i += 1;
        }
        int newScore = matt_score_ordering(temp, list);
        if (newScore > bestScore){
            bestScore = newScore;
            for (int i=0; i<size; i++){
                inp[i] = temp[i];
            }
        }
    }
    return;
}

//========================
//======== SOLVER ========
//========================

std::vector<int> solve_instance_matt(AdjMatrix matrix, AdjList list) {
    int size = matrix.getSize();
    int vertex_array[size];
    int best_array[size];
    int score;
    int newScore;
    int naiveScore;
    initialize_vertex_array(vertex_array, size, list);
    copyIntArray(best_array, vertex_array, size);

    score = matt_score_ordering(vertex_array, list);
    naiveScore = score;
    printf("Naive score is: %d\n", score);

    //randomSolver(vertex_array, size, list);
    //newScore = matt_score_ordering(vertex_array, list);
    //if (newScore > score) {
    //    copyIntArray(best_array, vertex_array, size);
    //    score = newScore;
    //}
    //printf("randomSolver score is: %d\n", newScore);


    topologicalSort(list, vertex_array, size);
    newScore = matt_score_ordering(vertex_array, list);
    printf("successfully finished\n");
    if (newScore > score) {
        copyIntArray(best_array, vertex_array, size);
        score = newScore;
    }
    printf("topologicalSort score is: %d\n", newScore);


    greedyIncoming(matrix, vertex_array, size);
    newScore = matt_score_ordering(vertex_array, list);
    if (newScore > score) {
        copyIntArray(best_array, vertex_array, size);
        score = newScore;
    }
    printf("GreedyIncoming score is: %d\n", newScore);

    greedyOutgoing(matrix, vertex_array, size);
    newScore = matt_score_ordering(vertex_array, list);
    if (newScore > score) {
        copyIntArray(best_array, vertex_array, size);
        score = newScore;
    }
    printf("GreedyOutgoing score is: %d\n", newScore);

    pairSwapForward(list, vertex_array, size);
    newScore = matt_score_ordering(vertex_array, list);
    if (newScore > score) {
        copyIntArray(best_array, vertex_array, size);
        score = newScore;
    }
    printf("pairSwapForward #1 score is: %d\n", newScore);

    pairSwapBackward(list, vertex_array, size);
    newScore = matt_score_ordering(vertex_array, list);
    if (newScore > score) {
        copyIntArray(best_array, vertex_array, size);
        score = newScore;
    }
    printf("pairSwapBackward #1 score is: %d\n", newScore);

    pairSwapForward(list, vertex_array, size);
    newScore = matt_score_ordering(vertex_array, list);
    if (newScore > score) {
        copyIntArray(best_array, vertex_array, size);
        score = newScore;
    }
    printf("pairSwapForward #2 score is: %d\n", newScore);

    pairSwapBackward(list, vertex_array, size);
    newScore = matt_score_ordering(vertex_array, list);
    if (newScore > score) {
        copyIntArray(best_array, vertex_array, size);
        score = newScore;
    }
    printf("pairSwapBackward #2 score is: %d\n", newScore);

    int scoreGain = score-naiveScore;
    float percentGain = ((float)score - (float)naiveScore)/(float)naiveScore*(float)100;
    printf("Improved by %d points, which is %f percent\n", scoreGain, percentGain);

    vector<int> best (vertex_array, vertex_array + size);
    return best;
}

int matt_main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Need to supply instance filename\n");
        exit(1);
    }
    AdjMatrix matrix (argv[1]);
    AdjList list (argv[1]);

    solve_instance_matt(matrix, list);
    return 0;
}

int main(int argc, char *argv[]) {
    return matt_main(argc, argv);
}
