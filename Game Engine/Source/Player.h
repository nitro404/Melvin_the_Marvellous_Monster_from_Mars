#pragma once

#include "Includes.h"
#include "Variables.h"
#include "Sprite.h"
#include "SpriteSheet.h"
#include "Items.h"

class Player : public Object, public Items {
public:
	Player(float xPos, float yPos, int windowWidth, int windowHeight, Variables * settings, LPDIRECT3DDEVICE9 d3dDevice);
	~Player();
	
	void tick();
	void draw(LPDIRECT3DDEVICE9 d3dDevice);

	void moveLeft();
	void moveRight();
	void jump();

public:
	int isMoving;

private:
	Sprite * playerSprite;
	Sprite * disguiseSprite;
	Sprite * playerSpriteSheetImage;
	SpriteSheet * playerSpriteSheet;

	Variables * settings;

	int disguise;

	bool isJumping;
	float jumpTick;
	float maxJumpHeight;

	int movingAnimationSequence;
	int movingAnimationInterval;
	int movingAnimationEnd;

	D3DCOLOR playerColour;
};
