#include <iostream>
#include "matt/processinput.h"
#include "solutionScore.cpp"
#include <vector>
#include "solver_josh.cpp"
#include <limits.h>

using namespace std;

int main(int argc, char *argv[]){
  //For now, will have to get called for each instance.

      if (argc != 2) {
          cout<<"Need to supply instance filename"<<endl;
          exit(1);
      }

      AdjMatrix objectMatrix (argv[1]);
      AdjList objectList (argv[1]);
      int maxScore = INT_MIN;

      //All ranks stored here
      vector<vector<int>> ranks;

      //All other rankings will be added accordingly
      ranks.push_back(solve_instance(objectMatrix, objectList));

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
      for (int i: best)
        cout<< i;

      return 0;

}
