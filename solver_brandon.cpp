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

vector<int> frugalRoutine() {

}

vector<int> frugalDynamic(AdjMatrix matrix, AdjList list, int subSize) {


	
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

vector<int> solve_instance_brandon(AdjMatrix matrix, AdjList list) {
	
}

int brandon_solver_test_main() {
	return 0;
}
