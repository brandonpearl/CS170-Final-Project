// #include "matt/processInput.h"
#include "solutionScore.cpp"
#include <vector>
using namespace std;

void print_array(int *arr, int size) {
    int i;
    printf("[");
    char delim1 = ',';
    char delim2 = ' ';
    for (i=0; i<size; i++) {
        if (i == size-1) {
            delim1 = ']';
            delim2 = '\n';
        }
        printf("%d%c%c", arr[i], delim1, delim2);
    }
}

/*
This function adapted from basic online shuffle code
*/
void shuffle(int *array, int size)
{
    if (size > 1) 
    {
        int i;
        for (i = 0; i < size - 1; i++) 
        {
          int j = i + rand() / (RAND_MAX / (size - i) + 1);
          int t = array[j];
          array[j] = array[i];
          array[i] = t;
        }
    }
}

int score_ordering(int *ordering, AdjList list) {
    std::vector<int> v(ordering, ordering + list.getSize());
    return scoreSolution(v, list);
}

void swap_if_better(int *arr, int subset_count, int size, AdjList list) {
    int curr_max_score = score_ordering(arr, list);

    int arr_cpy[size];

    int buff[subset_count];
    int curr_start;
    for (curr_start = 0; curr_start + 2*subset_count <= size; curr_start += 2*subset_count) {
        memcpy(arr_cpy, arr, sizeof(int)*size);
        memcpy(buff, arr_cpy+curr_start, sizeof(int)*subset_count);
        memcpy(arr_cpy+curr_start, arr_cpy+curr_start+subset_count, sizeof(int)*subset_count);
        memcpy(arr_cpy+curr_start+subset_count, buff, sizeof(int)*subset_count);
        int test_score = score_ordering(arr_cpy, list);
        if (test_score > curr_max_score) {
            printf("Keeping test version\n");
            curr_max_score = test_score;
            memcpy(arr+curr_start, arr_cpy+curr_start, sizeof(int)*2*subset_count);
        }
    }


}

void alg_shuffle(int *v_array, AdjMatrix matrix, AdjList list, int shuffle_count) {
    int curr_max_score = score_ordering(v_array, list);
    int vertex_count = matrix.getSize();
    int arr_cpy[vertex_count];
    memcpy(arr_cpy, v_array, sizeof(int)*vertex_count);
    int i;
    for (i=0; i < shuffle_count; i++) {
        shuffle(arr_cpy, vertex_count);
        int test_score = score_ordering(arr_cpy, list);
        if (test_score > curr_max_score) {
            printf("Keeping shuffled version\n");
            curr_max_score = test_score;
            memcpy(v_array, arr_cpy, sizeof(int)*vertex_count);
        }
    }
}


void alg_swap(int *v_array, AdjMatrix matrix, AdjList list) {
    int vertex_count = matrix.getSize();
    int subset_size;
    for (subset_size = vertex_count/2; subset_size >= 1; subset_size /= 2) {
        swap_if_better(v_array, subset_size, vertex_count, list);
    }

}


void initialize_vertex_array(int *inp, int size) {
    int i;
    for (i = 0; i < size; i++) {
        inp[i] = i+1;
    }
}

/*
This is the public method to call for this file
*/
std::vector<int> solve_instance(AdjMatrix matrix, AdjList list) {

    int vertex_count = matrix.getSize();

    int vertex_array[vertex_count];
    initialize_vertex_array(vertex_array, vertex_count);

    printf("Initial, with score %d: ", score_ordering(vertex_array, list));
    print_array(vertex_array, matrix.getSize());

    alg_shuffle(vertex_array, matrix, list, 100000);
    alg_swap(vertex_array, matrix, list);

    printf("Final, with score %d: ", score_ordering(vertex_array, list));
    print_array(vertex_array, matrix.getSize());

    std::vector<int> v(vertex_array, vertex_array + vertex_count);
    return v;
}


// int main(int argc, char *argv[]) {
//     if (argc != 2) {
//         printf("Need to supply instance filename\n");
//         exit(1);
//     }

//     solve_instance(argv[1]);

//     return 0;
// }