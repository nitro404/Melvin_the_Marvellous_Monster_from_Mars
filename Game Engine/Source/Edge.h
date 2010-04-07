#pragma once

#if _DEBUG
#include "Includes.h"
extern int externalScrollingOffset;
#endif

#include "Vertex.h"

class Edge {
public:
	Edge() { }
	Edge(const Vertex & v1, const Vertex & v2) : a(v1), b(v2) { }
	Edge(const Edge & e) : a(e.a), b(e.b) { }
	Edge & operator = (const Edge & e) { a = e.a; b = e.b; return *this; }
	~Edge() { }

	bool containsVertex(const Vertex & v) { return a == v || b == v; }
	double getDeltaX() { return b.x - a.x; }
	double getDeltaY() { return b.y - a.y; }
	double getLength() { return sqrt(pow((double) (b.x - a.x), 2) + pow((double) (b.y - a.y), 2)); }

	static Edge parseFrom(const char * data) {
		char * temp = strtrimcpy(data);
		if(temp == NULL) {
			return Edge();
		}
		char * center = strchr(temp, ';');
		if(center == NULL) {
			delete [] temp;
			return Edge();
		}
		*center = '\0';
		Vertex v1 = Vertex::parseFrom(temp);
		Vertex v2 = Vertex::parseFrom(center + sizeof(char));
		delete [] temp;
		return Edge(v1, v2);
	}

	inline bool operator == (const Edge & e) { return (a == e.a) && (b == e.b); }
	inline bool operator != (const Edge & e) { return *this != e; }

#if _DEBUG
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
