#pragma once

#include "Includes.h"
#include "Variables.h"
#include "Sprite.h"
#include "SpriteSheet.h"

class Player {
public:
	Player(float xPos, float yPos, int xBoundary, int yBoundary, Variables * settings, LPDIRECT3DDEVICE9 d3dDevice);
	~Player();
	
	void tick();
	void Player::draw(LPDIRECT3DDEVICE9 d3dDevice);

	void moveLeft();
	void moveRight();
	void jump();

private:
	Sprite * playerSprite;
	Sprite * playerSpriteSheet;
	SpriteSheet * playerSprites;

	Variables * settings;

	D3DXVECTOR2 position;
	D3DXVECTOR2 boundary;
	D3DXVECTOR2 scale;
	D3DXVECTOR2 offset;

	static int movementSpeed;

	bool isJumping;
	float jumpTick;
	static float maxJumpHeight;

	D3DCOLOR playerColour;
};
