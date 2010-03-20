#pragma once

#include "Includes.h"
#include "DirectX.h"
#include "Sprite.h"

class Object {
public:
	Object() { }
	virtual ~Object() { }

	virtual void tick() { }
	virtual void draw(LPDIRECT3DDEVICE9 d3dDevice) { }
	virtual void reset() { }
	virtual void readFrom(ifstream & input) { }

	virtual float getX() { return position.x + offset.x; }
	virtual float getY() { return position.y + offset.y; }
	virtual float getOffsetX() { return offset.x; }
	virtual float getOffsetY() { return offset.y; }
	virtual float getHeight() { return size.x; }
	virtual float getWidth() { return size.y; }

	virtual float getOrientation() { return orientation; }
	virtual float getVelocityX() { return velocity.x; }
	virtual float getVelocityY() { return velocity.y; }
	virtual float getVelocity() { return sqrt( pow(velocity.x, 2) + pow(velocity.y, 2) ); }

	virtual void setPosition(float xPos, float yPos) { position.x = xPos; position.y = yPos; }
	virtual void setVelocity(float xVel, float yVel) { velocity.x = xVel; velocity.y = yVel; }

	virtual bool checkCollision(Object & o) { return false; }

protected:
	// position, offset
	D3DXVECTOR2 position;
	D3DXVECTOR2 offset;

	// angle
	float orientation;
	float rotationStep;
	bool rotationDirection;

	// velocity
	D3DXVECTOR2 velocity;
	float velocityStep;
	float minVelocity;
	float maxVelocity;

	// scaling
	float minScale;
	float maxScale;
	D3DXVECTOR2 scale;
	D3DXVECTOR2 size;
	float scaleSpeed;
	bool scaleDirection;

	// boundaries
	int windowWidth;
	int windowHeight;
};
