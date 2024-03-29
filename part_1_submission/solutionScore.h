#ifndef _io
#define _io
#include <iostream>
#include <assert.h>
#include <stdio.h>
#endif
#ifndef _vector
#define _vector
#include <vector>
#endif
#ifndef _adj
#define _adj
#include "matt/processinput.h"
#endif
using namespace std;
inline int min(int a, int b) {if (a>b) {return b;} else {return a;}}
inline int max(int a, int b) {if (a>b) {return a;} else {return b;}}

int scoreSolutionGeneral(vector<int> rank, AdjList adj, int size, bool fullSolution) {
	if (fullSolution) {
		assert(size == adj.getSize());
	}
	vector<bool> hasSeen (size, false);
	int score = 0;

	for (int i=0; i<size; i++) {
		int node = min(rank[i] - 1, size-1);
		hasSeen[node] = true;
		set<int> adjacent = adj.allEdges(node);
		for (set<int>::iterator j=adjacent.begin(); j!=adjacent.end(); ++j) {
			if (!hasSeen[*j]) {
				score++;
			}
		}
	}
	for (int i=0; fullSolution && i<size; i++) {
		assert(hasSeen[i]);
	}
	return score;
}

int scorePartialSolution(vector<int> rank, AdjList adj, int size) {
	return scoreSolutionGeneral(rank, adj, size, false);
}

int scoreSolution(vector<int> rank, AdjList adj) {
	return scoreSolutionGeneral(rank, adj, rank.size(), true);
}


/*
Changed name to test_main so this file can be included in another file that has a
main function
*/
int brandon_test_main(int argc, char *argv[]) {
	int size;
	if (argc > 1) {
		size = atof(argv[1]);
	} else {
		size = 10;
	}
	AdjList lst = AdjList(size);
	vector<int> a;
	a.reserve(size);
	for (int i=1; i<=size; i++) {
		a.push_back(i);
	}
	cout << scoreSolution(a, lst) << endl;
	return 0;
}
