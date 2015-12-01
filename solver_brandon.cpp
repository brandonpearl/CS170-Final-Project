#ifndef _io
#define _io
#include <iostream>
#include <assert.h>
#include <stdio.h>
#endif
#ifndef _util
#define _util
// #include "solutionScore.cpp"
#include <vector>
#include <stdlib.h>
#endif
#include <time.h>
using namespace std;

class IMC {
	/*
	 * Simple container class for greedyInMinusOut.
	 */
public:
	IMC(int player, int value) {p = player; v = value;}
	int p;
	int v;
};

void swap(vector<int> rankStart, AdjMatrix adj, int swapAttempts) {
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
	for (int i=1; i<=returnVector.size(); i++) {
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

vector<int> frugalRoutine() {

}

vector<int> frugalDynamic(AdjMatrix matrix, AdjList list, int subSize) {


	
}

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

vector<int> greedyInMinusOut(AdjMatrix matrix) {
	/*
	 * Sorts the nodes based upon the least to greatest ordering of indegree minus outdegree.
	 */
	vector<IMC> sort;
	sort.reserve(matrix.getSize());
	set<int> empty;
	for (int i=0; i<matrix.getSize(); i++) {
		sort.push_back(IMC(i+1, matrix.countIn(i, empty) - matrix.countOut(i, empty)));
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
			int node = S.pop()-1;
			L.push_back(node+1);
			for (int i=0; i<copy.size(); i++) {
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

vector<int> solve_instance_brandon(AdjMatrix matrix, AdjList list) {
	
}

int brandon_solver_test_main() {
	return 0;
}
