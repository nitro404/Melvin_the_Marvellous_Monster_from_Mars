#pragma once

#include "Includes.h"
#include "Vertex.h"
#include "Edge.h"

class Graph {
public:
	Graph() { }
	~Graph() {
		for(unsigned int i=0;i<edges.size();i++) {
			delete edges.at(i);
		}
	}

	void addEdge(const Edge & e) {
		if(!containsEdge(e)) {
			edges.push_back(new Edge(e));
		}
	}

	int size() {
		return edges.size();
	}

	Edge * elementAt(int i) {
		if(i < 0 || i >= (int) edges.size()) { return NULL; }
		return edges.at(i);
	}

	Edge * getEdge(int i) {
		return elementAt(i);
	}
	
	bool containsVertex(const Vertex & v) {
		for(unsigned int i=0;i<edges.size();i++) {
			if(edges.at(i)->a == v || edges.at(i)->b == v) {
				return true;
			}
		}
		return false;
	}
	
	bool containsEdge(const Edge & e) {
		for(unsigned int i=0;i<edges.size();i++) {
			if(*edges.at(i) == e) {
				return true;
			}
		}
		return false;
	}

private:
	vector<Edge *> edges;
};
