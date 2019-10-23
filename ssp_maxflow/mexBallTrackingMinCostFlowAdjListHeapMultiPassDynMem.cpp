//============================================================================
// Name        : BallTrackingMinCostFlowAdjListHeapMultiPass.cpp
// Author      : Boyang Gao
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================
// mex mexBallTrackingMinCostFlowAdjListHeapMultiPass.cpp -largeArrayDims
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <cassert>
#include <algorithm>
#include <cfloat>
#include <vector>
#include <map>
#include <string>
#include <mex.h>
using namespace std;

//#define N 13000
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
	int nIn, nOut;
	map<int, ResidualEdge> edge;
	Node() {
		this->nIn = 0;
		this->nOut = 0;
		this->isAppear = true;
	}
	void addEdge(int iNode, ResidualEdge & re) {
		this->nOut++;
		this->edge[iNode] = re;
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

class HeapElement {
public:
	int nodeId;
	float shortestDistance;
	HeapElement() {
		this->nodeId = -1;
		this->shortestDistance = FLT_MAX;
	}
	HeapElement(int nodeId, float shortestDistance) {
		this->nodeId = nodeId;
		this->shortestDistance = shortestDistance;
	}
	bool operator<(const HeapElement& rhs) const {
		return this->shortestDistance > rhs.shortestDistance;
	}
};

class Graph {
	Node * nodes;
	int nNode;
	int ss;
	int t;
	int nowFlow;
	vector<DirectedEdge> originalEdges;
//	vector<std::string> resultTrack;
	vector<mxArray*> resultTrack4Matlab;
public:
	Graph() {
		this->nNode = -1;
		this->ss = -1;
		this->t = -1;
		this->nowFlow = -1;
	}
	int initGraph(const mxArray *array_ptr);
	int minCostFlow();
	int saveFlowPathTagDeleteNode();
	int deleteFlow();
	mxArray * outputTrack();
	bool isFinish() {
		return (nodes[ss].nOut <= 0);
	}
	~Graph() {
		delete [] nodes;
	}
private:
	int connectEdge(int from, int to, ResidualEdge & forwardEdge,
			ResidualEdge & reverseEdge) {
		nodes[from].edge[to] = forwardEdge;
		nodes[from].nOut++;
		nodes[to].nIn++;
		nodes[to].edge[from] = reverseEdge;
		return 0;
	}
	int disconnectEdge(int from, int to) {
		nodes[from].edge.erase(to);
		nodes[from].nOut--;
		nodes[to].nIn--;
		nodes[to].edge.erase(from);
		return 0;
	}
};

int Graph::initGraph(const mxArray *array_ptr) {
	int maxNodeId = 0;
	double *pr, *pi;
	mwIndex *ir, *jc;
	mwSize col;
	mwIndex starting_row_index, stopping_row_index, current_row_index;
	mwSize n;

	/* Get the starting positions of all four data arrays. */
	pr = mxGetPr(array_ptr);
	ir = mxGetIr(array_ptr);
	jc = mxGetJc(array_ptr);

	/* Display the nonzero elements of the sparse array. */
	n = mxGetN(array_ptr);
	for (col = 0; col < n; col++) {
		starting_row_index = jc[col];
		stopping_row_index = jc[col + 1];
		if (starting_row_index == stopping_row_index)
			continue;
		else {
			for (current_row_index = starting_row_index;
					current_row_index < stopping_row_index;
					current_row_index++) {
//				mexPrintf("\t(%"FMT_SIZE_T"u,%"FMT_SIZE_T"u) = %g\n",
//					ir[current_row_index]+1,
//					col+1, pr[current_row_index]);
				int fromId = ir[current_row_index]+1;
				int toId = col+1;
				float cost = pr[current_row_index];
				if (fromId > maxNodeId) {
					maxNodeId = fromId;
				} else {
				}
		//		nodes[fromId].isAppear = true;
				if (toId > maxNodeId) {
					maxNodeId = toId;
				} else {
				}
		//		nodes[toId].isAppear = true;
//				ResidualEdge edgeA(1, 1, cost);
//				ResidualEdge edgeB(0, 0, -cost);
//				connectEdge(fromId, toId, edgeA, edgeB);
				DirectedEdge dEdge(fromId, toId, cost);
				originalEdges.push_back(dEdge);
			}
		}
	}
	printf("nEdge: %d\n", (int) originalEdges.size());
	nNode = maxNodeId;
	printf("nNode: %d\n", nNode);
	this->nodes = new Node[nNode+2];
	for(vector<DirectedEdge>::iterator it = originalEdges.begin(); it != originalEdges.end(); it++) {
		ResidualEdge edgeA(1, 1, it->cost);
		ResidualEdge edgeB(0, 0, -it->cost);
		connectEdge(it->from, it->to, edgeA, edgeB);
	}
	ss = 0;
	t = nNode + 1;
//	assert(t < N);
	for (int i = 1; i <= nNode; i++) {
//		if (nodes[i].isAppear) {
		if (nodes[i].nIn == 0) {
			ResidualEdge edgeA(1, 1, 0.f);
			ResidualEdge edgeB(0, 0, 0.f);
			connectEdge(ss, i, edgeA, edgeB);
			DirectedEdge dEdge(ss, i, 0.f);
			originalEdges.push_back(dEdge);
		} else {
		}
		if (nodes[i].nOut == 0) {
			ResidualEdge edgeA(1, 1, 0.f);
			ResidualEdge edgeB(0, 0, 0.f);
			connectEdge(i, t, edgeA, edgeB);
			DirectedEdge dEdge(i, t, 0.f);
			originalEdges.push_back(dEdge);
		} else {
		}
//		} else {}
	}
	printf("nSourceNode: %d\n", (int) nodes[ss].edge.size());
	printf("nSinkNode: %d\n", (int) nodes[t].edge.size());
	return 0;
}

int Graph::minCostFlow() {
	nowFlow = 0;
	float * shortestDist = new float[nNode + 2];
	bool  * tag          = new bool[nNode + 2];
	int   * from         = new int[nNode + 2];
	vector<HeapElement> heap(nNode + 2);
	while (true) {
		heap.clear();
		memset(tag, false, sizeof(bool)*(nNode+2));
		for (int i = 1; i <= nNode + 1; i++)
			shortestDist[i] = FLT_MAX;
		shortestDist[ss] = 0.f;
		from[t] = -1;
		from[ss] = 0;
		for (int tmi = ss; tmi >= 0;) {
			int mi = tmi;
//			printf("%d ", mi);
			tag[mi] = true;
			for (map<int, ResidualEdge>::iterator it = nodes[mi].edge.begin();
					it != nodes[mi].edge.end(); it++) {
				int i = it->first;
				if (tag[i] || it->second.cap < 1) {
					continue;
				} else {
					float tmp;
					if ((tmp = shortestDist[mi] + it->second.cost)
							< shortestDist[i]) {
						shortestDist[i] = tmp;
						from[i] = mi;
						HeapElement he(i, shortestDist[i]);
						heap.push_back(he);
						push_heap(heap.begin(), heap.end());
					} else {
					}
				}
			}
			while (heap.size() > 0 && tag[heap.front().nodeId]) {
				pop_heap(heap.begin(), heap.end());
				heap.pop_back();
			}
			if (heap.size() > 0) {
				tmi = heap.front().nodeId;
			} else {
				tmi = -1;
			}
		}
//		printf("\n");
		if (from[t] < 0)
			break;	//Maxflow achieves
		for (size_t i = 0; i < originalEdges.size(); i++) {
			DirectedEdge dEdge = originalEdges[i];
			nodes[dEdge.from].edge[dEdge.to].cost += shortestDist[dEdge.from]
					- shortestDist[dEdge.to];
			nodes[dEdge.to].edge[dEdge.from].cost += shortestDist[dEdge.to]
					- shortestDist[dEdge.from];
		}
		int b = GN;
		for (int j = t, i = from[t]; j; j = i, i = from[i]) {
			int v = nodes[i].edge[j].cap;
			if (b > v) {
				b = v;
			} else {
			}
		}
		for (int j = t, i = from[t]; j; j = i, i = from[i]) {
			nodes[i].edge[j].cap -= b;
			nodes[j].edge[i].cap += b;
//			if (fabs(wg[j][i].cost) > 1e-4) {
//				printf("%d %d: %f\n", j,i,wg[j][i].cost);
//			}
//			printf("%d->", j);
		}
//		printf("0\n");
		nowFlow += b;
		printf("flow: %d\n", nowFlow);
	}
	printf("max flow: %d\n", nowFlow);
	delete [] shortestDist;
	delete [] tag;
	delete [] from;
	return 0;
}

int Graph::saveFlowPathTagDeleteNode() {
	int * flowPath = new int[nNode + 2];
	for (int j = 0; j < nowFlow; j++) {
		flowPath[0] = ss;
		int nEdgeInFlowPath = 1;
		for (int mi = ss; mi != t; nEdgeInFlowPath++) {
			nodes[mi].isAppear = false;
			int l = -1;
			for (map<int, ResidualEdge>::iterator it = nodes[mi].edge.begin();
					it != nodes[mi].edge.end(); it++) {
				int next = it->first;
				if (it->second.cap < it->second.initCap) {
					l = next;
					break;
				} else {
				}
			}
			assert(l > 0);
			flowPath[nEdgeInFlowPath] = l;
			nodes[mi].edge[l].cap += 1;
			nodes[l].edge[mi].cap -= 1;
			mi = l;
		}
//		printf("nEdgeInFlowPath: %d\n", nEdgeInFlowPath);
//		printf("0");

//		char tmp[128];
//		sprintf(tmp, "%d", flowPath[1]);
//		std::string result = tmp;
//		for (int i = 2; i < nEdgeInFlowPath - 1; i++) {
//			sprintf(tmp, " %d", flowPath[i]);
//			result += tmp;
//		}
//		resultTrack.push_back(result);
		mwSize dims[1] = {nEdgeInFlowPath-2};
		mxArray * intArray = mxCreateNumericArray(1, dims, mxINT32_CLASS, mxREAL);
		int * intPtr = (int*) mxGetData(intArray);
		for (int i = 1; i < nEdgeInFlowPath - 1; i++) {
			intPtr[i-1] = flowPath[i];
		}
		resultTrack4Matlab.push_back(intArray);
	}
	delete [] flowPath;
	return 0;
}

int Graph::deleteFlow() {
	nodes[ss].isAppear = true;
	nodes[t].isAppear = true;
	vector<DirectedEdge> newOriginalEdges;
	for (vector<DirectedEdge>::iterator it = originalEdges.begin();
			it != originalEdges.end(); it++) {
		int from = it->from;
		int to = it->to;
		if (nodes[from].isAppear == true && nodes[to].isAppear == true) {
			newOriginalEdges.push_back(*it);
		} else {
			disconnectEdge(from, to);
			if (nodes[from].isAppear == false && nodes[to].isAppear == true) {
				if (nodes[to].nIn == 0 && to != t) {
					ResidualEdge edgeA(1, 1, 0.f);
					ResidualEdge edgeB(0, 0, 0.f);
					connectEdge(ss, to, edgeA, edgeB);
					DirectedEdge dEdge(ss, to, 0.f);
					newOriginalEdges.push_back(dEdge);
				} else {
				}
			} else if (nodes[from].isAppear == true
					&& nodes[to].isAppear == false) {
				if (nodes[from].nOut == 0 && from != ss) {
					ResidualEdge edgeA(1, 1, 0.f);
					ResidualEdge edgeB(0, 0, 0.f);
					connectEdge(from, t, edgeA, edgeB);
					DirectedEdge dEdge(from, t, 0.f);
					newOriginalEdges.push_back(dEdge);
				} else {
				}
			} else {
			}
		}
	}
	originalEdges.clear();
	originalEdges = newOriginalEdges;
	return 0;
}

mxArray * Graph::outputTrack() {
//	FILE * fpOut = fopen("mexTrackOut.txt", "w");
//	fprintf(fpOut, "%d\n", (int) resultTrack.size());
//	for (vector<std::string>::iterator it = resultTrack.begin();
//			it != resultTrack.end(); it++) {
//		fprintf(fpOut, "%s\n", it->c_str());
//	}
//	fclose(fpOut);
	mwSize dims[1] = {resultTrack4Matlab.size()};
	mxArray * cellArray = mxCreateCellArray(1, dims);
	for (int i = 0; i < resultTrack4Matlab.size(); i++) {
		mxSetCell(cellArray, i, resultTrack4Matlab[i]);
	}
	return cellArray;
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {
	Graph g;
	g.initGraph(prhs[0]);
	while (!g.isFinish()) {
		g.minCostFlow();
		g.saveFlowPathTagDeleteNode();
		g.deleteFlow();
	}
	plhs[0] = g.outputTrack();
	return;
}
