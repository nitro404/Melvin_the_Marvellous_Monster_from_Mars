#pragma once

#include "Includes.h"
#include "Variables.h"
#include "Sprite.h"
#include "SpriteSheet.h"
#include "SpriteSheets.h"
#include "Object.h"

class Level;

class Player : public Object {
public:
	Player(float xPos, float yPos, int windowWidth, int windowHeight, double & externalTimeElapsed, Level & externalLevel, Variables * settings, SpriteSheets * spriteSheets, LPDIRECT3DDEVICE9 d3dDevice);
	~Player();
	
	void tick();
	void draw(int * scrollingOffset, LPDIRECT3DDEVICE9 d3dDevice);

	void moveLeft();
	void moveRight();
	void jump();
	void grab();

private:
	void loadSprites();

public:
	const static int DISGUISE_NONE;
	const static int DISGUISE_WIG;
	const static int DISGUISE_FBI;
	const static int DISGUISE_BOX;
	const static int DISGUISE_BIO;
	bool isMoving;

private:
	Sprite * playerSprite;
	Sprite * disguiseSprite;
	Sprite *** disguiseSprites;
	Sprite ** playerSprites;
	SpriteSheet * playerSpriteSheet;

	double & timeElapsed;
	Variables * settings;
	Level & level;

	int disguise;

	int movementDirection;
	int spacing;

	bool isJumping;
	float jumpVelocity;
	float jumpCooldown;
	float jumpTime;

	bool isGrabbing;
	int movingAnimationSequence;
	int movingAnimationInterval;
	int movingAnimationEnd;

	float grabRadius;
	Object * item;
	int grabAnimationSequence;
	int grabAnimationInterval;
	int grabAnimationEnd;

	D3DCOLOR playerColour;

	D3DXVECTOR2 frontOfPlayer;
};
