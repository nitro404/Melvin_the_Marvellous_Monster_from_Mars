#pragma once

#include "Includes.h"

class Vertex {
public:
	Vertex() : x(0), y(0) { }
	Vertex(int xPos, int yPos) : x(xPos), y(yPos) { }
	Vertex(const Vertex & v) : x(v.x), y(v.y) { }
	Vertex & operator = (const Vertex & v) { x = v.x; y = v.y; return *this; }
	~Vertex() { }
	
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

	inline double length() { return sqrt((double) ((x * x) + (y * y))); }

	static Vertex parseFrom(const char * data) {
		char * temp = strtrimcpy(data);
		if(temp == NULL) {
			return Vertex(0, 0);
		}
		char * center = strchr(temp, ',');
		if(center == NULL) {
			delete [] temp;
			return Vertex(0, 0);
		}
		*center = '\0';
		int xPos = atoi(temp);
		int yPos = atoi(center + sizeof(char));
		delete [] temp;
		return Vertex(xPos, yPos);
	}

public:
	int x, y;
};
