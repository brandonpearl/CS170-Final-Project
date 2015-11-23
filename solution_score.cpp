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

int scoreSolution(int rank[], adjList adj) {
	assert(sizeof(rank)/sizeof(int) == adj.size);
	vector<bool> hasSeen (adj.size, false);
	int score = 0;
	for (int i=0; i<sizeof(rank)/sizeof(int); i++) {
		int node = rank[i] - 1;
		hasSeen[node] = true;
		set<int> adjacent = adj.list[i];
		for (int j=0; j<sizeof(adjacent)/sizeof(int); j++) {
			if (!hasSeen[j]) {
				hasSeen[j] = true;
				score++;
			}
		}
	}
	for (int i=0; i<sizeof(hasSeen)/sizeof(bool); i++) {
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