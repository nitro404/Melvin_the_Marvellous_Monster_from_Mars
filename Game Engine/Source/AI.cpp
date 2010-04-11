// ======================================= //
// Melvin the Marvellous Monster from Mars //
//                                         //
// Author: Kevin Scroggins                 //
// E-Mail: nitro404@hotmail.com            //
// Date: April 11, 2010                    //
// ======================================= //

#include "AI.h"
#include "Player.h"
#include "Level.h"

// ai type constants
const int AI::TYPE_UNKNOWN = -1;
const int AI::TYPE_WHITE_RAT = 0;
const int AI::TYPE_BROWN_RAT = 1;
const int AI::TYPE_GRAY_RAT = 2;
const int AI::TYPE_FBI_AGENT = 3;
const int AI::TYPE_BIOHAZARD_PERSON = 4;
const int AI::TYPE_SCIENTIST_1 = 5;
const int AI::TYPE_SCIENTIST_2 = 6;
const int AI::TYPE_GIRL_HOLDING_BABY = 7;
const int AI::TYPE_GRANNY = 8;
const int AI::TYPE_CONDUCTOR = 9;
const int AI::TYPE_HOBO = 10;

// ai grouping constants
const int AI::CATEGORY_UNKNOWN = -1;
const int AI::CATEGORY_RAT = 0;
const int AI::CATEGORY_SIMPLE = 1;
const int AI::CATEGORY_EXTENDED = 2;
const int AI::CATEGORY_STATIC = 3;

// ai state constants
const int AI::STATE_IDLE = 0;
const int AI::STATE_MOVING = 1;
const int AI::STATE_ATTACKING = 2;

AI::AI(int aiType,
	   float xPos,
	   float yPos,
	   int externalWindowWidth,
	   int externalWindowHeight,
	   int externalMapWidth,
	   int externalMapHeight,
	   Player & externalPlayer,
	   Level & externalLevel,
	   double & externalTimeElapsed,
	   SpriteSheets * spriteSheets)
		: type(aiType),
		  category(parseCategory(aiType)),
		  player(externalPlayer),
		  level(externalLevel),
		  timeElapsed(externalTimeElapsed),
		  mapWidth(externalMapWidth),
		  mapHeight(externalMapHeight),
		  state(STATE_IDLE),
		  movementDirection(((rand() % 2) == 0) ? -1 : 1), // randomize the direction (left/right)
		  changeStateTimer(0), // set the ai to change states right away
		  movingAnimationSequence(0),
		  idleAnimationSequence(0),
		  attackAnimationSequence(0),
		  movementSprites(NULL),
		  idleSprites(NULL),
		  attackSprites(NULL),
		  spriteSheet(NULL) {
	this->windowWidth = externalWindowWidth;
	this->windowHeight = externalWindowHeight;
	this->sprite = NULL;

	// load the relevant animation sprites for the current ai from the specified sprite sheet collection
	loadSprites(spriteSheets);

	// initialise the transformation variables
	this->scale = D3DXVECTOR2(1, 1);
	this->size = D3DXVECTOR2(sprite->getWidth() * scale.x, sprite->getHeight() * scale.y);
	this->offset = D3DXVECTOR2((sprite->getWidth() / 2.0f) * scale.x, (sprite->getHeight() / 2.0f) * scale.y);
	this->position = D3DXVECTOR2(xPos, yPos);
	this->velocity = D3DXVECTOR2(0, 0);
	this->velocityStep = getRandomFloat(4.9, 5.8);

	// default to the idle state
	setStateIdle();
}

AI::~AI() {
	// clean up local memory
	if(movementSprites != NULL) { delete [] movementSprites; }
	if(idleSprites != NULL) { delete [] idleSprites; }
	if(attackSprites != NULL) { delete [] attackSprites; }
}

// update the ai
void AI::tick() {
	if(canMove()) {
		// move the ai in the corresponding direction
		if(isMoving()) {
			position.x += (isMoving()) ? (((movementDirection >= 0) ? 1 : -1) * (float) (velocityStep * timeElapsed * 10)) : 0;
		}

		// check left boundary
		if(getX() - getOffsetX() + (velocityStep * timeElapsed * 10) < 0) {
			position.x = 0;
			setStateIdle();
		}

		// check right boundary
		if(getX() + getOffsetX() - (velocityStep * timeElapsed * 10) > mapWidth) {
			position.x = mapWidth - (getOffsetX() * 2);
			setStateIdle();
		}
	}

	/*
	static D3DXVECTOR2 previousLastPosition = position;
	D3DXVECTOR2 lastPosition = position;
	D3DXVECTOR2 lastBottomCenter(position.x + offset.x, position.y + size.y);
	D3DXVECTOR2 previousLastBottomCenter(previousLastPosition.x + offset.x, previousLastPosition.y + size.y);

	if(canMove()) {
		if(isMoving()) {
			position.x += (isMoving()) ? (((movementDirection >= 0) ? 1 : -1) * (float) (velocityStep * timeElapsed * 10)) : 0;
		}
		position.y += (float) (-velocity.y * (timeElapsed * 10));
		velocity.y -= (float) (Constants::GRAVITY * (timeElapsed * 10));
	}

	D3DXVECTOR2 intersection;
	D3DXVECTOR2 bottomCenter(position.x + offset.x, position.y + size.y);

	if(canMove()) {
		double newY;
		if(level.checkCollision(previousLastBottomCenter, bottomCenter, &intersection, &newY)) {
			position.y = (float) (newY - size.y - 0.1f);
			velocity.y = 0;
		}

		if(getX() - getOffsetX() + (velocityStep * timeElapsed * 10) < 0) {
			position.x = 0;
			setStateIdle();
		}

		if(getX() + getOffsetX() - (velocityStep * timeElapsed * 10) > mapWidth) {
			position.x = mapWidth - (getOffsetX() * 2);
			setStateIdle();
		}

		if(position.y + size.y > windowHeight) {
			position.y = windowHeight - size.y;
			velocity.y = 0;
		}
	}

	previousLastPosition = lastPosition;
	*/

	// if it is time for the ai to change states, determine the next state based on its type/category
	if(changeStateTimer <= 0) {
		// rat states
		if(category == CATEGORY_RAT) {
			// if the rat is idle, randomize the next state and switch to it
			if(isIdle()) {
				int nextState = rand() % 2;
				if(nextState == 0) {
					setStateMoving();
					changeStateTimer = getRandomInt(5, 8);
				}
				else if(nextState == 1) {
					setStateAttacking();
					changeStateTimer = getRandomInt(1, 2);
				}
			}
			// if the rat is sniffing, randomize the next state and switch to it
			else if(isMoving()) {
				int nextState = rand() % 2;
				if(nextState == 0) {
					setStateAttacking();
					changeStateTimer = getRandomInt(1, 2);
				}
				else if(nextState == 1) {
					setStateIdle();
					changeStateTimer = getRandomInt(3, 4);
				}
			}
			// if the rat is eating, randomize the next state and switch to it
			else if(isAttacking()) {
				int nextState = rand() % 2;
				if(nextState == 0) {
					setStateIdle();
					changeStateTimer = getRandomInt(3, 4);
				}
				else if(nextState == 1) {
					setStateMoving();
					changeStateTimer = getRandomInt(5, 8);
				}
			}
			else {
				setStateIdle();
			}
		}
		// fbi agent states
		else if(type == TYPE_FBI_AGENT) {
			// if the fbi agent is idle, switch him to the moving state
			if(isIdle()) {
				setStateMoving();
				changeStateTimer = getRandomInt(5, 8);
			}
			// if the fbi agent is moving, switch him to the idle state
			else if(isMoving()) {
				setStateIdle();
				changeStateTimer = getRandomInt(6, 9);
			}
			// if the fbi agent has his gun deployed, wait until the player is out of range and set his state to idle
			else if(isAttacking()) {
				if(!player.isDisguised() && !CollisionHandler::checkRadiusIntersection(player.getCenter(), getCenter(), 100, 100)) {
					setStateIdle();
					changeStateTimer = getRandomInt(2, 4);
				}
			}
			else {
				setStateIdle();
			}
		}
		// extended category states
		else if(category == CATEGORY_EXTENDED) {
			// if the ai is idle, set the state to moving
			if(isIdle()) {
				setStateMoving();
				changeStateTimer = getRandomInt(5, 8);
			}
			// if the state is moving, set the state to ai
			else if(isMoving()) {
				setStateIdle();
				changeStateTimer = getRandomInt(6, 9);
			}
			else {
				setStateIdle();
			}
		}
	}

	// if the ai is an fbi agent, play the deploy gun animation and stop the ai if the player gets too close
	if(type == TYPE_FBI_AGENT && !player.isDisguised() && CollisionHandler::checkRadiusIntersection(player.getCenter(), getCenter(), 100, 100)) {
		setStateAttacking(false);
		changeStateTimer = 0;
	}

	// decrement the state change timer
	changeStateTimer -= timeElapsed;

	// increment the idle animation (if appropriate)
	if(isIdle() && hasIdleAnimation()) {
		idleAnimationSequence++;
		if(idleAnimationSequence >= idleAnimationEnd) {
			idleAnimationSequence = 0;
		}
	}

	// increment the movement animation (if appropriate)
	if(isMoving() && hasMovingAnimation()) {
		movingAnimationSequence++;
		if(movingAnimationSequence >= movingAnimationEnd) {
			movingAnimationSequence = 0;
		}
	}

	// increment the attack animation (if appropriate) and freeze it at the end if the ai is an fbi agent
	if(isAttacking() && hasAttackingAnimation()) {
		if(type == TYPE_FBI_AGENT) {
			if(attackAnimationSequence < attackAnimationEnd - 1) {
				attackAnimationSequence++;
			}
		}
		else {
			attackAnimationSequence++;
			if(attackAnimationSequence >= attackAnimationEnd) {
				attackAnimationSequence = 0;
			}
		}
	}

	// if the category is a rat, load the corresponding rat sprite based on the state and animation variables
	if(category == CATEGORY_RAT) {
		if(isIdle()) {
			sprite = idleSprites[idleAnimationSequence / idleAnimationInterval];
		}
		else if(isMoving()) {
			sprite = movementSprites[movingAnimationSequence / movingAnimationInterval];
		}
		else if(isAttacking()) {
			sprite = attackSprites[attackAnimationSequence / attackAnimationInterval];
		}
	}
	// if the category is a simple ai, load the corresponding sprite based on the state and animation variables
	else if(category == CATEGORY_SIMPLE) {
		if(isIdle()) {
			sprite = idleSprites[0];
		}
		else if(isMoving()) {
			sprite = movementSprites[movingAnimationSequence / movingAnimationInterval];
		}
		else if(isAttacking()) {
			sprite = attackSprites[attackAnimationSequence / attackAnimationInterval];
		}
	}
	// if the category is a extended ai, load the corresponding sprite based on the state and animation variables
	else if(category == CATEGORY_EXTENDED) {
		if(isIdle()) {
			sprite = idleSprites[0];
		}
		else if(isMoving()) {
			sprite = movementSprites[movingAnimationSequence / movingAnimationInterval];
		}
	}
	// if the category is static ai, load the corresponding sprite based on whether the player is disguised or not
	// the ai will appear sad if the player is disguised, or neutral if they are disguised
	else if(category == CATEGORY_STATIC) {
		if(player.isDisguised()) {
			sprite = idleSprites[1];
		}
		else {
			sprite = idleSprites[2];
		}
	}
}

// render the ai
void AI::draw(int * scrollingOffset, LPDIRECT3DDEVICE9 d3dDevice) {
	// apply the sidescrolling offset
	D3DXVECTOR2 offsetPosition = position;
	if(scrollingOffset != NULL) {
		offsetPosition.x -= (*scrollingOffset);
	}

	// draw the sprite with respect to which direction it is facing
	if(movementDirection >= 0) {
		sprite->drawBackwards(&scale, &offset, 0, NULL, &offsetPosition, d3dDevice);
	}
	else {
		sprite->draw(&scale, &offset, 0, NULL, &offsetPosition, d3dDevice);
	}
}

// switch the ai's state to idle
void AI::setStateIdle(bool resetAnimation) {
	state = STATE_IDLE;
	if(resetAnimation) {
		movingAnimationSequence = 0;
		idleAnimationSequence = 0;
		attackAnimationSequence = 0;
	}
}

// switch the ai's state to moving
void AI::setStateMoving(int direction, bool resetAnimation) {
	state = STATE_MOVING;
	if(resetAnimation) {
		movingAnimationSequence = 0;
		idleAnimationSequence = 0;
		attackAnimationSequence = 0;
	}
	if(direction == 0) {
		movementDirection = ((rand() % 2) == 0) ? -1 : 1;
	}
}

// switch the ai's state to attacking
void AI::setStateAttacking(bool resetAnimation) {
	state = STATE_ATTACKING;
	if(resetAnimation) {
		movingAnimationSequence = 0;
		idleAnimationSequence = 0;
		attackAnimationSequence = 0;
	}
}

// switch the ai's state to a random state
void AI::setStateRandom(bool resetAnimation) {
	int randomState = rand() % ((canAttack()) ? 3 : 2);
	if(randomState == 0) {
		setStateIdle(resetAnimation);
	}
	else if(randomState == 1) {
		setStateMoving(resetAnimation);
	}
	else if(randomState == 2) {
		setStateAttacking(resetAnimation);
	}
}

// check if the ai is in the specified state
bool AI::isIdle() const { return state == STATE_IDLE; }
bool AI::isMoving() const { return state == STATE_MOVING; }
bool AI::isAttacking() const { return state == STATE_ATTACKING; }

// check if the ai can move
bool AI::canMove() const {
	return category == CATEGORY_RAT || category == CATEGORY_SIMPLE || CATEGORY_EXTENDED;
}

// check if the ai can attack
bool AI::canAttack() const {
	return category == CATEGORY_RAT || category == CATEGORY_SIMPLE;
}

// check if the ai has an animation for the specified state
bool AI::hasIdleAnimation() const { return category == CATEGORY_RAT; }
bool AI::hasMovingAnimation() const { return category == CATEGORY_RAT || category == CATEGORY_SIMPLE || CATEGORY_EXTENDED; }
bool AI::hasAttackingAnimation() const { return category == CATEGORY_RAT || category == CATEGORY_SIMPLE; }

// loads the relevant animation sprites for the current ai from the sprite sheet collection
void AI::loadSprites(SpriteSheets * spriteSheets) {
	// set default animation speeds
	movingAnimationInterval = 10;
	idleAnimationInterval = 10;
	attackAnimationInterval = 10;

	// if the ai is part of the rat category
	if(category == CATEGORY_RAT) {
		// initialise the animation arrays
		movementSprites = new Sprite*[3];
		idleSprites = new Sprite*[3];
		attackSprites = new Sprite*[4];

		// set the animation values
		idleAnimationInterval = 8;
		movingAnimationEnd = movingAnimationInterval * 3;
		idleAnimationEnd = idleAnimationInterval * 3;
		attackAnimationEnd = attackAnimationInterval * 4;

		// if the rat is of type white rat, obtain the corresponding sprites
		if(type == TYPE_WHITE_RAT) {
			// obtain the appropriate sprite sheet
			this->spriteSheet = spriteSheets->getSpriteSheet("White Rat");

			this->movementSprites[0] = this->spriteSheet->getSprite("White Rat Walk 1");
			this->movementSprites[1] = this->spriteSheet->getSprite("White Rat Walk 2");
			this->movementSprites[2] = this->spriteSheet->getSprite("White Rat Walk 3");

			this->idleSprites[0] = this->spriteSheet->getSprite("White Rat Sniff 1");
			this->idleSprites[1] = this->spriteSheet->getSprite("White Rat Sniff 2");
			this->idleSprites[2] = this->spriteSheet->getSprite("White Rat Sniff 3");

			this->attackSprites[0] = this->spriteSheet->getSprite("White Rat Bite 1");
			this->attackSprites[1] = this->spriteSheet->getSprite("White Rat Bite 2");
			this->attackSprites[2] = this->spriteSheet->getSprite("White Rat Bite 3");
			this->attackSprites[3] = this->spriteSheet->getSprite("White Rat Bite 4");
		}
		// if the rat is of type brown rat, obtain the corresponding sprites
		else if(type == TYPE_BROWN_RAT) {
			// obtain the appropriate sprite sheet
			this->spriteSheet = spriteSheets->getSpriteSheet("Brown Rat");
			
			this->movementSprites[0] = this->spriteSheet->getSprite("Brown Rat Walk 1");
			this->movementSprites[1] = this->spriteSheet->getSprite("Brown Rat Walk 2");
			this->movementSprites[2] = this->spriteSheet->getSprite("Brown Rat Walk 3");

			this->idleSprites[0] = this->spriteSheet->getSprite("Brown Rat Sniff 1");
			this->idleSprites[1] = this->spriteSheet->getSprite("Brown Rat Sniff 2");
			this->idleSprites[2] = this->spriteSheet->getSprite("Brown Rat Sniff 3");

			this->attackSprites[0] = this->spriteSheet->getSprite("Brown Rat Bite 1");
			this->attackSprites[1] = this->spriteSheet->getSprite("Brown Rat Bite 2");
			this->attackSprites[2] = this->spriteSheet->getSprite("Brown Rat Bite 3");
			this->attackSprites[3] = this->spriteSheet->getSprite("Brown Rat Bite 4");
		}
		// if the rat is of type gray rat, obtain the corresponding sprites
		else if(type == TYPE_GRAY_RAT) {
			// obtain the appropriate sprite sheet
			this->spriteSheet = spriteSheets->getSpriteSheet("Gray Rat");
			
			this->movementSprites[0] = this->spriteSheet->getSprite("Gray Rat Walk 1");
			this->movementSprites[1] = this->spriteSheet->getSprite("Gray Rat Walk 2");
			this->movementSprites[2] = this->spriteSheet->getSprite("Gray Rat Walk 3");

			this->idleSprites[0] = this->spriteSheet->getSprite("Gray Rat Sniff 1");
			this->idleSprites[1] = this->spriteSheet->getSprite("Gray Rat Sniff 2");
			this->idleSprites[2] = this->spriteSheet->getSprite("Gray Rat Sniff 3");

			this->attackSprites[0] = this->spriteSheet->getSprite("Gray Rat Bite 1");
			this->attackSprites[1] = this->spriteSheet->getSprite("Gray Rat Bite 2");
			this->attackSprites[2] = this->spriteSheet->getSprite("Gray Rat Bite 3");
			this->attackSprites[3] = this->spriteSheet->getSprite("Gray Rat Bite 4");
		}
		this->sprite = this->idleSprites[0];
	}
	// if the ai is part of the simple ai category
	else if(category == CATEGORY_SIMPLE) {
		// initialise the animation arrays
		movementSprites = new Sprite*[5];
		idleSprites = new Sprite*[1];
		attackSprites = new Sprite*[6];

		// set the animation values
		movingAnimationEnd = movingAnimationInterval * 5;
		idleAnimationInterval = 0;
		idleAnimationEnd = 0;
		attackAnimationEnd = attackAnimationInterval * 6;

		// obtain the appropriate sprite sheet
		this->spriteSheet = spriteSheets->getSpriteSheet("People");

		// if the ai is of type fbi agent, obtain the corresponding sprites
		if(type == TYPE_FBI_AGENT) {
			this->movementSprites[0] = this->spriteSheet->getSprite("FBI Agent Walk 1");
			this->movementSprites[1] = this->spriteSheet->getSprite("FBI Agent Walk 2");
			this->movementSprites[2] = this->spriteSheet->getSprite("FBI Agent Walk 3");
			this->movementSprites[3] = this->spriteSheet->getSprite("FBI Agent Walk 4");
			this->movementSprites[4] = this->spriteSheet->getSprite("FBI Agent Walk 5");

			this->idleSprites[0] = this->spriteSheet->getSprite("FBI Agent Stationary");

			this->attackSprites[0] = this->spriteSheet->getSprite("FBI Agent Deploy Gun 1");
			this->attackSprites[1] = this->spriteSheet->getSprite("FBI Agent Deploy Gun 2");
			this->attackSprites[2] = this->spriteSheet->getSprite("FBI Agent Deploy Gun 3");
			this->attackSprites[3] = this->spriteSheet->getSprite("FBI Agent Deploy Gun 4");
			this->attackSprites[4] = this->spriteSheet->getSprite("FBI Agent Deploy Gun 5");
			this->attackSprites[5] = this->spriteSheet->getSprite("FBI Agent Deploy Gun 6");
		}
		this->sprite = this->idleSprites[0];
	}
	// if the ai is part of the extended ai category
	else if(category == CATEGORY_EXTENDED) {
		// initialise the animation arrays
		movementSprites = new Sprite*[13];
		idleSprites = new Sprite*[1];

		// set the animation values
		movingAnimationInterval = 8;
		movingAnimationEnd = movingAnimationInterval * 13;
		idleAnimationInterval = 0;
		idleAnimationEnd = 0;
		attackAnimationInterval = 0;
		attackAnimationEnd = 0;

		// obtain the appropriate sprite sheet
		this->spriteSheet = spriteSheets->getSpriteSheet("People");

		// if the ai is of type biohazard person, obtain the corresponding sprites
		if(type == TYPE_BIOHAZARD_PERSON) {
			this->movementSprites[0] = this->spriteSheet->getSprite("BioHazard Person Walk 1");
			this->movementSprites[1] = this->spriteSheet->getSprite("BioHazard Person Walk 2");
			this->movementSprites[2] = this->spriteSheet->getSprite("BioHazard Person Walk 3");
			this->movementSprites[3] = this->spriteSheet->getSprite("BioHazard Person Walk 4");
			this->movementSprites[4] = this->spriteSheet->getSprite("BioHazard Person Walk 5");
			this->movementSprites[5] = this->spriteSheet->getSprite("BioHazard Person Walk 6");
			this->movementSprites[6] = this->spriteSheet->getSprite("BioHazard Person Walk 7");
			this->movementSprites[7] = this->spriteSheet->getSprite("BioHazard Person Walk 8");
			this->movementSprites[8] = this->spriteSheet->getSprite("BioHazard Person Walk 9");
			this->movementSprites[9] = this->spriteSheet->getSprite("BioHazard Person Walk 10");
			this->movementSprites[10] = this->spriteSheet->getSprite("BioHazard Person Walk 11");
			this->movementSprites[11] = this->spriteSheet->getSprite("BioHazard Person Walk 12");
			this->movementSprites[12] = this->spriteSheet->getSprite("BioHazard Person Walk 13");

			this->idleSprites[0] = this->spriteSheet->getSprite("BioHazard Person Stationary");
		}
		// if the ai is of type scientist 1, obtain the corresponding sprites
		else if(type == TYPE_SCIENTIST_1) {
			this->movementSprites[0] = this->spriteSheet->getSprite("Scientist 1 Walk 1");
			this->movementSprites[1] = this->spriteSheet->getSprite("Scientist 1 Walk 2");
			this->movementSprites[2] = this->spriteSheet->getSprite("Scientist 1 Walk 3");
			this->movementSprites[3] = this->spriteSheet->getSprite("Scientist 1 Walk 4");
			this->movementSprites[4] = this->spriteSheet->getSprite("Scientist 1 Walk 5");
			this->movementSprites[5] = this->spriteSheet->getSprite("Scientist 1 Walk 6");
			this->movementSprites[6] = this->spriteSheet->getSprite("Scientist 1 Walk 7");
			this->movementSprites[7] = this->spriteSheet->getSprite("Scientist 1 Walk 8");
			this->movementSprites[8] = this->spriteSheet->getSprite("Scientist 1 Walk 9");
			this->movementSprites[9] = this->spriteSheet->getSprite("Scientist 1 Walk 10");
			this->movementSprites[10] = this->spriteSheet->getSprite("Scientist 1 Walk 11");
			this->movementSprites[11] = this->spriteSheet->getSprite("Scientist 1 Walk 12");
			this->movementSprites[12] = this->spriteSheet->getSprite("Scientist 1 Walk 13");

			this->idleSprites[0] = this->spriteSheet->getSprite("Scientist 1 Stationary");
		}
		// if the ai is of type scientist 2, obtain the corresponding sprites
		if(type == TYPE_SCIENTIST_2) {
			this->movementSprites[0] = this->spriteSheet->getSprite("Scientist 2 Walk 1");
			this->movementSprites[1] = this->spriteSheet->getSprite("Scientist 2 Walk 2");
			this->movementSprites[2] = this->spriteSheet->getSprite("Scientist 2 Walk 3");
			this->movementSprites[3] = this->spriteSheet->getSprite("Scientist 2 Walk 4");
			this->movementSprites[4] = this->spriteSheet->getSprite("Scientist 2 Walk 5");
			this->movementSprites[5] = this->spriteSheet->getSprite("Scientist 2 Walk 6");
			this->movementSprites[6] = this->spriteSheet->getSprite("Scientist 2 Walk 7");
			this->movementSprites[7] = this->spriteSheet->getSprite("Scientist 2 Walk 8");
			this->movementSprites[8] = this->spriteSheet->getSprite("Scientist 2 Walk 9");
			this->movementSprites[9] = this->spriteSheet->getSprite("Scientist 2 Walk 10");
			this->movementSprites[10] = this->spriteSheet->getSprite("Scientist 2 Walk 11");
			this->movementSprites[11] = this->spriteSheet->getSprite("Scientist 2 Walk 12");
			this->movementSprites[12] = this->spriteSheet->getSprite("Scientist 2 Walk 13");

			this->idleSprites[0] = this->spriteSheet->getSprite("Scientist 2 Stationary");
		}
		this->sprite = this->idleSprites[0];
	}
	// if the ai is part of the static ai category
	else if(category == CATEGORY_STATIC) {
		// initialise the animation arrays
		idleSprites = new Sprite*[3];

		// set the animation values
		movingAnimationEnd = 0;
		movingAnimationInterval = 0;
		idleAnimationEnd = 0;
		idleAnimationInterval = 0;
		attackAnimationEnd = 0;
		attackAnimationInterval = 0;

		// obtain the appropriate sprite sheet
		this->spriteSheet = spriteSheets->getSpriteSheet("People");

		// if the ai is of type girl holding baby, obtain the corresponding sprites
		if(type == TYPE_GIRL_HOLDING_BABY) {
			this->idleSprites[0] = this->spriteSheet->getSprite("Girl Holding Baby Happy");
			this->idleSprites[1] = this->spriteSheet->getSprite("Girl Holding Baby Neutral");
			this->idleSprites[2] = this->spriteSheet->getSprite("Girl Holding Baby Sad");
		}
		// if the ai is of type granny, obtain the corresponding sprites
		else if(type == TYPE_GRANNY) {
			this->idleSprites[0] = this->spriteSheet->getSprite("Granny Happy");
			this->idleSprites[1] = this->spriteSheet->getSprite("Granny Neutral");
			this->idleSprites[2] = this->spriteSheet->getSprite("Granny Sad");
		}
		// if the ai is of type conductor, obtain the corresponding sprites
		else if(type == TYPE_CONDUCTOR) {
			this->idleSprites[0] = this->spriteSheet->getSprite("Conductor Happy");
			this->idleSprites[1] = this->spriteSheet->getSprite("Conductor Neutral");
			this->idleSprites[2] = this->spriteSheet->getSprite("Conductor Sad");
		}
		// if the ai is of type hobo, obtain the corresponding sprites
		if(type == TYPE_HOBO) {
			this->idleSprites[0] = this->spriteSheet->getSprite("Hobo Happy");
			this->idleSprites[1] = this->spriteSheet->getSprite("Hobo Neutral");
			this->idleSprites[2] = this->spriteSheet->getSprite("Hobo Sad");
		}
		this->sprite = this->idleSprites[0];
	}
}

// parses the type of ai from a string and returns the corresponding constant value
int AI::parseType(const char * data) {
	int type = TYPE_UNKNOWN;
	if(data == NULL) { return type; }
	char * temp = strtrimcpy(data);
	if(temp == NULL || strlen(temp) == 0) { return type; }

	if(strlen(temp) > 17) {
		temp[17] = '\0';
		if(_stricmp(temp, "Girl Holding Baby") == 0) {
			type = TYPE_GIRL_HOLDING_BABY;
		}
	}
	if(strlen(temp) > 16) {
		temp[16] = '\0';
		if(_stricmp(temp, "BioHazard Person") == 0) {
			type = TYPE_BIOHAZARD_PERSON;
		}
	}
	if(strlen(temp) > 11) {
		temp[11] = '\0';
		if(_stricmp(temp, "Scientist 1") == 0) {
			type = TYPE_SCIENTIST_1;
		}
		if(_stricmp(temp, "Scientist 2") == 0) {
			type = TYPE_SCIENTIST_2;
		}
	}
	if(strlen(temp) > 9) {
		temp[9] = '\0';
		if(_stricmp(temp, "Conductor") == 0) {
			type = TYPE_CONDUCTOR;
		}
		if(_stricmp(temp, "White Rat") == 0) {
			type = TYPE_WHITE_RAT;
		}
		if(_stricmp(temp, "Brown Rat") == 0) {
			type = TYPE_BROWN_RAT;
		}
		if(_stricmp(temp, "FBI Agent") == 0) {
			type = TYPE_FBI_AGENT;
		}
	}
	if(strlen(temp) > 8) {
		temp[8] = '\0';
		if(_stricmp(temp, "Gray Rat") == 0) {
			type = TYPE_GRAY_RAT;
		}
	}
	if(strlen(temp) > 6) {
		temp[6] = '\0';
		if(_stricmp(temp, "Granny") == 0) {
			type = TYPE_GRANNY;
		}
	}
	if(strlen(temp) > 4) {
		temp[4] = '\0';
		if(_stricmp(temp, "Hobo") == 0) {
			type = TYPE_HOBO;
		}
	}

	delete [] temp;

	return type;
}

// parses the category the ai belongs to based on its type and returns it
int AI::parseCategory(int type) {
	int category = CATEGORY_UNKNOWN;

	if(type == TYPE_WHITE_RAT ||
	   type == TYPE_BROWN_RAT ||
	   type == TYPE_GRAY_RAT) {
		category = CATEGORY_RAT;
	}
	else if(type == TYPE_FBI_AGENT) {
		category = CATEGORY_SIMPLE;
	}
	else if(type == TYPE_BIOHAZARD_PERSON ||
			type == TYPE_SCIENTIST_1 ||
			type == TYPE_SCIENTIST_2) {
		category = CATEGORY_EXTENDED;
	}
	else if(type == TYPE_GIRL_HOLDING_BABY ||
			type == TYPE_CONDUCTOR ||
			type == TYPE_GRANNY ||
			type == TYPE_HOBO) {
		category = CATEGORY_STATIC;
	}

	return category;
}

// parses the ai from a string, initialises and returns it with corresponding sprites loaded from the specified sprite sheet collection
AI * AI::parseFrom(const char * data, SpriteSheets & spriteSheets, int externalWindowWidth, int externalWindowHeight, int mapWidth, int mapHeight, Player & externalPlayer, Level & externalLevel, double & timeElapsed) {
	char * temp = strtrimcpy(data);

	// parse the x and y location of the ai
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

	// trim the sprite / sprite sheet names
	char * spriteSheetName = strtrimcpy(spriteSheetNameData);
	char * spriteName = strtrimcpy(spriteNameData);

	// parse the ai type
	int type = parseType(spriteName);

	// create and initialise the ai
	AI * ai = new AI(type, (float) atoi(xData), (float) atoi(yData), externalWindowWidth, externalWindowHeight, mapWidth, mapHeight, externalPlayer, externalLevel, timeElapsed, &spriteSheets);

	delete [] temp;
	delete [] spriteSheetName;
	delete [] spriteName;
	return ai;
}
