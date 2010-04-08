#pragma once

#include "Includes.h"
#include "Variables.h"
#include "Object.h"
#include "Items.h"

class Player;
class Level;

class AI : public Object {
public:
	AI(int type, float xPos, float yPos, int externalWindowWidth, int externalWindowHeight, int externalMapWidth, int externalMapHeight, Player & externalPlayer, Level & externalLevel, double & externalTimeElapsed, SpriteSheets * spriteSheets);
	~AI();
	
	void tick();
	void draw(int * scrollingOffset, LPDIRECT3DDEVICE9 d3dDevice);

	void setStateIdle(bool resetAnimation = true);
	void setStateMoving(int direction = 0, bool resetAnimation = true);
	void setStateAttacking(bool resetAnimation = true);
	void setStateRandom(bool resetAnimation = true);

	bool isIdle() const;
	bool isMoving() const;
	bool isAttacking() const;

	bool canMove() const;
	bool canAttack() const;

	bool hasIdleAnimation() const;
	bool hasMovingAnimation() const;
	bool hasAttackingAnimation() const;

	static int parseType(const char * data);
	static int parseCategory(int type);
	static AI * parseFrom(const char * data, SpriteSheets & spriteSheets, int externalWindowWidth, int externalWindowHeight, int mapWidth, int mapHeight, Player & externalPlayer, Level & externalLevel, double & timeElapsed);

private:
	void loadSprites(SpriteSheets * spriteSheets);

public:
	const static int TYPE_UNKNOWN;
	const static int TYPE_WHITE_RAT;
	const static int TYPE_BROWN_RAT;
	const static int TYPE_GRAY_RAT;
	const static int TYPE_FBI_AGENT;
	const static int TYPE_BIOHAZARD_PERSON;
	const static int TYPE_SCIENTIST_1;
	const static int TYPE_SCIENTIST_2;
	const static int TYPE_GIRL_HOLDING_BABY;
	const static int TYPE_GRANNY;
	const static int TYPE_CONDUCTOR;
	const static int TYPE_HOBO;

	const static int CATEGORY_UNKNOWN;
	const static int CATEGORY_RAT;
	const static int CATEGORY_SIMPLE;
	const static int CATEGORY_EXTENDED;
	const static int CATEGORY_STATIC;

	const static int STATE_IDLE;
	const static int STATE_MOVING;
	const static int STATE_ATTACKING;

private:
	Sprite ** movementSprites;
	Sprite ** idleSprites;
	Sprite ** attackSprites;
	SpriteSheet * spriteSheet;
	int type;
	int category;

	int state;
	double changeStateTimer;
	int movementDirection;
	int movingAnimationSequence;
	int movingAnimationInterval;
	int movingAnimationEnd;
	int idleAnimationSequence;
	int idleAnimationInterval;
	int idleAnimationEnd;
	int attackAnimationSequence;
	int attackAnimationInterval;
	int attackAnimationEnd;

	double & timeElapsed;
	Player & player;
	Level & level;
	int mapWidth;
	int mapHeight;
};
