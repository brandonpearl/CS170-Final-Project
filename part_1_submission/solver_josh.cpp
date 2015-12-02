#include "solutionScore.h"
#include <vector>
#include <assert.h>
#include <limits.h>
using namespace std;

#define MIN_VAL(a,b) (((a)<(b))?(a):(b))
#define MAX_VAL(a,b) (((a)<(b))?(b):(a))

int debug = 0;

/*
#######################################################################
The only thing you have to know to use this file, is to call the method
std::vector<int> solve_instance(AdjMatrix matrix, AdjList list);
Everything else is taken care of by this method. It will return to you the best
solution this file has found.
#######################################################################
*/

void initialize_vertex_array(int *inp, int size);

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
            if (debug) printf("Keeping swap version\n");
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
            if (debug) printf("Shuffling timing out after %d seconds.\n", timeout);
            return;
        }
        shuffle(arr_cpy, vertex_count);
        int test_score = score_ordering(arr_cpy, list);
        if (test_score > curr_max_score) {
            memcpy(v_array, arr_cpy, sizeof(int)*vertex_count);
            if (debug) printf("Keeping shuffle version\n");
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

    int sorted_by_indegree[size];
    for (num_chosen = 0; num_chosen < size; num_chosen++) {
        int next_vertex = get_smallest_indegree(degrees, size);
        sorted_by_indegree[num_chosen] = next_vertex;
    }
    int indegree_sort_score = score_ordering(sorted_by_indegree, list);

    if (outdegree_sort_score > indegree_sort_score && outdegree_sort_score > curr_score) {
        if (debug) printf("Choosing outdegree ordering\n");
        memcpy(v_array, sorted_by_outdegree, sizeof(int)*size);
    }
    else if (indegree_sort_score > curr_score) {
        if (debug) printf("Choosing indegree ordering\n");
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
    int start_choices[size];
    int i;
    for (i = 0; i < size; i++) {
        start_choices[i] = i+1;
    }
    shuffle(start_choices, size); // Randomize start choices
    int start_vertex;
    for (i = 0; i < size; i++) { // Every possible start vertex
        if (time(NULL) - start_time > timeout) {
            if (debug) printf("Greedy timing out after %d seconds.\n", timeout);
            return;
        }
        start_vertex = start_choices[i];
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
            if (debug) printf("Greedy ordering led to improvement\n");
        }
    }
}


/*
Permute the input array, and choose the permutation with the best score, to be written
to output_buff. Best not to use this on an input array of size larger than 8, as it can
get laggy.
*/
void do_permuration_and_score(int *arr, int size, int original_size, AdjList list, int *curr_score, int *output_buff) {
    if (size == 0) {
        int *arr_base = arr - original_size;
        int test_score = score_partial_ordering(arr_base, list, original_size);
        if (test_score > *curr_score) {
            *curr_score = test_score;
            memcpy(output_buff, arr_base, sizeof(int)*original_size);
        }
        return;
    }
    int i;
    for (i=0; i<size; i++) {
        int temp = arr[0];
        arr[0] = arr[i];
        arr[i] = temp;
        do_permuration_and_score(arr+1, size-1, original_size, list, curr_score, output_buff);
        arr[i] = arr[0];
        arr[0] = temp;
    }
}


/*
Figure out the exact optimal solution, using brute force (try every ordering)
Don't try to call this on large arrays, as this algorithm grows as O(size!).
At the end of the method, sub_array will be reordered according to the absolute
optimal possible for it.
*/
void do_brute_force(int *sub_array, int sub_size, AdjList list) {
    int best_score = 0;
    int buff[sub_size];
    memcpy(buff, sub_array, sizeof(int)*sub_size);
    do_permuration_and_score(sub_array, sub_size, sub_size, list, &best_score, buff);
    memcpy(sub_array, buff, sizeof(int)*sub_size);
}

/*
At this point, we have done brute force search on a number of subsets equal to
num_subsets, where each subset is of size subset_size. Now, we permute
these subsets to try to find the best ordering of the subsets.
Essentially we do brute force on the small subsets, then do approximation by
moving around the order of the subsets in the larger array.
*/
void find_best_subset_ordering(int *subset_order, int num_subsets, int orig_num, int **subset_buff,
                        int subset_size, AdjList list, int *curr_score, int *output_buff) {
    int i;
    if (num_subsets == 0) {
        int *subset_ordering_base = subset_order - orig_num;
        int test_output[list.getSize()];
        int *curr_loc = test_output;
        for (i=0; i<orig_num; i++) {
            int subset_to_pick = subset_ordering_base[i];
            if (subset_to_pick == orig_num) {
                memcpy(curr_loc, subset_buff[subset_to_pick-1], sizeof(int)*(list.getSize()%subset_size));
                curr_loc += list.getSize() % subset_size;
            }
            else {
                memcpy(curr_loc, subset_buff[subset_to_pick-1], sizeof(int)*subset_size);
                curr_loc += subset_size;
            }
        }
        int test_score = score_ordering(test_output, list);
        if (test_score > *curr_score) {
            *curr_score = test_score;
            memcpy(output_buff, test_output, sizeof(int)*list.getSize());
        }
        return;
    }
    for (i=0; i<num_subsets; i++) {
        int temp = subset_order[0];
        subset_order[0] = subset_order[i];
        subset_order[i] = temp;
        find_best_subset_ordering(subset_order+1, num_subsets-1, orig_num,
                            subset_buff, subset_size, list, curr_score, output_buff);
        subset_order[i] = subset_order[0];
        subset_order[0] = temp;
    }
}


/*
Break the v_array into subgroups of size at most subset_size. Do a brute force on each of these
subgroups. Then, figure out the best way to combine these subgroups together.
*/
void divide_and_brute_force(int *v_array, AdjList list, int subset_size) {
    int num_subsets = list.getSize()/subset_size+1;
    int *subset_buff[num_subsets];
    int i;
    for (i=0; i<num_subsets; i++) {
        subset_buff[i] = v_array + i*subset_size;
    }
    int *arr_end = v_array + list.getSize();
    for (i=0; i<num_subsets; i++) {
        int arr_length = MIN_VAL((int) (arr_end - subset_buff[i]), subset_size);
        do_brute_force(subset_buff[i], arr_length, list);
    }
    // Find the best subset ordering
    int num_subsets_for_permutation = MIN_VAL(num_subsets, 5); // limit # of permutations
    int subset_size_for_permutation = list.getSize()/num_subsets_for_permutation + 1;
    int subset_ordering_buff[num_subsets_for_permutation];
    for (i=1; i<=num_subsets_for_permutation; i++) {
        subset_ordering_buff[i-1] = i;
    }
    int *subset_buff_for_permutation[num_subsets_for_permutation];
    for (i=0; i<num_subsets_for_permutation; i++) {
        subset_buff_for_permutation[i] = v_array + i*subset_size_for_permutation;
    }
    int curr_max_score;
    int output_buff[list.getSize()];
    find_best_subset_ordering(subset_ordering_buff, num_subsets_for_permutation, num_subsets_for_permutation,
                    subset_buff_for_permutation, subset_size_for_permutation, list, &curr_max_score, output_buff);
    memcpy(v_array, output_buff, sizeof(int)*list.getSize());
}


/*
Shuffle the input array num_iterations times, and for each iteration, send it to the
divide_and_brute_force method to see if it can improve the ordering.
*/
void alg_brute_force_on_subgroups(int *v_array, AdjMatrix matrix, AdjList list,
                                int num_iterations, int timeout) {
    int v_array_cpy[list.getSize()];
    memcpy(v_array_cpy, v_array, sizeof(int)*list.getSize());
    int curr_max_score = score_ordering(v_array_cpy, list);
    time_t start_time = time(NULL);
    int i;
    for (i=0; i<num_iterations; i++) {
        if (time(NULL) - start_time > timeout) {
            if (debug) printf("Brute force timing out after %d seconds.\n", timeout);
            return;
        }
        shuffle(v_array_cpy, list.getSize());
        divide_and_brute_force(v_array_cpy, list, 7);
        int test_score = score_ordering(v_array_cpy, list);
        if (test_score > curr_max_score) {
            curr_max_score = test_score;
            memcpy(v_array, v_array_cpy, sizeof(int)*list.getSize());
            if (debug) printf("Brute force on subgroups leads to improvement\n");
        }
    }
}

/*
Consider all pairs in a random order, and swap if it leads to improvement.
*/
void alg_swap_all_pairs(int *v_array, AdjMatrix matrix, AdjList list, int timeout) {
    int size = list.getSize();
    int first[size];
    int second[size];
    initialize_vertex_array(first, size);
    initialize_vertex_array(second, size);
    shuffle(first, size);
    shuffle(second, size);
    int locations[size];
    int index;
    for (index = 0; index < size; index++) {
        int node = v_array[index];
        locations[node - 1] = index;
    }
    int curr_score = score_ordering(v_array, list);
    int i, j;
    int improvement_count = 0;
    time_t start_time = time(NULL);
    for (i = 0; i < size; i++) {
        if (debug && time(NULL) - start_time > timeout) {
            printf("Swap all pairs timing out after %d seconds\n", timeout);
            break;
        }
        for (j = 0; j < size; j++) {
            int node_A = first[i];
            int node_B = second[j];
            int index_A = locations[node_A - 1];
            int index_B = locations[node_B - 1];
            v_array[index_A] = node_B;
            v_array[index_B] = node_A;
            int test_score = score_ordering(v_array, list);
            if (test_score > curr_score) {
                improvement_count++;
                if (debug && improvement_count > 9) {
                    printf("Swap all pairs led to improvement x%d\n", improvement_count);
                    improvement_count = 0;
                } 
                curr_score = test_score;
                locations[node_A - 1] = index_B;
                locations[node_B - 1] = index_A;
            }
            else {
                v_array[index_A] = node_A;
                v_array[index_B] = node_B;
            }

        }
    }
    if (debug && improvement_count > 0) {
        printf("Swap all pairs led to improvement x%d\n", improvement_count);
        improvement_count = 0;
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

int refresh_score(int *vertex_array, AdjList list, int *curr_score) {
    int test_score = score_ordering(vertex_array, list);
    if (test_score < *curr_score) {
        return 0;
    }
    int diff = test_score - *curr_score;
    *curr_score = test_score;
    return diff;
}

/*
This is the public method to call for this file. This method will run all of the
algorithms I have written, one at a time, to iteratively improve upon a solution.
It will return the final result of running all of the algorithms in a C++ vector.
*/
std::vector<int> solve_instance_josh(AdjMatrix matrix, AdjList list) {

    time_t start_time = time(NULL);

    int vertex_count = matrix.getSize();

    int vertex_array[vertex_count];
    initialize_vertex_array(vertex_array, vertex_count);

    int node_degrees[vertex_count][2];
    initialize_node_degrees(node_degrees, matrix);

    int initial_score = score_ordering(vertex_array, list);
    int curr_score = initial_score;
    printf("\nStarting solver_josh\n");
    printf("Naive score: %d\n", curr_score);

    alg_order_by_degree(vertex_array, node_degrees, matrix, list);
    printf("Degree sort: +%d\n", refresh_score(vertex_array, list, &curr_score));
    alg_swap(vertex_array, matrix, list);
    printf("Log swap 1: +%d\n", refresh_score(vertex_array, list, &curr_score));
    alg_swap_all_pairs(vertex_array, matrix, list, 25);
    printf("Swap all pairs 1: +%d\n", refresh_score(vertex_array, list, &curr_score));
    alg_brute_force_on_subgroups(vertex_array, matrix, list, 5, 20);
    printf("Brute force on subgroups: +%d\n", refresh_score(vertex_array, list, &curr_score));
    alg_shuffle(vertex_array, matrix, list, 100000, 5);
    printf("Random shuffling: +%d\n", refresh_score(vertex_array, list, &curr_score));
    alg_greedy(vertex_array, matrix, list, 15);
    printf("Greedy: +%d\n", refresh_score(vertex_array, list, &curr_score));
    alg_swap(vertex_array, matrix, list);
    printf("Log swap 2: +%d\n", refresh_score(vertex_array, list, &curr_score));
    alg_swap_all_pairs(vertex_array, matrix, list, 20);
    printf("Swap all pairs 2: +%d\n", refresh_score(vertex_array, list, &curr_score));

    ensure_correct_format(vertex_array, vertex_count);

    int final_score = score_ordering(vertex_array, list);
    printf("Final score %d: ", final_score);

    int score_diff = final_score - initial_score;
    float percentGain = ((float)final_score - (float)initial_score)/(float)initial_score*(float)100;
    printf("Improved by %d points, which is %f%%\n", score_diff, percentGain);
    int elapsed_time = time(NULL) - start_time;
    printf("solver_josh took %d minute(s) %d second(s)\n", elapsed_time/60, elapsed_time%60);
    std::vector<int> v(vertex_array, vertex_array + vertex_count);
    return v;
}

/*
This main method is for debugging and testing this file;
can comment it out to include this file with another.
*/
int josh_main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Need to supply instance filename\n");
        exit(1);
    }

    debug = 1;

    AdjMatrix matrix = AdjMatrix(argv[1]);
    AdjList list = AdjList(argv[1]);
    solve_instance_josh(matrix, list);

    return 0;
}
