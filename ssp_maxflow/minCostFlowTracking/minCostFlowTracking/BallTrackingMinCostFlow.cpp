//============================================================================
// Name        : BallTrackingMinCostFlow.cpp
// Author      : Boyang Gao
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

/*
 * Algorithm: Mincost flow
 * complexity: unknown
 */
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <cassert>
#include <algorithm>
#include <cfloat>
#include <vector>
using namespace std;

#define N 4000+2
#define GN 10000000
class ResidualEdge {
public:
	int cap;
	float cost;
};
class Node {
public:
	int nIn,nOut;
	vector<int> postNodes;
};
class DirectedEdge {
public:
	int from, to;
	float cost;
};

ResidualEdge g[N][N], wg[N][N];
int main() {
	char pathInFile[] = "second_graph.txt";
	char pathOutFile[] = "track_out.txt";
	Node nodes[N];

	int maxNodeId = 0;
	float cost;
	memset(nodes, 0, sizeof(nodes));
	int nEdge = 0;
	FILE * pFile = fopen(pathInFile, "r");
	int fromId,toId;
	while(fscanf(pFile, "%d,%d,%f", &fromId,&toId,&cost) != EOF) {
//		printf("%d %d %f\n", fromId, toId, cost);
		if (fromId > maxNodeId) {
			maxNodeId = fromId;
		} else {}
		nodes[fromId].nOut++;
		if (toId > maxNodeId) {
			maxNodeId = toId;
		} else {}
		nodes[toId].nIn++;
		g[fromId][toId].cap = 1;
		g[fromId][toId].cost = cost;
		g[toId][fromId].cap = 0;
		g[toId][fromId].cost = -cost;
		nodes[fromId].postNodes.push_back(toId);
		nodes[toId].postNodes.push_back(fromId);
		nEdge++;
	}
	fclose(pFile);
	printf("nEdge: %d\n", nEdge);
	int nNode = maxNodeId;
	printf("nNode: %d\n", nNode);
	vector<DirectedEdge> originalEdges;
	pFile = fopen(pathInFile, "r");
	while(fscanf(pFile, "%d,%d,%f", &fromId,&toId,&cost) != EOF) {
		DirectedEdge dEdge;
		dEdge.from = fromId;
		dEdge.to = toId;
		dEdge.cost = cost;
		originalEdges.push_back(dEdge);
	}
	fclose(pFile);
	int ss = 0;
	int t = nNode + 1;
	assert(t < N);
	for(int i = 1; i <= nNode; i++) {
		g[i][i].cap = GN;
		if(nodes[i].nIn == 0) {
			g[ss][i].cap = 1;
			g[ss][i].cost = 0.f;
			g[i][ss].cap = 0;
			g[i][ss].cost = 0.f;
			nodes[ss].postNodes.push_back(i);
			nodes[i].postNodes.push_back(ss);
			DirectedEdge dEdge;
			dEdge.from = ss;
			dEdge.to = i;
			dEdge.cost = 0.f;
			originalEdges.push_back(dEdge);
		} else {}
		if(nodes[i].nOut == 0) {
			g[i][t].cap = 1;
			g[i][t].cost = 0.f;
			g[t][i].cap = 0;
			g[t][i].cost = 0.f;
			nodes[i].postNodes.push_back(t);
			nodes[t].postNodes.push_back(i);
			DirectedEdge dEdge;
			dEdge.from = i;
			dEdge.to = t;
			dEdge.cost = 0.f;
			originalEdges.push_back(dEdge);
		} else {}
	}
	g[ss][t].cap = 0;
	g[t][ss].cap = 0;
	memcpy(wg, g, sizeof(g));
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
			//printf("%d ", mi);
			tag[mi] = true;
			tmi = -1;
			for(int iPostNode = 0; iPostNode < nodes[mi].postNodes.size(); iPostNode++) {
				int i = nodes[mi].postNodes[iPostNode];
				if (tag[i] || wg[mi][i].cap < 1) {
					continue;
				} else {
					float tmp;
					if ((tmp = shortestDist[mi] + wg[mi][i].cost)
							< shortestDist[i]) {
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
		//printf("\n");
		if (from[t] < 0)
			break;	//Maxflow achieves
		for(int i = 0; i < originalEdges.size(); i++) {
			DirectedEdge dEdge = originalEdges[i];
			wg[dEdge.from][dEdge.to].cost += shortestDist[dEdge.from] - shortestDist[dEdge.to];
			wg[dEdge.to][dEdge.from].cost += shortestDist[dEdge.to] - shortestDist[dEdge.from];
		}
		int b = GN;
		for (int j = t, i = from[t]; j; j = i, i = from[i]) {
			if (b > wg[i][j].cap)
				b = wg[i][j].cap;
		}
		for (int j = t, i = from[t]; j; j = i, i = from[i]) {
			wg[i][j].cap -= b;
			wg[j][i].cap += b;
			//if (fabs(wg[j][i].cost) > 1e-4) {
			//	printf("%d %d: %f\n", j,i,wg[j][i].cost);
			//}
			//printf("%d->", j);
		}
		//printf("0\n");
		nowFlow += b;
		//printf("flow: %d\n", nowFlow);
	}
	int nInitNode = 0;
	int nRearNode = 0;
	for (int i = 1; i <= nNode; i++) {
		if (nodes[i].nIn == 0) {
			printf("suorceNode: %d\n", i);
			nInitNode++;
		} else {}
		if (nodes[i].nOut == 0) {
			printf("sinkNode: %d\n", i);
			nRearNode++;
		} else {}
	}
	printf("nSourceNode: %d\n", nInitNode);
	printf("nSinkNode: %d\n", nRearNode);
	printf("max flow: %d\n", nowFlow);

	freopen(pathOutFile, "w", stdout);
	printf("%d\n", nowFlow);
	int flowPath[N];
	for (int j = 0; j < nowFlow; j++) {
		flowPath[0] = ss;
		int nEdgeInFlowPath = 1;
		for (int mi = ss; mi != t; nEdgeInFlowPath++) {
			int l = -1;
			for(int i = 0; i < nodes[mi].postNodes.size(); i++) {
				int next = nodes[mi].postNodes[i];
				if(wg[mi][next].cap < g[mi][next].cap) {
					l = next;
					break;
				} else {}
			}
			assert(l > 0);
			flowPath[nEdgeInFlowPath] = l;
			wg[mi][l].cap ++;
			wg[l][mi].cap --;
			mi = l;
		}
//		printf("nEdgeInFlowPath: %d\n", nEdgeInFlowPath);
//		printf("0");
		printf("%d", flowPath[1]);
		for(int i = 2; i < nEdgeInFlowPath-1; i++) {
			printf(" %d", flowPath[i]);
		}
		printf("\n");
	}
	return 0;
}
