//============================================================================
// Name        : BallTrackingMinCostFlowAdjList.cpp
// Author      : Boyang Gao
// Version     :
// Copyright   : GBY
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <cassert>
#include <algorithm>
#include <cfloat>
#include <vector>
#include <map>
using namespace std;

#define N 600
#define GN 10000000
class ResidualEdge {
public:
	int cap;
	int initCap;
	float cost;
	ResidualEdge() {
		this->cap = -1;
		this->initCap = -1;
		this->cost = -FLT_MAX;
	}
	ResidualEdge(int cap, int iniCap, float cost) {
		this->cap = cap;
		this->initCap = iniCap;
		this->cost = cost;
	}
};
class Node {
public:
	bool isAppear;
	int nIn,nOut;
	map<int, ResidualEdge> edge;
	Node() {
		this->nIn = 0;
		this->nOut = 0;
		this->isAppear = false;
	}
};
class DirectedEdge {
public:
	int from, to;
	float cost;
	DirectedEdge(int from, int to, float cost) {
		this->from = from;
		this->to = to;
		this->cost = cost;
	}
};

int main() {
	char pathInFile[] = "forBoyang.txt";
	char pathOutFile[] = "track_out.txt";
	Node nodes[N];

	int maxNodeId = 0;
	float cost;
	int nEdge = 0;
	FILE * pFile = fopen(pathInFile, "r");
	int fromId,toId;
	vector<DirectedEdge> originalEdges;
	while(fscanf(pFile, "%d,%d,%f", &fromId,&toId,&cost) != EOF) {
//		printf("%d %d %f\n", fromId, toId, cost);
		if (fromId > maxNodeId) {
			maxNodeId = fromId;
		} else {}
		nodes[fromId].nOut++;
		nodes[fromId].isAppear = true;
		if (toId > maxNodeId) {
			maxNodeId = toId;
		} else {}
		nodes[toId].nIn++;
		nodes[toId].isAppear = true;
		ResidualEdge edgeA(1, 1, cost);
		ResidualEdge edgeB(0, 0, -cost);
		nodes[fromId].edge[toId] = edgeA;
		nodes[toId].edge[fromId] = edgeB;
		DirectedEdge dEdge(fromId, toId, cost);
		originalEdges.push_back(dEdge);
		nEdge++;
	}
	fclose(pFile);
	printf("nEdge: %d\n", nEdge);
	int nNode = maxNodeId;
	printf("nNode: %d\n", nNode);
	int ss = 0;
	int t = nNode + 1;
	assert(t < N);
	for(int i = 1; i <= nNode; i++) {
		if (nodes[i].isAppear) {
			if (nodes[i].nIn == 0) {
				ResidualEdge edgeA(1, 1, 0.f);
				ResidualEdge edgeB(0, 0, 0.f);
				nodes[ss].edge[i] = edgeA;
				nodes[i].edge[ss] = edgeB;
				DirectedEdge dEdge(ss, i, 0.f);
				originalEdges.push_back(dEdge);
			} else {}
			if (nodes[i].nOut == 0) {
				ResidualEdge edgeA(1, 1, 0.f);
				ResidualEdge edgeB(0, 0, 0.f);
				nodes[i].edge[t] = edgeA;
				nodes[t].edge[i] = edgeB;
				DirectedEdge dEdge(i, t, 0.f);
				originalEdges.push_back(dEdge);
			} else {}
		} else {}
	}
	float shortestDist[N];
	bool tag[N];
	int from[N];
	int nowFlow = 0;
	while (true) {
		memset(tag, false, sizeof(tag));
		for (int i = 1; i <= nNode+1; i++)
			shortestDist[i] = FLT_MAX;
		shortestDist[ss] = 0.f;
		int tmi, mi;
		tmi = mi = ss;
		from[t] = -1;
		from[ss] = 0;
		while (tmi >= 0) {
			mi = tmi;
			printf("%d ", mi);
			tag[mi] = true;
			tmi = -1;
			for(map<int, ResidualEdge>::iterator it = nodes[mi].edge.begin(); it != nodes[mi].edge.end(); it++) {
				int i = it->first;
				if(tag[i] || it->second.cap < 1) {
					continue;
				} else {
					float tmp;
					if((tmp = shortestDist[mi] + it->second.cost) < shortestDist[i]) {
						shortestDist[i] = tmp;
						from[i] = mi;
					} else {}
				}
			}
			for(int i = 1; i <= nNode + 1; i++) {
				if(tag[i]) {
					continue;
				} else {
					if (tmi < 0 || shortestDist[tmi] > shortestDist[i]) {
						tmi = i;
					} else {}
				}
			}
		}
		printf("\n");
		if (from[t] < 0)
			break;	//Maxflow achieves
		for(size_t i = 0; i < originalEdges.size(); i++) {
			DirectedEdge dEdge = originalEdges[i];
			nodes[dEdge.from].edge[dEdge.to].cost += shortestDist[dEdge.from] - shortestDist[dEdge.to];
			nodes[dEdge.to].edge[dEdge.from].cost += shortestDist[dEdge.to] - shortestDist[dEdge.from];
		}
		int b = GN;
		for (int j = t, i = from[t]; j; j = i, i = from[i]) {
			int v = nodes[i].edge[j].cap;
			if (b > v) {
				b = v;
			} else {}
		}
		for (int j = t, i = from[t]; j; j = i, i = from[i]) {
			nodes[i].edge[j].cap -= b;
			nodes[j].edge[i].cap += b;
//			if (fabs(wg[j][i].cost) > 1e-4) {
//				printf("%d %d: %f\n", j,i,wg[j][i].cost);
//			}
			printf("%d->", j);
		}
		printf("0\n");
		nowFlow += b;
		printf("flow: %d\n", nowFlow);
	}
	int nInitNode = 0;
	int nRearNode = 0;
	for (int i = 1; i <= nNode; i++) {
		if (nodes[i].isAppear) {
			if (nodes[i].nIn == 0) {
//			printf("sourceNode: %d\n", i);
				nInitNode++;
			} else {}
			if (nodes[i].nOut == 0) {
//			printf("sinkNode: %d\n", i);
				nRearNode++;
			} else {}
		} else {}
	}
	printf("nSourceNode: %d\n", nInitNode);
	printf("nSinkNode: %d\n", nRearNode);
	printf("max flow: %d\n", nowFlow);

	FILE * fpOut = fopen(pathOutFile, "w");
	fprintf(fpOut, "%d\n", nowFlow);
	int flowPath[N];
	for (int j = 0; j < nowFlow; j++) {
		flowPath[0] = ss;
		int nEdgeInFlowPath = 1;
		for (int mi = ss; mi != t; nEdgeInFlowPath++) {
			int l = -1;
			for(map<int, ResidualEdge>::iterator it = nodes[mi].edge.begin(); it != nodes[mi].edge.end(); it++) {
				int next = it->first;
				if(it->second.cap < it->second.initCap) {
					l = next;
					break;
				} else {}
			}
			assert(l > 0);
			flowPath[nEdgeInFlowPath] = l;
			nodes[mi].edge[l].cap += 1;
			nodes[l].edge[mi].cap -= 1;
			mi = l;
		}
//		printf("nEdgeInFlowPath: %d\n", nEdgeInFlowPath);
//		printf("0");
		fprintf(fpOut, "%d", flowPath[1]);
		for(int i = 2; i < nEdgeInFlowPath-1; i++) {
			fprintf(fpOut, " %d", flowPath[i]);
		}
		fprintf(fpOut, "\n");
	}
	fclose(fpOut);
	return 0;
}

