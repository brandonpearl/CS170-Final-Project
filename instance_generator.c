#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int string_to_int(char *s) {
    int ret_val = 0;
    while (*s != '\0') {
        ret_val = 10 * ret_val + *s - '0';
        s += 1;
    }
    return ret_val;
}

void generate_instance(int node_count, float p_edge, char *file_name) {
    FILE *test_exist = fopen(file_name, "r");
    if (test_exist != NULL) {
        printf("File already exists\n");
        fclose(test_exist);
        exit(1);
    }
    FILE *f = fopen(file_name, "w");
    if (f == NULL) {
        printf("File creation error\n");
        exit(1);
    }
    fprintf(f, "%d\n", node_count);

    int row, column;
    srand(time(NULL));

    for (row = 0; row < node_count; row++) {
        char delimeter = ' ';
        for (column = 0; column < node_count; column++) {
            int rand_val = rand() % 101;
            int edge_val = row != column && rand_val <= p_edge;
            if (column == node_count - 1 && row < node_count - 1) {
                delimeter = '\n';
            }
            fprintf(f, "%d%c", edge_val, delimeter);
        }
    }
}

/*
Program takes three command line args:
1: Node count; integer from 0 to 100
2: Edge probability; integer from 0 to 100 representing percentage change that a 
                      given edge exists
3: Destination filename; filename must not already exist. Should include extenstion,
                         for example: ex.in
*/
int main(int argc, char* argv[]){
    if (argc != 4) {
        printf("Incorrect arg count\n");
        exit(1);
    }
    int node_count = string_to_int(argv[1]);
    int p_edge = string_to_int(argv[2]);
    char *file_name = argv[3];
    if (node_count < 0 || node_count > 100) {
        printf("Node count must be a number from 0 to 100\n");
        exit(1);
    }
    if (p_edge < 0 || p_edge > 100) {
        printf("Edge probability must be an integer from 0 to 100\n");
        exit(1);
    }
    // printf("Args are: %d %d %s\n", node_count, p_edge, file_name);
    generate_instance(node_count, p_edge, file_name);
}
