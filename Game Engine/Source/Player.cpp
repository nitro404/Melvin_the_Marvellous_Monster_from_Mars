#include "Player.h"
#include "Level.h"

#if _DEBUG
extern D3DXVECTOR2 playerNewPosition;
extern D3DXVECTOR2 playerLastPosition;

extern D3DXVECTOR2 playerCollisionPointA;
extern D3DXVECTOR2 playerCollisionPointB;
extern D3DXVECTOR2 playerCollisionPosition;
#endif

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
				  jumpVelocity(32),
				  jumpCooldown(0.8f),
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
	
	this->playerSpriteSheet = spriteSheets->getSpriteSheet("Alien");

	loadSprites();

	this->playerSprite = playerSpriteSheet->getSprite("Alien Walk 1");
//	this->disguiseSprite = playerSpriteSheet->getSprite("Alien Walk 1 Wig");

	this->scale = D3DXVECTOR2(1, 1);
	this->size = D3DXVECTOR2(playerSprite->getWidth() * scale.x, playerSprite->getHeight() * scale.y);
	this->offset = D3DXVECTOR2(((playerSprite->getWidth() / 2.0f) * scale.x) + 20, (playerSprite->getHeight() / 2.0f) * scale.y);
	this->position = D3DXVECTOR2(xPos, yPos);
	this->velocity = D3DXVECTOR2(0, 0);
	this->maxVelocity = 2;
	this->velocityStep = 20.0f;

	this->movementHistory.push_front(D3DXVECTOR2(position.x + offset.x, position.y + size.y));
}

Player::~Player() {
	for(int i=0;i<4;i++) {
		delete [] disguiseSprites[i];
	}
	delete [] disguiseSprites;
	delete [] playerSprites;
}

void Player::loadSprites() {
	playerSprites = new Sprite*[15];

	disguiseSprites = new Sprite**[4];
	for(int i=0;i<4;i++) {
		disguiseSprites[i] = new Sprite*[12];
	}

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
}

void Player::tick() {
	static D3DXVECTOR2 previousLastPosition = position;
	D3DXVECTOR2 lastPosition = position;
	D3DXVECTOR2 lastBottomCenter(position.x + offset.x, position.y + size.y);
	D3DXVECTOR2 previousLastBottomCenter(previousLastPosition.x + offset.x, previousLastPosition.y + size.y);

	playerColour = D3DCOLOR_RGBA(255, 255, 255, 255);

	jumpTime -= (float) timeElapsed;
	if(jumpTime < 0 && velocity.x == 0 && velocity.y == 0) {
		jumpTime = 0;
		isJumping = false;
	}

	position.x += (isMoving) ? (((movementDirection >= 0) ? 1 : -1) * (float) (velocityStep * timeElapsed * 10)) : 0;
	position.y += (float) (-velocity.y * (timeElapsed * 10));
	velocity.y -= (float) (Constants::GRAVITY * (timeElapsed * 10));

	playerSprite = playerSprites[movingAnimationSequence / movingAnimationInterval];
	if(disguise != DISGUISE_NONE) {
		disguiseSprite = disguiseSprites[disguise][(movingAnimationSequence / movingAnimationInterval)];
	}

	if(isMoving) {
		movingAnimationSequence++;
		if(movingAnimationSequence >= movingAnimationEnd) {
			movingAnimationSequence = 0;
		}
	}

	if(isGrabbing) {
		grabAnimationSequence++;
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

	if(isJumping) {
		if(velocity.y > 0.5) {
			playerSprite = playerSprites[9];
			if(disguise != DISGUISE_NONE) {
				disguiseSprite = disguiseSprites[disguise][9];
			}
		}
		else if(velocity.y <= 0.5 && velocity.y >= -0.5) {
			playerSprite = playerSprites[10];
			if(disguise != DISGUISE_NONE) {
				disguiseSprite = disguiseSprites[disguise][10];
			}
		}
		else if(velocity.y < -0.5) {
			playerSprite = playerSprites[11];
			if(disguise != DISGUISE_NONE) {
				disguiseSprite = disguiseSprites[disguise][11];
			}
		}
	}

	if(isGrabbing) {
		int grabIndex = grabAnimationSequence / grabAnimationInterval;
		if(item->getY() >= frontOfPlayer.y) {
			playerSprite = playerSprites[3 + grabIndex];
			if(disguise != DISGUISE_NONE) {
				disguiseSprite = disguiseSprites[disguise][3 + grabIndex];
			}
		}
		else {
			playerSprite = playerSprites[6 + grabIndex];
			if(disguise != DISGUISE_NONE) {
				disguiseSprite = disguiseSprites[disguise][6 + grabIndex];
			}
		}
	}

	D3DXVECTOR2 intersection;
	D3DXVECTOR2 bottomCenter(position.x + offset.x, position.y + size.y);
#if _DEBUG
	playerNewPosition = bottomCenter;
	playerLastPosition = previousLastBottomCenter;
#endif
	double newY;
	if(level.checkCollision(previousLastBottomCenter, bottomCenter, &intersection, &newY)) {
		position.y = (float) (newY - size.y - 0.1f);
		isJumping = false;
		velocity.x = 0;
		velocity.y = 0;
	}

	if(position.y + size.y > windowHeight) {
		position.y = windowHeight - size.y;
		velocity.x = 0;
		velocity.y = 0;
	}

	if(getX() - getOffsetX() + spacing - (velocityStep * timeElapsed * 10) < 0) {
		position.x = (float) -spacing;
	}

	if(getX() + getOffsetX() - spacing + (velocityStep * timeElapsed * 10) > level.mapWidth) {
		position.x = level.mapWidth - (getOffsetX() * 2) + spacing;
	}

	frontOfPlayer = D3DXVECTOR2((movementDirection >= 0) ? getX() + offset.x - spacing : getX() - offset.x + spacing, getY() + 15);
	previousLastPosition = lastPosition;

	if(position != lastPosition) {
		movementHistory.push_front(D3DXVECTOR2(position.x + offset.x, position.y + size.y));
	}
}

void Player::draw(int * scrollingOffset, LPDIRECT3DDEVICE9 d3dDevice) {
	D3DXVECTOR2 offsetPosition = position;
	if(scrollingOffset != NULL) {
		offsetPosition.x -= (*scrollingOffset);
	}
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
//	testDrawEmptyBox(d3dDevice, getX(), getY(), offset.x, offset.y, &externalScrollingOffset);
//	testDrawPoint(d3dDevice, getX(), getY(), &externalScrollingOffset);

//	testDrawPoint(d3dDevice, frontOfPlayer.x, frontOfPlayer.y, &externalScrollingOffset);
	testDrawEmptyCircle(d3dDevice, frontOfPlayer.x, frontOfPlayer.y, grabRadius, grabRadius, D3DCOLOR_XRGB(0, 0, 255), &externalScrollingOffset);
#endif
}

void Player::moveLeft() {
	movementDirection = -1;
}

void Player::moveRight() {
	movementDirection = 1;
}

void Player::jump() {
	if(!isJumping && jumpTime == 0) {
		isJumping = true;
		velocity.y = jumpVelocity;
		jumpTime = jumpCooldown;
	}
}

void Player::grab() {
	if(isGrabbing) { return; }

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

D3DXVECTOR2 Player::getFollowPosition() const {
	if(movementHistory.empty()) {
		return D3DXVECTOR2(0, 0);
	}
	else {
		return movementHistory.back();
	}
}

void Player::popFollowPosition() {
	if(!movementHistory.empty()) {
		movementHistory.pop_back();
	}
}

bool Player::hasFollowPosition() {
	return !movementHistory.empty();
}