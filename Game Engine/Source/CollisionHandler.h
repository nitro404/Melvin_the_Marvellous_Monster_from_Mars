#pragma once

#include "Includes.h"
#include "Vertex.h"
#include "Edge.h"

class CollisionHandler {
public:
	static bool checkLineIntersection(const Edge & e, const D3DXVECTOR2 & p, const D3DXVECTOR2 & d, D3DXVECTOR2 & i);
	static bool checkRadiusIntersection(const D3DXVECTOR2 & p1, const D3DXVECTOR2 & p2, double r1, double r2);
};
