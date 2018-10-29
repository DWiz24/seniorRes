#ifndef COREH
#define COREH
#include <bits/stdc++.h>

struct Edge {
	int s;
	int d;
	int time;
	int dist;
	int cars;
};

struct Node {
	int id;
	std::vector<Edge> adj;
	int x;
	int y;
	long long dijkD;
};
extern Node* gr;
extern int nodes;

struct QE {
	void* val;
	long long t;
};

struct PQ {
	QE* a=0;
	int sz=0;
	int maxsz=-1;
};

void deletePQ(PQ& p);
void initPQ(PQ& p, int maxnum);

void push(PQ& p, long long t, void* v);

QE pop(PQ& p);

long long dijkstra(int start, int end);

void runSim(long long maxT);

#endif
