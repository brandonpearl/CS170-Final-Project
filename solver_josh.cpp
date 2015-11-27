// #include "matt/processInput.h"
#include "solutionScore.cpp"
#include <vector>
#include <assert.h>
#include <limits.h>
using namespace std;

/*
Utility method to print out the first 'size' elements of the input array, in
a nice format.
*/
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
Utility method specifically to print out the 2-d degree array being used by the 
alg_order_by_degree.
*/
void print_degrees(int arr[][2], int size) {
    int i;
    for (i=0; i<size; i++) {
        printf("Node %d indegree: %d, outdegree: %d\n", i+1, arr[i][0], arr[i][1]);
    }
}

/*
This function adapted from basic online shuffle code to randomize the ordering
of elements in the array.
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

/*
Convert the C integer array into a C++ vector, and return the objective score
using the scoreSolution method.
*/
int score_ordering(int *ordering, AdjList list) {
    std::vector<int> v(ordering, ordering + list.getSize());
    return scoreSolution(v, list);
}

/*
Convert the C integer array into a C++ vector, and evaluate the score for the 
first col_sol_size elements using the scorePartialSolution method.
*/
int score_partial_ordering(int *ordering, AdjList list, int cur_sol_size) {
    std::vector<int> v(ordering, ordering + list.getSize());
    return scorePartialSolution(v, list, cur_sol_size);
}

/*
Swaps adjacent groups of elements of size subset_count, if doing such a swap
improves the score of the input array.
*/
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
            printf("Keeping swap version\n");
            curr_max_score = test_score;
            memcpy(arr+curr_start, arr_cpy+curr_start, sizeof(int)*2*subset_count);
        }
    }
}

/*
Shuffle the input array shuffle_count times, or until timeout number of seconds.
Keep the shuffled version if it has a higher score than our current array.
*/
void alg_shuffle(int *v_array, AdjMatrix matrix, AdjList list, int shuffle_count, int timeout) {
    int curr_max_score = score_ordering(v_array, list);
    int vertex_count = matrix.getSize();
    int arr_cpy[vertex_count];
    memcpy(arr_cpy, v_array, sizeof(int)*vertex_count);
    time_t start_time = time(NULL);
    int i;
    for (i=0; i < shuffle_count; i++) {
        if (i%300 == 0 && time(NULL) - start_time > timeout) {
            printf("Shuffling timing out after %d seconds.\n", timeout);
            return;
        }
        shuffle(arr_cpy, vertex_count);
        int test_score = score_ordering(arr_cpy, list);
        if (test_score > curr_max_score) {
            memcpy(v_array, arr_cpy, sizeof(int)*vertex_count);
            printf("Keeping shuffle version\n");
            curr_max_score = test_score;
        }
    }
}

/*
Swaps adjacent groups of elements of size 1,2,...,log(n), if doing such a swap
results in an improvement in score.
*/
void alg_swap(int *v_array, AdjMatrix matrix, AdjList list) {
    int vertex_count = matrix.getSize();
    int subset_size;
    for (subset_size = vertex_count/2; subset_size >= 1; subset_size /= 2) {
        swap_if_better(v_array, subset_size, vertex_count, list);
    }

}

/*
Given an array of indegrees and outdegrees for each node, return the one
that currently has the largest outdegree.
*/
int get_highest_outdegree(int node_degrees[][2], int num_nodes) {
    int curr_max_val = -1;
    int curr_max_node = -1;
    int i;
    for (i=0; i<num_nodes; i++) {
        if (node_degrees[i][1] > curr_max_val) {
            curr_max_val = node_degrees[i][1];
            curr_max_node = i+1;
        }
    }
    assert(curr_max_node > 0);
    assert(curr_max_node <= num_nodes);
    node_degrees[curr_max_node-1][1] = -1; // Remove from future consideration
    return curr_max_node;
}

/*
Given an array of indegrees and outdegrees for each node, return the one
that currently has the smallest indegree.
*/
int get_smallest_indegree(int node_degrees[][2], int num_nodes) {
    int curr_min_val = INT_MAX;
    int curr_min_node = -1;
    int i;
    for (i=0; i<num_nodes; i++) {
        if (node_degrees[i][0] < curr_min_val) {
            curr_min_val = node_degrees[i][0];
            curr_min_node = i+1;
        }
    }
    assert(curr_min_node > 0);
    assert(curr_min_node <= num_nodes);
    node_degrees[curr_min_node-1][0] = INT_MAX; // Remove from future consideration
    return curr_min_node;
}


/*
* Note that this method will "destroy" the information inside degrees, so copy it first if
* you want to retain its data
This algorithm will try ordering the array by outdegree descending, and indegree ascending, and
keep the version with the best score, if that score is better than the score the input array
currently has.
*/
void alg_order_by_degree(int *v_array, int degrees[][2], AdjMatrix matrix, AdjList list) {
    int curr_score = score_ordering(v_array, list);
    int size = matrix.getSize();
    int sorted_by_outdegree[size];
    int num_chosen;
    for (num_chosen = 0; num_chosen < size; num_chosen++) {
        int next_vertex = get_highest_outdegree(degrees, size);
        sorted_by_outdegree[num_chosen] = next_vertex;
    }
    int outdegree_sort_score = score_ordering(sorted_by_outdegree, list);
    if (outdegree_sort_score > curr_score) {
        printf("Outdegree sort is better than base\n");
    }

    int sorted_by_indegree[size];
    for (num_chosen = 0; num_chosen < size; num_chosen++) {
        int next_vertex = get_smallest_indegree(degrees, size);
        sorted_by_indegree[num_chosen] = next_vertex;
    }
    int indegree_sort_score = score_ordering(sorted_by_indegree, list);
    if (indegree_sort_score > curr_score) {
        printf("Indegree sort is better than base\n");
    }

    if (outdegree_sort_score > indegree_sort_score && outdegree_sort_score > curr_score) {
        printf("Choosing outdegree ordering\n");
        memcpy(v_array, sorted_by_outdegree, sizeof(int)*size);
    }
    else if (indegree_sort_score > curr_score) {
        printf("Choosing indegree ordering\n");
        memcpy(v_array, sorted_by_indegree, sizeof(int)*size);
    }
}

/*
Used to sort elements ascending.
*/
int compare_ascending(const void *elem1, const void *elem2) {
    return *(int*)elem1 - *(int*)elem2;
}

/*
For every possible start vertex, chooses the next vertices in order, based on which one
will give the best intermediate score for the current size solution created.
Will stop after timeout seconds.
*/
void alg_greedy(int *v_array, AdjMatrix matrix, AdjList list, int timeout) {
    int size = matrix.getSize();
    int output_buff[size];
    int sorted_v_array_buff[size];
    memcpy(sorted_v_array_buff, v_array, sizeof(int)*size);
    qsort(sorted_v_array_buff, size, sizeof(int), compare_ascending);
    int remaining_vertices[size];
    int v_array_curr_score = score_ordering(v_array, list);
    time_t start_time = time(NULL);
    int start_vertex;
    for (start_vertex = 1; start_vertex <= size; start_vertex++) { // Every possible start vertex
        if (time(NULL) - start_time > timeout) {
            printf("Greedy timing out after %d seconds.\n", timeout);
            return;
        }
        memcpy(remaining_vertices, sorted_v_array_buff, sizeof(int)*size);
        output_buff[0] = start_vertex;
        remaining_vertices[start_vertex-1] = -1;
        int curr_sol_size;
        for (curr_sol_size = 2; curr_sol_size <= size; curr_sol_size++) {
            int next_vertex;
            int curr_score = -1;
            int curr_best_next_node = -1;
            for (next_vertex = 1; next_vertex <= size; next_vertex++) {
                if (remaining_vertices[next_vertex-1] == -1) {
                    continue;
                }
                output_buff[curr_sol_size-1] = next_vertex;
                int test_score = score_partial_ordering(output_buff, list, curr_sol_size);
                if (test_score > curr_score) {
                    curr_score = test_score;
                    curr_best_next_node = next_vertex;
                }
            }
            assert(curr_score >= 0);
            assert(curr_best_next_node > 0);
            output_buff[curr_sol_size-1] = curr_best_next_node;
            remaining_vertices[curr_best_next_node-1] = -1;
        }
        int start_vertex_test_score = score_ordering(output_buff, list);
        if (start_vertex_test_score > v_array_curr_score) {
            v_array_curr_score = start_vertex_test_score;
            memcpy(v_array, output_buff, sizeof(int)*size);
            printf("Greedy ordering led to improvement\n");
        }
    }
}

/*
Initialize the array to have elements 1...size
*/
void initialize_vertex_array(int *inp, int size) {
    int i;
    for (i = 0; i < size; i++) {
        inp[i] = i+1;
    }
}

/*
Fill out the node degrees array, so that for a given node, its indegree is 
arr[node-1][0], and its outdegree is arr[node-1][1] (assuming nodes start
numbering from 1)
*/
void initialize_node_degrees(int arr[][2], AdjMatrix matrix) {
    int size = matrix.getSize();
    int i;
    for (i=0; i<size; i++) {
        arr[i][0] = 0;
        arr[i][1] = 0;
    }

    int start, destination;
    for (start = 0; start < size; start++) {
        for (destination = 0; destination < size; destination++) {
            if (matrix.edgeExists(start, destination)) {
                arr[start][1]++; // Outdegree
                arr[destination][0]++; // Indegree
            }
        }
    }
}

/*
Verifies that the solution is in correct format (ie. we have nodes 1...n, and
we only have each node once)
*/
void ensure_correct_format(int *v_array, int size) {
    int buff[size];
    int i;
    for (i=0; i<size; i++) {
        buff[i] = 0;
    }
    for (i=0; i<size; i++) {
        buff[v_array[i]-1]++;
    }
    for (i=0; i<size; i++) {
        assert(buff[i] == 1);
    }
}

/*
This is the public method to call for this file. This method will run all of the
algorithms I have written, one at a time, to iteratively improve upon a solution.
It will return the final result of running all of the algorithms in a C++ vector.
*/
std::vector<int> solve_instance(AdjMatrix matrix, AdjList list) {

    int vertex_count = matrix.getSize();

    int vertex_array[vertex_count];
    initialize_vertex_array(vertex_array, vertex_count);

    int node_degrees[vertex_count][2];
    initialize_node_degrees(node_degrees, matrix);

    printf("Initial, with score %d: ", score_ordering(vertex_array, list));
    print_array(vertex_array, matrix.getSize());

    printf("------ Sorting by degree ------\n");
    alg_order_by_degree(vertex_array, node_degrees, matrix, list);
    printf("------ Swapping ------\n");
    alg_swap(vertex_array, matrix, list);
    printf("------ Shuffling ------\n");
    alg_shuffle(vertex_array, matrix, list, 100000, 30);
    printf("------ Swapping ------\n");
    alg_swap(vertex_array, matrix, list);
    printf("------ Greedy begin ------\n");
    alg_greedy(vertex_array, matrix, list, 90);
    printf("------ Swapping ------\n");
    alg_swap(vertex_array, matrix, list);

    ensure_correct_format(vertex_array, vertex_count);

    printf("Final, with score %d: ", score_ordering(vertex_array, list));
    print_array(vertex_array, matrix.getSize());

    std::vector<int> v(vertex_array, vertex_array + vertex_count);
    return v;
}

/*
For debugging and testing this file; can comment out to include this file with another.
*/
int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Need to supply instance filename\n");
        exit(1);
    }

    AdjMatrix matrix = AdjMatrix(argv[1]);
    AdjList list = AdjList(argv[1]);
    solve_instance(matrix, list);

//     return 0;
// }
=======
    return 0;
}
