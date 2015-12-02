#ifndef _io
#define _io
#include <iostream>
#include <assert.h>
#include <stdio.h>
#endif
#ifndef _util
#define _util
#include "solutionScore.h"
#include <stdlib.h>
#include <limits>
#endif
#ifndef _vector
#define _vector
#include <vector>
#endif
#ifndef _adj
#define _adj
#include "matt/processinput.h"
#endif
#include <time.h>
#include <math.h>
using namespace std;

int swapAttempts = 10000;
float pointless = 1e-7;

class IMC {
	/*
	 * Simple container class for greedyInMinusOut.
	 */
public:
	IMC(int player, int value) {p = player; v = value;}
	int p;
	float v;
};

void swap(vector<int> rankStart, AdjList adj, int swapAttempts) {
	/*
	 * Attempt a number of swaps equal to swapAttempts to improve the score
	 * of rankStart.
	 */ 
	srand(time(NULL));
	int prevScore = scoreSolution(rankStart, adj);
	for (int i=0; i<swapAttempts; i++) {
		int index1 = 0;
		int index2 = 0;
		while (index1 == index2) {
			index1 = rand() % rankStart.size();
			index2 = rand() % rankStart.size();
		}
		int temp = rankStart[index1];
		rankStart[index1] = rankStart[index2];
		rankStart[index2] = temp;
		int testScore = scoreSolution(rankStart, adj);
		if (testScore > prevScore) {
			prevScore = testScore;
		} else {
			temp = rankStart[index1];
			rankStart[index1] = rankStart[index2];
			rankStart[index2] = temp;
		}
	}
}

vector<int> makeRandomOrder(int elements) {
	/*
	 * Generate a random ordering of S = {1...elements}.
	 * Makes use of Fisher-Yates algorithm.
	 */
	srand(time(NULL));
	vector<int> returnVector;
	returnVector.reserve(elements);
	for (int i=1; i<=elements; i++) {
		returnVector.push_back(i);
	}
	for (int i=returnVector.size()-1; i>0; i--) {
		int j = rand() % i;
		int temp = returnVector[i];
		returnVector[i] = returnVector[j];
		returnVector[j] = temp;
	}
	return returnVector;
}

/*vector<int> frugalRoutine() {

}*/

/*vector<int> frugalDynamic(AdjMatrix matrix, AdjList list, int subSize) {


	
}*/

vector<IMC> mergeSort(vector<IMC> list, int first, int last) {
	/*
	 * Basic merge sort algorithm in which IMC instances hold both player id's and values.
	 */
	if (last-first == 0) {
		vector<IMC> ret;
		ret.push_back(list[first]);
		return ret;
	} else if (last-first == 1) {
		vector<IMC> ret;
		ret.reserve(2);
		if (list[first].v >= list[last].v) {
			ret.push_back(list[last]);
			ret.push_back(list[first]);
		} else {
			ret.push_back(list[first]);
			ret.push_back(list[last]);
		}
		return ret;
	} else {
		int half = int((first+last)/2);
		vector<IMC> left = mergeSort(list, first, half);
		vector<IMC> right = mergeSort(list, half+1, last);
		vector<IMC> returnVector;
		returnVector.reserve(left.size()+right.size());
		int i = 0;
		int j = 0;
		while (i < left.size() || j < right.size()) {
			if (j >= right.size()) {
				returnVector.push_back(left[i]);
				i++;
			} else if (i >= left.size()) {
				returnVector.push_back(right[j]);
				j++;
			} else if (left[i].v <= right[j].v) {
				returnVector.push_back(left[i]);
				i++;
			} else {
				returnVector.push_back(right[j]);
				j++;
			}

		}
		return returnVector;
	}
}

bool find(vector<int> list, int val) {
	for (int i=0; i<list.size(); i++) {
		if (list[i] == val) {
			return true;
		}
	}
	return false;
}

vector<int> bruteForce(vector<int> avoid, AdjList list) {
	assert(list.getSize() <= 20);
	if (avoid.size() == list.getSize()) {
		return avoid;
	}
	vector<int> result;
	vector<int> bestSolution;
	int bestScore = numeric_limits<int>::min();
	for (int i=1; i<=list.getSize(); i++) {
		if (find(avoid, i)) {
			continue;
		}
		vector<int> start (avoid);
		start.push_back(i);
		result = bruteForce(start, list);
		int testScore = scoreSolution(result, list);
		if (testScore > bestScore) {
			bestScore = testScore;
			bestSolution = result;
		}
		result.clear();
	}
	return bestSolution;
}

vector<int> greedyInMinusOut(AdjMatrix matrix) {
	/*
	 * Sorts the nodes based upon the least to greatest ordering of indegree minus outdegree.
	 */
	vector<IMC> sort;
	sort.reserve(matrix.getSize());
	set<int> empty;
	for (int i=0; i<matrix.getSize(); i++) {
		sort.push_back(IMC(i+1, (float)matrix.countIn(i, empty) - (float)matrix.countOut(i, empty)));
	}
	sort = mergeSort(sort, 0, sort.size()-1);
	vector<int> returnVector;
	returnVector.reserve(sort.size());
	for (int i=0; i<sort.size(); i++) {
		returnVector.push_back(sort[i].p);
	}
	return returnVector;
}

vector<int> topologicalSort(AdjMatrix matrix) {
	/*
	 * Attempt to find optimal solution in a topological sort. If found, we will return
	 * the vector with all elements in their topological sort. If not all edges have been
	 * removed from the graph copy, we clear the L vector and return said empty vector.   
	 */
	AdjMatrix copy = matrix.duplicate();
	vector<int> L;
	L.reserve(copy.getSize());
	vector<int> S;
	set<int> empty;
	for (int i=0; i<copy.getSize(); i++) {
		if (copy.countIn(i, empty) == 0) {
			S.push_back(i+1);
		}
	}
	if (S.size() == 0) {
		return L;
	} else {
		while (S.size() != 0) {
			int node = S.back()-1;
			S.pop_back();
			L.push_back(node+1);
			for (int i=0; i<copy.getSize(); i++) {
				if (copy.edgeExists(node, i) && node != i) {
					copy.removeEdge(node, i);
					if (copy.countIn(i, empty) == 0) {
						S.push_back(i+1);
					}
				}
			}
		}
	}
	if (copy.hasEdges()) {
		L.clear();
		return L;
	} else {
		return L;
	}
}

float powBrandon(float base, int power) {
	if (base == 0) {
		return 0.0;
	} else {
		return pow(base, (float)power);
	}
}

vector<int> matrixAdjacency(AdjMatrix matrix, int power) {
	assert(power == 1 || power == -1);
	vector<AdjMatrix> matrices;
	matrices.reserve(matrix.getSize());
	matrices.push_back(matrix);
	for (int i=1; i<matrix.getSize(); i++) {
		matrices.push_back(matrices.back().multiplyMatrix(matrix));
	} 
	vector<IMC> scores;
	scores.reserve(matrix.getSize());
	for (int i=0; i<matrix.getSize(); i++) {
		float score = 0;
		for (int j=0; j<matrices.size(); j++) {
			score = score - powBrandon(j, power)*(float)(matrices[j].rowSum(i));
		}
		scores.push_back(IMC(i+1, score));
	}
	vector<IMC> result = mergeSort(scores, 0, scores.size()-1);
	vector<int> returnVector;
	returnVector.reserve(result.size());
	for (int i=0; i<result.size(); i++) {
		returnVector.push_back(result[i].p);
	}
	return returnVector;

}

vector<int> solve_instance_brandon(AdjMatrix matrix, AdjList list) {
	vector<int> best;
	if (matrix.getSize() <= 9) {
		return bruteForce(best, list);
	}
	vector<int> random = makeRandomOrder(matrix.getSize());
	int currentScore = scoreSolution(random, list);
	float percentChange = numeric_limits<float>::max();
	int stagnant = 0;
	cout << "Beginning Random Swaps" << endl;
	while (abs(percentChange) > pointless && stagnant <= 100) {
		swap(random, list, swapAttempts);
		int nextScore = scoreSolution(random, list);
		percentChange = float(((float)nextScore - (float)currentScore)/(float)currentScore);
		if (percentChange <= pointless) {
			stagnant++;
		} else {
			stagnant = 0;
		}
		currentScore = nextScore;
	}
	cout << "Attempting Topological Sort" << endl;
	vector<int> topological = topologicalSort(matrix);
	cout << "Beginning Greedy Approximation" << endl;
	vector<int> greedy = greedyInMinusOut(matrix);
	cout << "Beginning Adjacency Approximation" << endl;
	vector<int> adjacency1 = matrixAdjacency(matrix, 1);
	vector<int> adjacency2 = matrixAdjacency(matrix, -1);
	if (topological.size() != 0) {
		return topological;
	}
	vector<vector<int> > solutionCandidates;
	solutionCandidates.push_back(random);
	solutionCandidates.push_back(greedy);
	solutionCandidates.push_back(adjacency1);
	solutionCandidates.push_back(adjacency2);
	int bestScore = numeric_limits<int>::min();
	vector<int> bestSolution;
	for (int i=0; i<solutionCandidates.size(); i++) {
		int testScore = scoreSolution(solutionCandidates[i], list);
		cout << "Comparing Score of: " << testScore << " to current max of: " << bestScore << endl;
		if (testScore > bestScore) {
			bestScore = testScore;
			bestSolution = solutionCandidates[i];
		}
	}
	swap(bestSolution, list, swapAttempts);
	return bestSolution;

}

int main(int argc, char *argv[]) {
	AdjMatrix matrix (argv[1]);
    AdjList list (argv[1]);

    vector<int> solution = solve_instance_brandon(matrix, list);
    cout << scoreSolution(solution, list);
    return 0;
}
