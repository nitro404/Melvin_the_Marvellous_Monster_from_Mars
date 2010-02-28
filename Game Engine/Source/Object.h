#pragma once

#include "Includes.h"
#include "DirectX.h"

class Object {
public:
	Object() { }
	virtual ~Object() {
		if(position != NULL) { delete position; }
	}

	virtual void tick() { }
	virtual void draw() { }
	virtual void readFrom(ifstream & input) { }

private:
	Point * position;
};
