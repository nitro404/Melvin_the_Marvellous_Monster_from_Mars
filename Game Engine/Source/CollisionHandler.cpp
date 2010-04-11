// ======================================= //
// Melvin the Marvellous Monster from Mars //
//                                         //
// Author: Kevin Scroggins                 //
// E-Mail: nitro404@hotmail.com            //
// Date: April 11, 2010                    //
// ======================================= //

#include "CollisionHandler.h"

#if _DEBUG
extern D3DXVECTOR2 playerCollisionPointA;
extern D3DXVECTOR2 playerCollisionPointB;
extern D3DXVECTOR2 playerCollisionPosition;
#endif

bool CollisionHandler::checkLineIntersection(const Edge & edge, const D3DXVECTOR2 & p1, const D3DXVECTOR2 & p2, D3DXVECTOR2 * i, double * newY) {
	double x1 = edge.a.x;
	double y1 = edge.a.y;
	double x2 = edge.b.x;
	double y2 = edge.b.y;
	double x3 = p1.x;
	double y3 = p1.y;
	double x4 = p2.x;
	double y4 = p2.y;

	//Add thickness to the collision line
	y1 += 0.8;
	y2 += 0.8;

    //Is Line Undefined
    if (x1 == x2 && y1 == y2 || x3 == x4 && y3 == y4) {
        return false;
	}

    //Do lines share an End Point
    if (x1 == x3 && y1 == y3 || x2 == x3 && y2 == y3 ||
        x1 == x4 && y1 == y4 || x2 == x4 && y2 == y4) {
        return false;
	}

    //Translate so that PointA is on the Origin
    x2 -= x1; y2 -= y1;
    x3 -= x1; y3 -= y1;
    x4 -= x1; y4 -= y1;

    //Get the Length of AB
    double distAB = sqrt(((x2 * x2) + (y2 * y2)));

    //Rotate the system so that Point B is on the Positive X Axis
    double theCos = x2 / distAB;
    double theSin = y2 / distAB;
    double newX = x3 * theCos + y3 * theSin;
    y3 = y3 * theCos - x3 * theSin; x3 = newX;
    newX = x4 * theCos + y4 * theSin;
    y4 = y4 * theCos - x4 * theSin; x4 = newX;

    //Fail if CD does not cross AB
    if (y3 < 0 && y4 < 0 || y3 >= 0 && y4 >= 0) {
        return false;
	}

    //Discover the position of the intersection point along line A-B
    double ABpos = x4 + (x3 - x4) * y4 / (y4 - y3);

    //Fail if segment CD crosses line AB outside segment AB
    if (ABpos < 0 || ABpos > distAB) {
        return false;
	}

    //Apply the discovered position to line A-B in the originial 
	if(i != NULL) {
	    i->x = (float) (x1 + (ABpos * theCos));
	    i->y = (float) (y1 + (ABpos * theSin));
	}

	//Compute the new y value based on the line the object is colliding with and offset it based on the thickness of the line to prevent bouncing
	if(newY != NULL) {
		x1 = edge.a.x;
		y1 = edge.a.y;
		x2 = edge.b.x;
		y2 = edge.b.y;
		double x3 = p2.x;
		double slope = ((double) (y2 - y1)) / ((double) (x2 - x1));
		double b = y1 + (slope * (0 - x1));
		*newY = (slope * x3) + b + 0.8;

#if _DEBUG
		// debug rendering
		playerCollisionPointA = D3DXVECTOR2((float) x1, (float) ((slope * x1) + b));
		playerCollisionPointB = D3DXVECTOR2((float) x2, (float) ((slope * x2) + b));
		playerCollisionPosition = D3DXVECTOR2((float) x3, (float)  *newY);
#endif
	}

    return true;
}

bool CollisionHandler::checkRadiusIntersection(const D3DXVECTOR2 & p1, const D3DXVECTOR2 & p2, double r1, double r2) {
	// return true if the distance between two circles is less than 0
	return sqrt(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2)) < r1 + r2;
}
