#include "CollisionHandler.h"

bool CollisionHandler::checkLineIntersection(Edge & e, D3DXVECTOR2 & c, D3DXVECTOR2 & d, D3DXVECTOR2 & i) {
	//Line collision detection algorithm borrowed from previos project, "Lamb Mines"
	Vertex a = e.a;
	Vertex b = e.b;

    //Is Line Undefined
    if (a.x == b.x && a.y == b.y || c.x == d.x && c.y == d.y)
        return false;

    //Do lines share an End Point
    if (a.x == c.x && a.y == c.y || b.x == c.x && b.y == c.y ||
        a.x == d.x && a.y == d.y || b.x == d.x && b.y == d.y)
        return false;

    //Translate so that PointA is on the Origin
    b.x -= a.x; b.y -= a.y;
    c.x -= a.x; c.y -= a.y;
    d.x -= a.x; d.y -= a.y;

    //Get the Length of AB
    float distAB = (float) sqrt((double) ((b.x * b.x) + (b.y * b.y)));

    //Rotate the system so that Point B is on the Positive X Axis
    float theCos = b.x / distAB;
    float theSin = b.y / distAB;
    float newX = c.x * theCos + c.y * theSin;
    c.y = c.y * theCos - c.x * theSin; c.x = newX;
    newX = d.x * theCos + d.y * theSin;
    d.y = d.y * theCos - d.x * theSin; d.x = newX;

    //Fail if CD does not cross AB
    if (c.y < 0 && d.y < 0 || c.y >= 0 && d.y >= 0)
        return false;

    //Discover the position of the intersection point along line A-B
    float ABpos = d.x + (c.x - d.x) * d.y / (d.y - c.y);

    //Fail if segment CD crosses line AB outside segment AB
    if (ABpos < 0 || ABpos > distAB)
        return false;

    // Apply the discovered position to line A-B in the originial 
    i.x = a.x + ABpos * theCos;
    i.y = a.y + ABpos * theSin;

    //Success
    return true;
}
