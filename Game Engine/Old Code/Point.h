#pragma once

#include "Includes.h"

class Point {
public:
	Point(double xPos = 0, double yPos = 0) : x(xPos), y(yPos) { }
	Point(const Point & p) : x(p.x), y(p.y) { }
	Point & operator = (const Point & p) { this->x = p.x; this->y = p.y; return *this; }
	~Point() { }
	
	inline Point operator * (double d) const		{ return Point(x * d, y * d); }
    inline void operator *= (double d)				{ x *= d; y *= d; }
	inline Point operator / (double d) const		{ return Point(x * (1.0/d), y * (1.0/d)); }
    inline void operator /= (double d)				{x /= d; y /= d; }
	inline Point operator + (const Point & p) const { return Point(x + p.x, y + p.y); }
    inline void operator += (const Point & p)		{ x += p.x; y += p.y; }
    inline void operator += (double d)				{ x += d; y += d; }
	inline Point operator - (const Point & p) const { return Point(x - p.x, y - p.y); }
    inline void operator -= (const Point & p)		{ x -= p.x; y -= p.y; }
    inline void operator -= (double d)				{ x -= d; y -=d ; }
	inline Point operator - () const				{ return Point(-x, -y); }
	inline bool operator == (const Point & p)		{ return x == p.x && y == p.y; }
	inline bool operator != (const Point & p)		{ return *this != p; }
	
	double length() { return sqrt((x * x) + (y * y)); }
	double dot(Point & p) { return (x * p.x) + (y * p.y); }
	
	double x, y;
};
