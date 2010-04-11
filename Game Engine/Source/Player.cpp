// ======================================= //
// Melvin the Marvellous Monster from Mars //
//                                         //
// Author: Kevin Scroggins                 //
// E-Mail: nitro404@hotmail.com            //
// Date: April 11, 2010                    //
// ======================================= //

#include "Player.h"
#include "Level.h"

#if _DEBUG
extern D3DXVECTOR2 playerNewPosition;
extern D3DXVECTOR2 playerLastPosition;

extern D3DXVECTOR2 playerCollisionPointA;
extern D3DXVECTOR2 playerCollisionPointB;
extern D3DXVECTOR2 playerCollisionPosition;
#endif

// disguise type constants
const int Player::DISGUISE_NONE = -1;
const int Player::DISGUISE_WIG = 0;
const int Player::DISGUISE_FBI = 1;
const int Player::DISGUISE_BOX = 2;
const int Player::DISGUISE_BIO = 3;

Player::Player(float xPos,
			   float yPos,
			   int windowWidth,
			   int windowHeight,
			   double & externalTimeElapsed,
			   Level & externalLevel,
			   Variables * externalSettings,
			   SpriteSheets * spriteSheets,
			   LPDIRECT3DDEVICE9 d3dDevice)
				: timeElapsed(externalTimeElapsed),
				  settings(externalSettings),
				  level(externalLevel),
				  disguise(DISGUISE_NONE),
				  isJumping(false),
				  doneJumping(true),
				  jumpVelocity(32),
				  jumpCooldown(0.7f),
				  jumpTime(0),
				  isMoving(false),
				  movementDirection(0),
				  movingAnimationSequence(0),
				  movingAnimationInterval(11),
				  movingAnimationEnd(movingAnimationInterval * 3),
				  grabRadius(35),
				  item(NULL),
				  isGrabbing(false),
				  grabAnimationSequence(0),
				  grabAnimationInterval(9),
				  grabAnimationEnd(grabAnimationInterval * 3),
				  playerSprite(NULL),
				  disguiseSprite(NULL),
				  playerSprites(NULL),
				  disguiseSprites(NULL),
				  spacing(25) {
	this->windowHeight = windowHeight;
	this->windowWidth = windowWidth;
	
	// obtain the player sprite sheet
	this->playerSpriteSheet = spriteSheets->getSpriteSheet("Alien");

	// load all corresponding animation and disguise sprites
	loadSprites();

	// instantiate all player transformation values
	this->scale = D3DXVECTOR2(1, 1);
	this->size = D3DXVECTOR2(playerSprite->getWidth() * scale.x, playerSprite->getHeight() * scale.y);
	this->offset = D3DXVECTOR2(((playerSprite->getWidth() / 2.0f) * scale.x) + 20, (playerSprite->getHeight() / 2.0f) * scale.y);
	this->position = D3DXVECTOR2(xPos, yPos);
	this->velocity = D3DXVECTOR2(0, 0);
	this->maxVelocity = 2;
	this->velocityStep = 20.0f;

	// add the initial player position to the player movement history
	this->movementHistory.push_front(D3DXVECTOR2(position.x + offset.x, position.y + size.y));
}

Player::~Player() {
	for(int i=0;i<4;i++) {
		delete [] disguiseSprites[i];
	}
	delete [] disguiseSprites;
	delete [] playerSprites;
}

// update the player
void Player::tick() {
	// keep track of the player's previous position
	static D3DXVECTOR2 previousLastPosition = position;
	D3DXVECTOR2 lastPosition = position;
	D3DXVECTOR2 lastBottomCenter(position.x + offset.x, position.y + size.y);
	D3DXVECTOR2 previousLastBottomCenter(previousLastPosition.x + offset.x, previousLastPosition.y + size.y);

	// update the player's colour (not yet implemented)
	playerColour = D3DCOLOR_RGBA(255, 255, 255, 255);

	// jump delay code
	jumpTime -= (float) timeElapsed;
	if(jumpTime < 0 && velocity.x == 0 && velocity.y == 0) {
		jumpTime = 0;
		isJumping = false;
	}

	// increment the x position based on the movement input and the y position based on gravity
	position.x += (isMoving) ? (((movementDirection >= 0) ? 1 : -1) * (float) (velocityStep * timeElapsed * 10)) : 0;
	position.y += (float) (-velocity.y * (timeElapsed * 10));
	velocity.y -= (float) (Constants::GRAVITY * (timeElapsed * 10));

	// select the player movement sprites based on the movement animation and disguise type
	playerSprite = playerSprites[movingAnimationSequence / movingAnimationInterval];
	if(disguise != DISGUISE_NONE) {
		disguiseSprite = disguiseSprites[disguise][(movingAnimationSequence / movingAnimationInterval)];
	}

	// if the player is moving
	if(isMoving) {
		// increment the movement animation
		movingAnimationSequence++;
		if(movingAnimationSequence >= movingAnimationEnd) {
			movingAnimationSequence = 0;
		}
	}

	// if the player is grabbing
	if(isGrabbing) {
		// increment the grabbing animation
		grabAnimationSequence++;
		// if the grab animation is at the end, interact with the item the player picked up
		if(grabAnimationSequence >= grabAnimationEnd) {
			grabAnimationSequence = 0;
			isGrabbing = false;
			if(item != NULL) {
				if(_stricmp(item->getName(), "Wig") == 0) {
					disguise = DISGUISE_WIG;
				}
				else if(_stricmp(item->getName(), "BioHazard Suit") == 0) {
					disguise = DISGUISE_BIO;
				}
				else if(_stricmp(item->getName(), "FBI Suit") == 0) {
					disguise = DISGUISE_FBI;
				}
				else if(_stricmp(item->getName(), "Box") == 0) {
					disguise = DISGUISE_BOX;
				}
			}
			item = NULL;
		}
	}

	// if the player is grabbing an item
	if(isGrabbing) {
		// obtain the appropriate frame to render
		int grabIndex = grabAnimationSequence / grabAnimationInterval;

		// then offset the frame based on whether the item is below the player's center of gravity
		if(item->getY() >= frontOfPlayer.y) {
			playerSprite = playerSprites[3 + grabIndex];
			if(disguise != DISGUISE_NONE) {
				disguiseSprite = disguiseSprites[disguise][3 + grabIndex];
			}
		}
		// or above the player's center of gravity
		else {
			playerSprite = playerSprites[6 + grabIndex];
			if(disguise != DISGUISE_NONE) {
				disguiseSprite = disguiseSprites[disguise][6 + grabIndex];
			}
		}
	}

	// if the player is jumping
	if(isJumping) {
		// if the player is done jumping, display the last jumping frame (impacting with the ground)
		if(doneJumping) {
			playerSprite = playerSprites[11];
			if(disguise != DISGUISE_NONE) {
				disguiseSprite = disguiseSprites[disguise][11];
			}
		}
		// otherwise if the player is still in the air
		else {
			// and the player is moving upwards or just starting to fall downwards, display the first jumping sprite
			if(velocity.y >= -12) {
				playerSprite = playerSprites[9];
				if(disguise != DISGUISE_NONE) {
					disguiseSprite = disguiseSprites[disguise][9];
				}
			}
			// or if the player is just starting to move downwards, display the second jumping sprite
			else if(velocity.y < -12) {
				playerSprite = playerSprites[10];
				if(disguise != DISGUISE_NONE) {
					disguiseSprite = disguiseSprites[disguise][10];
				}
			}
		}
	}

	D3DXVECTOR2 intersection;
	D3DXVECTOR2 bottomCenter(position.x + offset.x, position.y + size.y);

#if _DEBUG
	playerNewPosition = bottomCenter;
	playerLastPosition = previousLastBottomCenter;
#endif

	// determine if the player is colliding with a boundary
	double newY;
	if(level.checkCollision(previousLastBottomCenter, bottomCenter, &intersection, &newY)) {
		// if a collisio nwas detected, ensure the player is kept above the collision line
		// and stop the player from jumping if they collide with a line
		position.y = (float) (newY - size.y - 0.1f);
		isJumping = false;
		doneJumping = true;
		velocity.x = 0;
		velocity.y = 0;
	}

	// prevent the player from falling beyond the lower boundary of the screen
	if(position.y + size.y > windowHeight) {
		position.y = windowHeight - size.y;
		doneJumping = true;
		velocity.x = 0;
		velocity.y = 0;
	}

	// prevent the player from travelling off the left side of the map
	if(getX() - getOffsetX() + spacing - (velocityStep * timeElapsed * 10) < 0) {
		position.x = (float) -spacing;
	}

	// prevent the player from travelling off the right side of the map
	if(getX() + getOffsetX() - spacing + (velocityStep * timeElapsed * 10) > level.mapWidth) {
		position.x = level.mapWidth - (getOffsetX() * 2) + spacing;
	}

	// update the point representing the front of the player
	frontOfPlayer = D3DXVECTOR2((movementDirection >= 0) ? getX() + offset.x - spacing : getX() - offset.x + spacing, getY() + 15);
	previousLastPosition = lastPosition;

	// add the new position to the movement history if it is different from the last position
	if(position != lastPosition) {
		movementHistory.push_front(D3DXVECTOR2(position.x + offset.x, position.y + size.y));
	}
}

// render the player
void Player::draw(int * scrollingOffset, LPDIRECT3DDEVICE9 d3dDevice) {
	// compute the sidescrolling offset
	D3DXVECTOR2 offsetPosition = position;
	if(scrollingOffset != NULL) {
		offsetPosition.x -= (*scrollingOffset);
	}

	// render the player and the disguise facing the direction the player is moving
	if(movementDirection >= 0) {
		playerSprite->drawBackwards(&scale, &offset, 0, NULL, &offsetPosition, d3dDevice);
		if(disguise != DISGUISE_NONE && disguiseSprite != NULL) {
			disguiseSprite->drawBackwards(&scale, &offset, 0, NULL, &offsetPosition, d3dDevice);
		}
	}
	else {
		playerSprite->draw(&scale, &offset, 0, NULL, &offsetPosition, d3dDevice);
		if(disguise != DISGUISE_NONE && disguiseSprite != NULL) {
			disguiseSprite->draw(&scale, &offset, 0, NULL, &offsetPosition, d3dDevice);
		}
	}

#if _DEBUG
	// debug rendering
//	testDrawEmptyBox(d3dDevice, getX(), getY(), offset.x, offset.y, &externalScrollingOffset);
//	testDrawPoint(d3dDevice, getX(), getY(), &externalScrollingOffset);

//	testDrawPoint(d3dDevice, frontOfPlayer.x, frontOfPlayer.y, &externalScrollingOffset);
	testDrawEmptyCircle(d3dDevice, frontOfPlayer.x, frontOfPlayer.y, grabRadius, grabRadius, D3DCOLOR_XRGB(0, 0, 255), &externalScrollingOffset);
#endif
}

// move the player left
void Player::moveLeft() {
	movementDirection = -1;
}

// move the player right
void Player::moveRight() {
	movementDirection = 1;
}

// make the player jump
void Player::jump() {
	if(!isJumping && jumpTime == 0) {
		isJumping = true;
		velocity.y = jumpVelocity;
		jumpTime = jumpCooldown;
		doneJumping = false;
	}
}

// make the player grab an item
void Player::grab() {
	// if the player is already grabbing an item, do not let the player grab twice
	if(isGrabbing) { return; }

	// find the item the player grabbed using radial collision detection
	item = NULL;
	for(unsigned int i=0;i<level.items.size();i++) {
		if(CollisionHandler::checkRadiusIntersection(frontOfPlayer, level.items.at(i)->getCenter(), grabRadius, level.items.at(i)->getScaledRadius())) {
			item = level.items.at(i);
			break;
		}
	}

	if(item == NULL) { return; }

	isGrabbing = true;
}

// return true if the player is disguised
bool Player::isDisguised() const {
	return disguise != DISGUISE_NONE;
}

// return the player's last position
D3DXVECTOR2 Player::getFollowPosition() const {
	if(movementHistory.empty()) {
		return D3DXVECTOR2(0, 0);
	}
	else {
		return movementHistory.back();
	}
}

// remove the player's last position
void Player::popFollowPosition() {
	if(!movementHistory.empty()) {
		movementHistory.pop_back();
	}
}

// return true if the player has a previous position in the movement history collection
bool Player::hasFollowPosition() {
	return !movementHistory.empty();
}

// obtain the player and disguise sprites from the sprite sheet collection
void Player::loadSprites() {
	// initialise the animation arrays
	playerSprites = new Sprite*[15];

	disguiseSprites = new Sprite**[4];
	for(int i=0;i<4;i++) {
		disguiseSprites[i] = new Sprite*[12];
	}

	// obtain the player sprites
	playerSprites[0] = playerSpriteSheet->getSprite("Alien Walk 1");
	playerSprites[1] = playerSpriteSheet->getSprite("Alien Walk 2");
	playerSprites[2] = playerSpriteSheet->getSprite("Alien Walk 3");
	playerSprites[3] = playerSpriteSheet->getSprite("Alien Grab Low 1");
	playerSprites[4] = playerSpriteSheet->getSprite("Alien Grab Low 2");
	playerSprites[5] = playerSpriteSheet->getSprite("Alien Grab Low 3");
	playerSprites[6] = playerSpriteSheet->getSprite("Alien Grab High 1");
	playerSprites[7] = playerSpriteSheet->getSprite("Alien Grab High 2");
	playerSprites[8] = playerSpriteSheet->getSprite("Alien Grab High 3");
	playerSprites[9] = playerSpriteSheet->getSprite("Alien Jump 1");
	playerSprites[10] = playerSpriteSheet->getSprite("Alien Jump 2");
	playerSprites[11] = playerSpriteSheet->getSprite("Alien Jump 3");
	playerSprites[12] = playerSpriteSheet->getSprite("Alien Death 1");
	playerSprites[13] = playerSpriteSheet->getSprite("Alien Death 2");
	playerSprites[14] = playerSpriteSheet->getSprite("Alien Death 3");

	// obtain the wig disguise sprites
	disguiseSprites[DISGUISE_WIG][0] = playerSpriteSheet->getSprite("Alien Walk 1 Wig");
	disguiseSprites[DISGUISE_WIG][1] = playerSpriteSheet->getSprite("Alien Walk 2 Wig");
	disguiseSprites[DISGUISE_WIG][2] = playerSpriteSheet->getSprite("Alien Walk 3 Wig");
	disguiseSprites[DISGUISE_WIG][3] = playerSpriteSheet->getSprite("Alien Grab 1 Wig");
	disguiseSprites[DISGUISE_WIG][4] = playerSpriteSheet->getSprite("Alien Grab 2 Wig");
	disguiseSprites[DISGUISE_WIG][5] = playerSpriteSheet->getSprite("Alien Grab 3 Wig");
	disguiseSprites[DISGUISE_WIG][6] = playerSpriteSheet->getSprite("Alien Grab 1 Wig");
	disguiseSprites[DISGUISE_WIG][7] = playerSpriteSheet->getSprite("Alien Grab 2 Wig");
	disguiseSprites[DISGUISE_WIG][8] = playerSpriteSheet->getSprite("Alien Grab 3 Wig");
	disguiseSprites[DISGUISE_WIG][9] = playerSpriteSheet->getSprite("Alien Jump 1 Wig");
	disguiseSprites[DISGUISE_WIG][10] = playerSpriteSheet->getSprite("Alien Jump 2 Wig");
	disguiseSprites[DISGUISE_WIG][11] = playerSpriteSheet->getSprite("Alien Jump 3 Wig");

	// obtrain the fbi suit disguise sprites
	disguiseSprites[DISGUISE_FBI][0] = playerSpriteSheet->getSprite("Alien Walk 1 FBI Suit");
	disguiseSprites[DISGUISE_FBI][1] = playerSpriteSheet->getSprite("Alien Walk 2 FBI Suit");
	disguiseSprites[DISGUISE_FBI][2] = playerSpriteSheet->getSprite("Alien Walk 3 FBI Suit");
	disguiseSprites[DISGUISE_FBI][3] = playerSpriteSheet->getSprite("Alien Grab Low 1 FBI Suit");
	disguiseSprites[DISGUISE_FBI][4] = playerSpriteSheet->getSprite("Alien Grab Low 2 FBI Suit");
	disguiseSprites[DISGUISE_FBI][5] = playerSpriteSheet->getSprite("Alien Grab Low 3 FBI Suit");
	disguiseSprites[DISGUISE_FBI][6] = playerSpriteSheet->getSprite("Alien Grab High 1 FBI Suit");
	disguiseSprites[DISGUISE_FBI][7] = playerSpriteSheet->getSprite("Alien Grab High 2 FBI Suit");
	disguiseSprites[DISGUISE_FBI][8] = playerSpriteSheet->getSprite("Alien Grab High 3 FBI Suit");
	disguiseSprites[DISGUISE_FBI][9] = playerSpriteSheet->getSprite("Alien Jump 1 FBI Suit");
	disguiseSprites[DISGUISE_FBI][10] = playerSpriteSheet->getSprite("Alien Jump 2 FBI Suit");
	disguiseSprites[DISGUISE_FBI][11] = playerSpriteSheet->getSprite("Alien Jump 3 FBI Suit");

	// obtain the box disguise sprites
	disguiseSprites[DISGUISE_BOX][0] = playerSpriteSheet->getSprite("Alien Walk 1 Box");
	disguiseSprites[DISGUISE_BOX][1] = playerSpriteSheet->getSprite("Alien Walk 2 Box");
	disguiseSprites[DISGUISE_BOX][2] = playerSpriteSheet->getSprite("Alien Walk 3 Box");
	disguiseSprites[DISGUISE_BOX][3] = playerSpriteSheet->getSprite("Alien Grab Low 1 Box");
	disguiseSprites[DISGUISE_BOX][4] = playerSpriteSheet->getSprite("Alien Grab Low 2 Box");
	disguiseSprites[DISGUISE_BOX][5] = playerSpriteSheet->getSprite("Alien Grab Low 3 Box");
	disguiseSprites[DISGUISE_BOX][6] = playerSpriteSheet->getSprite("Alien Grab High 1 Box");
	disguiseSprites[DISGUISE_BOX][7] = playerSpriteSheet->getSprite("Alien Grab High 2 Box");
	disguiseSprites[DISGUISE_BOX][8] = playerSpriteSheet->getSprite("Alien Grab High 3 Box");
	disguiseSprites[DISGUISE_BOX][9] = playerSpriteSheet->getSprite("Alien Jump 1 Box");
	disguiseSprites[DISGUISE_BOX][10] = playerSpriteSheet->getSprite("Alien Jump 2 Box");
	disguiseSprites[DISGUISE_BOX][11] = playerSpriteSheet->getSprite("Alien Jump 3 Box");

	// obtain the biohazard suit disguise sprites
	disguiseSprites[DISGUISE_BIO][0] = playerSpriteSheet->getSprite("Alien Walk 1 BioHazard Suit");
	disguiseSprites[DISGUISE_BIO][1] = playerSpriteSheet->getSprite("Alien Walk 2 BioHazard Suit");
	disguiseSprites[DISGUISE_BIO][2] = playerSpriteSheet->getSprite("Alien Walk 3 BioHazard Suit");
	disguiseSprites[DISGUISE_BIO][3] = playerSpriteSheet->getSprite("Alien Grab Low 1 BioHazard Suit");
	disguiseSprites[DISGUISE_BIO][4] = playerSpriteSheet->getSprite("Alien Grab Low 2 BioHazard Suit");
	disguiseSprites[DISGUISE_BIO][5] = playerSpriteSheet->getSprite("Alien Grab Low 3 BioHazard Suit");
	disguiseSprites[DISGUISE_BIO][6] = playerSpriteSheet->getSprite("Alien Grab High 1 BioHazard Suit");
	disguiseSprites[DISGUISE_BIO][7] = playerSpriteSheet->getSprite("Alien Grab High 2 BioHazard Suit");
	disguiseSprites[DISGUISE_BIO][8] = playerSpriteSheet->getSprite("Alien Grab High 3 BioHazard Suit");
	disguiseSprites[DISGUISE_BIO][9] = playerSpriteSheet->getSprite("Alien Jump 1 BioHazard Suit");
	disguiseSprites[DISGUISE_BIO][10] = playerSpriteSheet->getSprite("Alien Jump 2 BioHazard Suit");
	disguiseSprites[DISGUISE_BIO][11] = playerSpriteSheet->getSprite("Alien Jump 3 BioHazard Suit");

	// set the default sprite
	this->playerSprite = playerSprites[0];
}
