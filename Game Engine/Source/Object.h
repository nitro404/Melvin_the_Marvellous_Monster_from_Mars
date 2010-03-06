#pragma once

#include "Includes.h"
#include "DirectX.h"
#include "Sprite.h"

class Object {
public:
	Object() { }
	virtual ~Object() { }

	virtual void tick() { }
	virtual void draw() { }
	virtual void readFrom(ifstream & input) { }

private:
	D3DXVECTOR2 position;
};
