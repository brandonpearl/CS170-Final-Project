#include <iostream>
#include <vector>
#include <limits.h>
#ifndef _adj
#define _adj
#include "matt/processinput.h"
#include "solutionScore.h"
#endif
#include "solver_josh.cpp"
#include "solver_matt.cpp"
#include "solver_brandon.cpp"

using namespace std;


vector<int> run_algs(char *filename) {
  AdjMatrix objectMatrix (filename);
  AdjList objectList (filename);
  int maxScore = INT_MIN;
  //All ranks stored here
  vector<vector<int> > ranks;

  //All other rankings will be added accordingly
  //ranks.push_back(solve_instance_josh(objectMatrix, objectList));
  ranks.push_back(solve_instance_matt(objectMatrix, objectList));
  //ranks.push_back(solve_instance_brandon(objectMatrix, objectList));


  int score;
  vector<int> best;
  for (vector<int> rank : ranks){
    score = scoreSolution(rank, objectList);
    if (score > maxScore) {
      best = rank;
      maxScore = score;
    }
  }
  cout<<"Optimal rank for given instance: "<< endl;

  //Can figure how to best output rank once we are done adding algos.
  cout<<"[";
  for (int i: best)
    cout<< i<<", ";
  cout << "]" << endl;

  return best;
}

/* 
Can either do
  ./MAS filename.in to run on one file
  OR
  ./MAS X Y to run from instances/X.in to instances/Y.in and generate an output file
*/
int main(int argc, char *argv[]){

  if (argc == 2) {
    run_algs(argv[1]);
    return 0;
  }
  if (argc != 3) {
    printf("Supply start file number, and end file number\n");
  }

  int start_file_no = atoi(argv[1]);
  int end_file_no = atoi(argv[2]);

  if (end_file_no < start_file_no) {
    printf("End must be larger than start\n");
    exit(1);
  }

  char output_file[1024];
  snprintf(output_file, sizeof(output_file), "%d_to_%d.out", start_file_no, end_file_no);
  FILE *f = fopen(output_file, "w");
  if (f == NULL) {
      printf("Error: results file creation error\n");
      exit(1);
  }

  printf("Running on files %d.in to %d.in\n", start_file_no, end_file_no);

  int i;
  for (i = start_file_no; i <= end_file_no; i++) {
    char file_name[1024];
<<<<<<< HEAD
    int err = snprintf(file_name, sizeof(file_name), "instances/%d.in", i);
    if (err < 0) {
        printf("\nError: Filename %s not found\n", file_name);
        return -1;
    }
=======
    snprintf(file_name, sizeof(file_name), "instances/%d.in", i);
>>>>>>> 82075b1b962d349a389f5a846d71b0081760a260
    printf("\nRunning on %s\n", file_name);
    vector<int> sol = run_algs(file_name);
    for (vector<int>::iterator j=sol.begin(); j!=sol.end(); ++j) {
      fprintf(f, "%d ", *j);
    }
    fprintf(f, "\n");
  }


}
