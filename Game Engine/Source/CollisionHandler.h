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

// the collision handler class is a non-instantiable static class which is used to detect collisions between objects
class CollisionHandler {
public:
	static bool checkLineIntersection(const Edge & edge, const D3DXVECTOR2 & p1, const D3DXVECTOR2 & p2, D3DXVECTOR2 * i, double * newY);
	static bool checkRadiusIntersection(const D3DXVECTOR2 & p1, const D3DXVECTOR2 & p2, double r1, double r2);
};
