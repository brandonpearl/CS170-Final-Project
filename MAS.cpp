#include <iostream>
#include <vector>
#include <limits.h>

#include "matt/processInput.h"
#include "solutionScore.cpp"
#include "solver_josh.cpp"
#include "solver_matt.cpp"
// #include "solver_brandon.cpp"

using namespace std;

int main(int argc, char *argv[]){
  //For now, will have to get called for each instance.

      // if (argc != 2) {
      //     cout<<"Need to supply instance filename"<<endl;
      //     exit(1);
      // }


      AdjMatrix objectMatrix (argv[1]);
      AdjList objectList (argv[1]);
      int maxScore = INT_MIN;
      //All ranks stored here
      vector<vector<int> > ranks;

      //All other rankings will be added accordingly
      ranks.push_back(solve_instance_josh(objectMatrix, objectList));
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

      return 0;

}
