// ======================================= //
// Melvin the Marvellous Monster from Mars //
//                                         //
// Author: Kevin Scroggins                 //
// E-Mail: nitro404@hotmail.com            //
// Date: April 11, 2010                    //
// ======================================= //

#pragma once

#include "Includes.h"
#include "Variables.h"
#include "Object.h"
#include "Items.h"

class Player;

// the pet class represents a simple ai character which follows the player around the map
class Pet : public Object, public Items {
public:
	Pet(float xPos, float yPos, int windowWidth, int windowHeight, Player & externalPlayer, double & externalTimeElapsed, Variables * externalSettings, SpriteSheets * spriteSheets, LPDIRECT3DDEVICE9 d3dDevice);
	~Pet();
	
	void tick(); // update the pet
	void draw(int * scrollingOffset, LPDIRECT3DDEVICE9 d3dDevice); // render the pet

private:
	// the sprites associated with the pet
	Sprite * petSprite;
	Sprite ** petMovementSprites;
	SpriteSheet * petSpriteSheet;

	// movement and animation variables
	int movementDirection;
	int movingAnimationSequence;
	int movingAnimationInterval;
	int movingAnimationEnd;

	// external data
	Player & player;
	double & timeElapsed;
	Variables * settings;
};
