// ======================================= //
// Melvin the Marvellous Monster from Mars //
//                                         //
// Author: Kevin Scroggins                 //
// E-Mail: nitro404@hotmail.com            //
// Date: April 11, 2010                    //
// ======================================= //

#pragma once

#include "Includes.h"

// the vertex class represents a location defined by an x and y coordinate pair
class Vertex {
public:
	Vertex() : x(0), y(0) { }
	Vertex(int xPos, int yPos) : x(xPos), y(yPos) { }
	Vertex(const Vertex & v) : x(v.x), y(v.y) { }
	Vertex & operator = (const Vertex & v) { x = v.x; y = v.y; return *this; }
	~Vertex() { }
	
	// operator overrides for manipulating vertices
	inline Vertex operator * (const int & c) const    { return Vertex (x * c, y * c); }
    inline void operator *=  (const int & c)          { x *= c; y *= c; }
	inline Vertex operator / (const int & c) const    { return Vertex(x / c, y / c); }
    inline void operator /=  (const int & c)          { x /= c; y /= c; }
	inline Vertex operator + (const Vertex & v) const { return Vertex(x + v.x, y + v.y); }
    inline void operator +=  (const Vertex & v)       { x += v.x; y += v.y; }
    inline void operator +=  (const int & c)          { x += c; y += c; }
	inline Vertex operator - (const Vertex & v) const { return Vertex(x - v.x, y - v.y); }
    inline void operator -=  (const Vertex & v)       { x -= v.x; y -= v.y; }
    inline void operator -=  (const int & c)          { x -= c; y -= c; }
	inline Vertex operator - () const                 { return Vertex(-x, -y); }
	inline bool operator ==  (const Vertex & v)       { return (x == v.x) && (y == v.y); }
	inline bool operator !=  (const Vertex & v)       { return *this != v; }

	// creates a vertex object from a specified string and returns it
	// parses the vertex from a string of the form: "x, y" where x and y are the corresponding coordinates of the vertex
	static Vertex parseFrom(const char * data) {
		// trim the string and verify its validity
		char * temp = strtrimcpy(data);
		if(temp == NULL) {
			return Vertex(0, 0);
		}

		// find the center of the string (using the comma delimiter), then null terminate it
		char * center = strchr(temp, ',');
		if(center == NULL) {
			delete [] temp;
			return Vertex(0, 0);
		}
		*center = '\0';

		// parse the x and y coordinates from each half of the string and return the instantiated vertex
		int xPos = atoi(temp);
		int yPos = atoi(center + sizeof(char));
		delete [] temp;
		return Vertex(xPos, yPos);
	}

public:
	int x, y;
};
