// ======================================= //
// Melvin the Marvellous Monster from Mars //
//                                         //
// Author: Kevin Scroggins                 //
// E-Mail: nitro404@hotmail.com            //
// Date: April 11, 2010                    //
// ======================================= //

#pragma once

#if _DEBUG
#include "Includes.h"
extern int externalScrollingOffset;
#endif

#include "Vertex.h"

// the edge class represents a pair of vertices which form a segmented line
class Edge {
public:
	Edge() { }
	Edge(const Vertex & v1, const Vertex & v2) : a(v1), b(v2) { }
	Edge(const Edge & e) : a(e.a), b(e.b) { }
	Edge & operator = (const Edge & e) { a = e.a; b = e.b; return *this; }
	~Edge() { }

	// get methods
	bool containsVertex(const Vertex & v) { return a == v || b == v; }
	double getDeltaX() { return b.x - a.x; }
	double getDeltaY() { return b.y - a.y; }
	double getLength() { return sqrt(pow((double) (b.x - a.x), 2) + pow((double) (b.y - a.y), 2)); }

	// creates an edge from the specified string and returns it
	// parses the edge from a string of the form: "x1, y1; x2, y2" where
	// x1 and y1 are the x and y coordinates of the first vertex of the vdge and
	// x2 and y2 are the x and y coordinates of the second vertex of the vdge
	static Edge parseFrom(const char * data) {
		// trim the string and verify its validity
		char * temp = strtrimcpy(data);
		if(temp == NULL) {
			return Edge();
		}

		// find the center of the string (using the semicolon delimiter), then null terminate it
		char * center = strchr(temp, ';');
		if(center == NULL) {
			delete [] temp;
			return Edge();
		}
		*center = '\0';

		// parse each vertex from each corresponding half of the string and return the instantiated edge
		Vertex v1 = Vertex::parseFrom(temp);
		Vertex v2 = Vertex::parseFrom(center + sizeof(char));
		delete [] temp;
		return Edge(v1, v2);
	}

	// equality operator overrides
	inline bool operator == (const Edge & e) { return (a == e.a) && (b == e.b); }
	inline bool operator != (const Edge & e) { return *this != e; }

#if _DEBUG
	// debug drawing
	void draw(LPDIRECT3DDEVICE9 d3dDevice) {
		testDrawLine(d3dDevice, (float) a.x, (float) a.y, (float) b.x, (float) b.y, D3DCOLOR_XRGB(255, 255, 0), &externalScrollingOffset);
		testDrawPoint(d3dDevice, (float) a.x, (float) a.y, D3DCOLOR_XRGB(255, 255, 0), &externalScrollingOffset);
		testDrawPoint(d3dDevice, (float) b.x, (float) b.y, D3DCOLOR_XRGB(255, 255, 0), &externalScrollingOffset);
	}
#endif
	
public:
	Vertex a;
	Vertex b;
};
