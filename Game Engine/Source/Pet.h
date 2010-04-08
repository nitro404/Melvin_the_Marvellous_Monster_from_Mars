#pragma once

#include "Includes.h"
#include "Variables.h"
#include "Object.h"
#include "Items.h"

class Player;

class Pet : public Object, public Items {
public:
	Pet(float xPos, float yPos, int windowWidth, int windowHeight, Player & externalPlayer, double & externalTimeElapsed, Variables * externalSettings, SpriteSheets * spriteSheets, LPDIRECT3DDEVICE9 d3dDevice);
	~Pet();
	
	void tick();
	void draw(int * scrollingOffset, LPDIRECT3DDEVICE9 d3dDevice);

private:
	Sprite * petSprite;
	Sprite ** petMovementSprites;
	SpriteSheet * petSpriteSheet;

	int movementDirection;
	int movingAnimationSequence;
	int movingAnimationInterval;
	int movingAnimationEnd;

	Player & player;
	double & timeElapsed;
	Variables * settings;
};
