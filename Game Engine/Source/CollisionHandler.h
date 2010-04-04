#pragma once

#include "Includes.h"
#include "Vertex.h"
#include "Edge.h"

class CollisionHandler {
public:
	static bool checkLineIntersection(Edge & e, D3DXVECTOR2 & p, D3DXVECTOR2 & d, D3DXVECTOR2 & i);
};
