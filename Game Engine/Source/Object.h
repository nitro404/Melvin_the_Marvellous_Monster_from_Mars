#pragma once

#include "Includes.h"
#include "DirectX.h"

struct ObjectVertex {
    D3DXVECTOR3 pos;
	float rhw;
    DWORD colour;
};

#define D3DFVF_OBJECT_VERTEX (D3DFVF_XYZRHW|D3DFVF_DIFFUSE)

class Object {
public:
	Object() { }
	virtual ~Object() {
		if(position != NULL) { delete position; }
		if(vertexBuffer != NULL) {
			vertexBuffer->Release();
		}
	}

	virtual void tick() { }
	virtual void draw() { }
	virtual void readFrom(ifstream & input) { }

protected:
	ObjectVertex vertex[4];
	LPDIRECT3DVERTEXBUFFER9 vertexBuffer;

private:
	Point * position;
};
