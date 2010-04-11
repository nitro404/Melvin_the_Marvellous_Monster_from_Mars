// ======================================= //
// Melvin the Marvellous Monster from Mars //
//                                         //
// Author: Kevin Scroggins                 //
// E-Mail: nitro404@hotmail.com            //
// Date: April 11, 2010                    //
// ======================================= //

#pragma once

#include "Includes.h"
#include "DirectX.h"
#include "Sprite.h"
#include "SpriteSheet.h"
#include "SpriteSheets.h"
#include "CollisionHandler.h"

// the object class (also referred to as an entity) represents an object which can be placed within the level (can be of various types including a tile, object or item)
class Object {
public:
	Object() : sprite(NULL) { }
	virtual ~Object() { }

	virtual void tick() { } // update the object
	virtual void draw(int * scrollingOffset, LPDIRECT3DDEVICE9 d3dDevice) { // render the object
		if(sprite != NULL) {
			// calculate the sidescrolling offset
			D3DXVECTOR2 offsetPosition = position;
			if(scrollingOffset != NULL) {
				offsetPosition.x -= (*scrollingOffset);
			}
			
			// render the sprite at the corresponding offset
			sprite->draw(&scale, &offset, orientation, &offset, &offsetPosition, d3dDevice);
		}
#if _DEBUG
		// debug rendering
		testDrawEmptyCircle(d3dDevice, getX(), getY(), getScaledRadius(), getScaledRadius(), _DEBUG_COLOUR, &externalScrollingOffset);
#endif
	}
	virtual void reset() { } // resets the object (not implemented in object)

	virtual char * getName() { return (sprite == NULL) ? NULL : sprite->getName(); } // returns the name of the object

	virtual float getX() { return position.x + offset.x; } // returns the horizontal center of the object
	virtual float getY() { return position.y + offset.y; } // returns the vertical center of the object
	virtual float getOffsetX() { return offset.x; } // retuns the horizontal distance from the edge to the center of the object
	virtual float getOffsetY() { return offset.y; } // returns the vertical distance from the edge to the center of the object
	virtual float getHeight() { return size.x; } // returns the height of the object
	virtual float getWidth() { return size.y; } // returns the width of the object
	virtual D3DXVECTOR2 getCenter() { return D3DXVECTOR2(position.x + offset.x, position.y + offset.y); } // returns a 2d directx vector pointing to the center of the object
	virtual D3DXVECTOR2 getBottomCenter() { return D3DXVECTOR2(position.x + offset.x, position.y + size.y); } // returns a 2d directx vector pointing to the bottom center of the object
	virtual float getScaledRadius() { return ((offset.x < offset.y) ? offset.x : offset.y) * 0.7f; } // returns the radius the size of 70% of the smallest size of the object

	virtual float getOrientation() { return orientation; } // returns the orientation of the object
	virtual float getVelocityX() { return velocity.x; } // returns the x velocity of the object
	virtual float getVelocityY() { return velocity.y; } // returns the y velocity of the object
	virtual float getSpeed() { return sqrt( pow(velocity.x, 2) + pow(velocity.y, 2) ); } // returns the scalar speed of the object

	virtual void setPosition(float xPos, float yPos) { position.x = xPos; position.y = yPos; } // sets the position of the object to the new position (based on the top left corner)
	virtual void setVelocity(float xVel, float yVel) { velocity.x = xVel; velocity.y = yVel; } // sets the velocity of the object to the new velocity

	virtual bool checkCollision(Object & o) { return false; } // checks if the object is colliding with another object (not implemented in object)

	static Object * parseFrom(const char * data, SpriteSheets & spriteSheets) {
		char * temp = strtrimcpy(data);
		Object * o = new Object();

		// parse the x and y coordinates of the object
		char * xData = temp;
		char * yData = strchr(xData, ',');
		*yData = '\0';
		yData += sizeof(char);
		
		// parse the parent sprite sheet name
		char * spriteSheetNameData = strchr(yData, ',');
		*spriteSheetNameData = '\0';
		spriteSheetNameData += sizeof(char);

		// parse the sprite name
		char * spriteNameData = strchr(spriteSheetNameData, ',');
		*spriteNameData = '\0';
		spriteNameData += sizeof(char);

		// trim the sprite sheet / sprite names
		char * spriteSheetName = strtrimcpy(spriteSheetNameData);
		char * spriteName = strtrimcpy(spriteNameData);

		// obtain the specified sprite sheet
		SpriteSheet * spriteSheet = spriteSheets.getSpriteSheet(spriteSheetName);
		if(spriteSheet == NULL) {
			prompt("WARNING: Unable to load from sprite sheet \"%s\".", spriteSheetName);
			delete o;
			delete [] temp;
			delete [] spriteSheetName;
			delete [] spriteName;
			return NULL;
		}

		// obtain the specified sprite from the corresponding sprite sheet
		o->sprite = spriteSheet->getSprite(spriteName);
		if(o->sprite == NULL) {
			prompt("WARNING: Unable to load sprite \"%s\" from sprite sheet \"%s\".", spriteName, spriteSheetName);
			delete o;
			delete [] temp;
			delete [] spriteSheetName;
			delete [] spriteName;
			return NULL;
		}

		// set the position of the object and translate it if it is tiled
		o->position.x = (float) atoi(xData);
		o->position.y = (float) atoi(yData);
		if(o->sprite->isTiled()) {
			o->position.x *= Constants::GRID_SIZE;
			o->position.y *= Constants::GRID_SIZE;
		}
		o->scale = D3DXVECTOR2(1, 1); // set the scale (default is natural scale)
		o->offset = D3DXVECTOR2((float) (o->sprite->getWidth() / 2.0f) * o->scale.x, (float) (o->sprite->getHeight() / 2.0f) * o->scale.y); // set the offset of the object
		o->orientation = 0; // set the orientation (0 is default)

		delete [] temp;
		delete [] spriteSheetName;
		delete [] spriteName;
		return o;
	}

protected:
	// the active sprite to render
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
