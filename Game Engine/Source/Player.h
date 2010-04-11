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

class Level;

// the player class represents the user's character within the level
class Player : public Object {
public:
	Player(float xPos, float yPos, int windowWidth, int windowHeight, double & externalTimeElapsed, Level & externalLevel, Variables * settings, SpriteSheets * spriteSheets, LPDIRECT3DDEVICE9 d3dDevice);
	~Player();
	
	void tick(); // update the player
	void draw(int * scrollingOffset, LPDIRECT3DDEVICE9 d3dDevice); // render the player

	void moveLeft(); // move the player left
	void moveRight(); // move the player right
	void jump(); // make the player jump
	void grab(); // make the player grab an item

	bool isDisguised() const; // return true if the player is disguised

	D3DXVECTOR2 getFollowPosition() const; // return the player's last position
	void popFollowPosition(); // remove the player's last position
	bool hasFollowPosition(); // return true if the player has a previous position in the movement history collection

private:
	void loadSprites(); // obtain the player and disguise sprites from the sprite sheet collection

public:
	// disguise type constants
	const static int DISGUISE_NONE;
	const static int DISGUISE_WIG;
	const static int DISGUISE_FBI;
	const static int DISGUISE_BOX;
	const static int DISGUISE_BIO;
	bool isMoving;

private:
	// player sprites
	Sprite * playerSprite;
	Sprite * disguiseSprite;
	Sprite *** disguiseSprites;
	Sprite ** playerSprites;
	SpriteSheet * playerSpriteSheet;

	// player colour
	D3DCOLOR playerColour;

	// disguise variable
	int disguise;

	// jumping variables
	bool isJumping;
	bool doneJumping;
	float jumpVelocity;
	float jumpCooldown;
	float jumpTime;

	// movement variables
	int movementDirection;
	int spacing;
	int movingAnimationSequence;
	int movingAnimationInterval;
	int movingAnimationEnd;
	deque<D3DXVECTOR2> movementHistory;

	// grabbing variables
	bool isGrabbing;
	float grabRadius;
	Object * item;
	int grabAnimationSequence;
	int grabAnimationInterval;
	int grabAnimationEnd;
	D3DXVECTOR2 frontOfPlayer;

	// external data
	double & timeElapsed;
	Variables * settings;
	Level & level;
};
