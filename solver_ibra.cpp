#include <iostream>
#include <vector>

using namespace std;

void swap_vertices(int x, int y, vector<int> &arr){
    int temp = arr[x];
    arr[x] = arr[y];
    arr[y] = temp;
}

int max_ibra(int x, int y, int z){
  int temp = MAX_VAL(x,y);
  if (z > temp) return z;
  return temp;
}
// void add_elements_to_end(int startIndex, vector<int> from_arr, vector<int> &to_arr){
//     for (int i = startIndex; i < from_arr.size(); i++) to_arr.push_back(from_arr[i]);
// }
//
// void add_elements_from_start(int endIndex, vector<int> from_arr, vector<int> &to_arr){
//   for (int i = 0; i <= endIndex; i++) to_arr.push_back(from_arr[i]);
// }

vector<int> combo(vector<int> left, vector<int> right){
  vector<int> result;
  for (int i = 0; i < left.size(); i++){
    result.push_back(left.at(i));
  }
  for (int j = 0; j < right.size(); j++){
    result.push_back(left.at(j));
  }
  return result;
}

void brute_force_permute_ibra(int *inpt, int size, AdjList list, vector<vector<int> > &results) {
        vector<int> vect(inpt, inpt + size);
        if (size == 0) return;
        else {
          for (int i = size - 1; i >= 0; i--){
            swap_vertices(i, size - 1, vect);
            brute_force_permute_ibra(inpt, size - 1, list, results);
            swap_vertices(i, size - 1, vect);
          }
        }
        return;
}

vector<int> give_max_ordering(vector<int> left_elements, vector<int> right_elements, vector<int> normal, AdjList adj){

  vector<int> result;
  //left then right
  vector<int> option_uno = combo(left_elements, right_elements);
  //right then left
  vector<int> option_dos = combo(right_elements, left_elements);

  int option_uno_score = scoreSolution(option_uno, adj);
  int option_dos_score = scoreSolution(option_dos, adj);
  int normal_score = scoreSolution(normal, adj);
  int max_score = max_ibra(option_uno_score, option_dos_score, normal_score);
  if (option_uno_score == max_score){
    result = option_uno;
  } else if (option_dos_score == max_score){
    result = option_dos;
  } else {
    result = normal;
  }
  return result;
}



vector<int> solve_instance_ibra(AdjMatrix matrix, AdjList list){

  bool split_vertices_by_half = false;

  int num_vertices = matrix.getSize();

  if (num_vertices % 2 == 0) split_vertices_by_half = true;
  vector<vector<int> > ranks;
  vector<vector<int> > left_ranks;
  vector<vector<int> > right_ranks;
  int vertices[num_vertices];
  initialize_vertex_array(vertices, num_vertices);
  int initial_score = score_ordering(vertices, list);
  int current_score = initial_score;
  vector<int> naive(vertices, vertices + num_vertices);
  printf("\nStarting solver_ibra\n");
  printf("Obtaining all results: %d\n", current_score);

  if (num_vertices <= 50){
    brute_force_permute_ibra(vertices, num_vertices, list, ranks);
  } else {
    if (split_vertices_by_half){
      int left_vertices[num_vertices/2];
      int right_vertices[num_vertices/2];
      initialize_vertex_array(left_vertices, num_vertices/2);
      initialize_vertex_array(right_vertices, num_vertices/2);

      brute_force_permute_ibra(left_vertices, num_vertices/2, list, left_ranks);
      brute_force_permute_ibra(right_vertices, num_vertices/2, list, right_ranks);
    } else {
      int left_vertices[num_vertices/2 - 1];
      int right_vertices[num_vertices/2 + 1];
      initialize_vertex_array(left_vertices, num_vertices/2 - 1);
      initialize_vertex_array(right_vertices, num_vertices/2 + 1);

      brute_force_permute_ibra(left_vertices, num_vertices/2 - 1, list, left_ranks);
      brute_force_permute_ibra(right_vertices, num_vertices/2 + 1, list, right_ranks);
    }

      for (int i = 0; i < left_ranks.size(); i++){
        for (int j = 0; j < right_ranks.size(); j++){
          ranks.push_back(give_max_ordering(left_ranks[i], right_ranks[j], naive, list));
        }
      }

  }

  //score results
  int max_score = 0;
  vector<int> best_rank;
  for(int k = 0; k < ranks.size(); k++){
    current_score = scoreSolution(ranks.at(k), list);
    if (current_score > max_score){
      best_rank = ranks.at(k);
      max_score = current_score;
    }
  }

  return best_rank;
}

//testing
int main (int argc, char *argv[]) {
    if (argc != 2) {
        printf("Need to supply instance filename\n");
        exit(1);
    }

    AdjMatrix matrix = AdjMatrix(argv[1]);
    AdjList list = AdjList(argv[1]);
    vector<int> best = solve_instance_ibra(matrix, list);
    printf("\nsolver_ibra's best instance: [");

    for(int v = 0; v < best.size(); v++){
      cout<<best.at(v);
    }
    cout<<"]"<< endl;

    return 0;
}
