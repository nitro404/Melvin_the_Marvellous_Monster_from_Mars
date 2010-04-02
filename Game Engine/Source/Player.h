#pragma once

#include "Includes.h"
#include "Variables.h"
#include "Sprite.h"
#include "SpriteSheet.h"
#include "SpriteSheets.h"
#include "Items.h"

class Player : public Object, public Items {
public:
	Player(float xPos, float yPos, int windowWidth, int windowHeight, double & externalTimeElapsed, Variables * settings, SpriteSheets * spriteSheets, LPDIRECT3DDEVICE9 d3dDevice);
	~Player();
	
	void tick();
	void draw(LPDIRECT3DDEVICE9 d3dDevice);

	void moveLeft();
	void moveRight();
	void jump();
	void grab(Object * o);

public:
	bool isMoving;

private:
	Sprite * playerSprite;
	Sprite * disguiseSprite;
	SpriteSheet * playerSpriteSheet;

	double & timeElapsed;
	Variables * settings;

	int disguise;

	int movementDirection;

	bool isJumping;
	float jumpVelocity;
	float jumpCooldown;
	float jumpTime;

	int movingAnimationSequence;
	int movingAnimationInterval;
	int movingAnimationEnd;

	D3DCOLOR playerColour;
};
