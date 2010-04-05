#pragma once

#include "Includes.h"
#include "Vertex.h"
#include "Edge.h"

class CollisionHandler {
public:
	static bool checkLineIntersection(Edge & e, D3DXVECTOR2 & p, D3DXVECTOR2 & d, D3DXVECTOR2 & i);
	static bool checkRadiusIntersection(D3DXVECTOR2 & p1, D3DXVECTOR2 & p2, double r1, double r2);
};
