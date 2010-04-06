#pragma once

#include "Includes.h"
#include "DirectX.h"
#include "Sprite.h"
#include "SpriteSheet.h"
#include "SpriteSheets.h"
#include "CollisionHandler.h"

class Object {
public:
	Object() : sprite(NULL) { }
	virtual ~Object() { }

	virtual void tick() { }
	virtual void draw(LPDIRECT3DDEVICE9 d3dDevice) {
		if(sprite != NULL) { sprite->draw(&scale, &offset, orientation, &offset, &position, d3dDevice); }
#if _DEBUG
		testDrawEmptyCircle(d3dDevice, getX(), getY(), getScaledRadius(), getScaledRadius(), _DEBUG_COLOUR);
#endif
	}
	virtual void reset() { }
	virtual void readFrom(ifstream & input) { }

	virtual char * getName() { return (sprite == NULL) ? NULL : sprite->getName(); }

	virtual D3DXVECTOR2 getCenter() { return D3DXVECTOR2(position.x + offset.x, position.y + offset.y); }
	virtual float getX() { return position.x + offset.x; }
	virtual float getY() { return position.y + offset.y; }
	virtual float getOffsetX() { return offset.x; }
	virtual float getOffsetY() { return offset.y; }
	virtual float getHeight() { return size.x; }
	virtual float getWidth() { return size.y; }
	virtual float getScaledRadius() { return ((offset.x < offset.y) ? offset.x : offset.y) * 0.7f; }

	virtual float getOrientation() { return orientation; }
	virtual float getVelocityX() { return velocity.x; }
	virtual float getVelocityY() { return velocity.y; }
	virtual float getSpeed() { return sqrt( pow(velocity.x, 2) + pow(velocity.y, 2) ); }

	virtual void setPosition(float xPos, float yPos) { position.x = xPos; position.y = yPos; }
	virtual void setVelocity(float xVel, float yVel) { velocity.x = xVel; velocity.y = yVel; }

	virtual bool checkCollision(Object & o) { return false; }

	static Object * parseFrom(const char * data, SpriteSheets & spriteSheets) {
		char * temp = strtrimcpy(data);
		Object * o = new Object();

		char * xData = temp;
		char * yData = strchr(xData, ',');
		*yData = '\0';
		yData += sizeof(char);
		char * spriteSheetNameData = strchr(yData, ',');
		*spriteSheetNameData = '\0';
		spriteSheetNameData += sizeof(char);
		char * spriteNameData = strchr(spriteSheetNameData, ',');
		*spriteNameData = '\0';
		spriteNameData += sizeof(char);

		char * spriteSheetName = strtrimcpy(spriteSheetNameData);
		char * spriteName = strtrimcpy(spriteNameData);

		SpriteSheet * spriteSheet = spriteSheets.getSpriteSheet(spriteSheetName);
		if(spriteSheet == NULL) {
			prompt("WARNING: Unable to load from sprite sheet \"%s\".", spriteSheetName);
			delete o;
			delete [] temp;
			delete [] spriteSheetName;
			delete [] spriteName;
			return NULL;
		}
		o->sprite = spriteSheet->getSprite(spriteName);
		if(o->sprite == NULL) {
			prompt("WARNING: Unable to load sprite \"%s\" from sprite sheet \"%s\".", spriteName, spriteSheetName);
			delete o;
			delete [] temp;
			delete [] spriteSheetName;
			delete [] spriteName;
			return NULL;
		}

		o->position.x = (float) atoi(xData);
		o->position.y = (float) atoi(yData);
		if(o->sprite->isTiled()) {
			o->position.x *= Constants::GRID_SIZE;
			o->position.y *= Constants::GRID_SIZE;
		}
		o->scale = D3DXVECTOR2(1, 1);
		o->offset = D3DXVECTOR2((float) (o->sprite->getWidth() / 2.0f) * o->scale.x, (float) (o->sprite->getHeight() / 2.0f) * o->scale.y);
		o->orientation = 0;

		delete [] temp;
		delete [] spriteSheetName;
		delete [] spriteName;
		return o;
	}

protected:
	Sprite * sprite;

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
