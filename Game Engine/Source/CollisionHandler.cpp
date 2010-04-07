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

    // Apply the discovered position to line A-B in the originial 
	if(i != NULL) {
	    i->x = (float) (x1 + (ABpos * theCos));
	    i->y = (float) (y1 + (ABpos * theSin));
	}

	if(newY != NULL) {
		double newX = p2.x;
		double slope = ((double) (edge.b.y - edge.a.y)) / ((double) (edge.b.x - edge.a.x));
		double b = edge.a.y + (slope * (0 - edge.a.x));
		*newY = (slope * newX) + b;
#if _DEBUG
		playerCollisionPointA = D3DXVECTOR2((float) edge.a.x, (float) ((slope * edge.a.x) + b));
		playerCollisionPointB = D3DXVECTOR2((float) edge.b.x, (float) ((slope * edge.b.x) + b));
		playerCollisionPosition = D3DXVECTOR2((float) newX, (float)  *newY);
#endif
	}

    //Success
    return true;
}

bool CollisionHandler::checkRadiusIntersection(const D3DXVECTOR2 & p1, const D3DXVECTOR2 & p2, double r1, double r2) {
	return sqrt(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2)) < r1 + r2;
}
