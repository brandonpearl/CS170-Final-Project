#ifndef _io
#define _io
#include <iostream>
#include <assert.h>
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
inline int min(int a, int b) {if (a>b) {return a;} else {return b;}}

int scoreSolution(int rank[], AdjList adj) {
	int size = sizeof(rank)/sizeof(int);
	assert(size == adj.getSize());
	vector<bool> hasSeen (size, false);
	int score = 0;
	for (int i=0; i<size; i++) {
		int node = min(rank[i] - 1, size-1);
		hasSeen[node] = true;
		set<int> adjacent = adj.allEdges(i);
		for (int j=0; j<size; j++) {
			if (!hasSeen[j]) {
				score++;
			}
		}
	}
	for (int i=0; i<size; i++) {
		if (!hasSeen[i]) {
			cout << "Ranking does not include all nodes." << endl;
			exit(0);
		}
	}
	return score;
}

int main() {
	return 0;
}
