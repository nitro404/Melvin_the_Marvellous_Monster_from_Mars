// ======================================= //
// Melvin the Marvellous Monster from Mars //
//                                         //
// Author: Kevin Scroggins                 //
// E-Mail: nitro404@hotmail.com            //
// Date: April 11, 2010                    //
// ======================================= //

#pragma once

#include "Includes.h"
#include "Vertex.h"
#include "Edge.h"

// the graph object represents a collision of edges and vertices which form a graph
class Graph {
public:
	Graph() { }

	~Graph() {
		for(unsigned int i=0;i<edges.size();i++) {
			delete edges.at(i);
		}
	}

	// adds an edge if it is not a duplicate
	void addEdge(const Edge & e) {
		if(!containsEdge(e)) {
			edges.push_back(new Edge(e));
		}
	}

	// returns the number of edges in the graph
	int size() {
		return edges.size();
	}

	// checks to see if any of the vertices in the graph contain the specified vertex
	bool containsVertex(const Vertex & v) {
		for(unsigned int i=0;i<edges.size();i++) {
			if(edges.at(i)->a == v || edges.at(i)->b == v) {
				return true;
			}
		}
		return false;
	}

	// checks to see if the specified edge is contained within the graph
	bool containsEdge(const Edge & e) {
		for(unsigned int i=0;i<edges.size();i++) {
			if(*edges.at(i) == e) {
				return true;
			}
		}
		return false;
	}

	// returns the index of the specified edge if it is contained within the graph
	int indexOfEdge(const Edge & e) {
		for(unsigned int i=0;i<edges.size();i++) {
			if(*edges.at(i) == e) {
				return i;
			}
		}
		return -1;
	}

	// returns the edge at the specified index, if it within the proper range, otherwise returns null
	Edge * elementAt(int index) {
		if(index < 0 || index >= (int) edges.size()) { return NULL; }
		return edges.at(index);
	}

	// see elementAt
	Edge * getEdge(int index) {
		return elementAt(index);
	}

private:
	vector<Edge *> edges;
};
