#include "Player.h"
#include "Level.h"

#if _DEBUG
extern D3DXVECTOR2 playerNewPosition;
extern D3DXVECTOR2 playerLastPosition;
#endif

const int Player::DISGUISE_NONE = -1;
const int Player::DISGUISE_WIG = 0;
const int Player::DISGUISE_FBI = 1;
const int Player::DISGUISE_BOX = 2;
const int Player::DISGUISE_BIOHAZARD = 3;

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
				  disguise(DISGUISE_BOX),
				  isJumping(false),
				  jumpVelocity(32),
				  jumpCooldown(0.92f),
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
				  disguises(NULL),
				  spacing(25) {
	this->windowHeight = windowHeight;
	this->windowWidth = windowWidth;
	
	this->playerSpriteSheet = spriteSheets->getSpriteSheet("Alien");

//	loadDisguises();

	this->playerSprite = playerSpriteSheet->getSprite("Alien Walk 1");
	this->disguiseSprite = playerSpriteSheet->getSprite("Alien Walk 1 Box");

	this->scale = D3DXVECTOR2(1, 1);
	this->size = D3DXVECTOR2(playerSprite->getWidth() * scale.x, playerSprite->getHeight() * scale.y);
	this->offset = D3DXVECTOR2(((playerSprite->getWidth() / 2.0f) * scale.x) + 20, (playerSprite->getHeight() / 2.0f) * scale.y);
	this->position = D3DXVECTOR2(xPos, yPos);
	this->velocity = D3DXVECTOR2(0, 0);
	this->maxVelocity = 2;
	this->velocityStep = 12.0f;
}

Player::~Player() {
	/*
	for(int i=0;i<4;i++) {
		delete [] disguises[i];
	}
	delete [] disguises;
	*/
}

void Player::loadDisguises() {
	disguises = new Sprite**[4];
	for(int i=0;i<4;i++) {
		disguises[4] = new Sprite*[12];
	}
	disguises[DISGUISE_WIG][0] = playerSpriteSheet->getSprite("Alien Walk 1 Wig");
	disguises[DISGUISE_WIG][1] = playerSpriteSheet->getSprite("Alien Walk 2 Wig");
	disguises[DISGUISE_WIG][2] = playerSpriteSheet->getSprite("Alien Walk 3 Wig");
	disguises[DISGUISE_WIG][3] = playerSpriteSheet->getSprite("Alien Grab 1 Wig");
	disguises[DISGUISE_WIG][4] = playerSpriteSheet->getSprite("Alien Grab 2 Wig");
	disguises[DISGUISE_WIG][5] = playerSpriteSheet->getSprite("Alien Grab 3 Wig");
	disguises[DISGUISE_WIG][6] = playerSpriteSheet->getSprite("Alien Grab 1 Wig");
	disguises[DISGUISE_WIG][7] = playerSpriteSheet->getSprite("Alien Grab 2 Wig");
	disguises[DISGUISE_WIG][8] = playerSpriteSheet->getSprite("Alien Grab 3 Wig");
	disguises[DISGUISE_WIG][9] = playerSpriteSheet->getSprite("Alien Jump 1 Wig");
	disguises[DISGUISE_WIG][10] = playerSpriteSheet->getSprite("Alien Jump 2 Wig");
	disguises[DISGUISE_WIG][11] = playerSpriteSheet->getSprite("Alien Jump 3 Wig");

	disguises[DISGUISE_FBI][0] = playerSpriteSheet->getSprite("Alien Walk 1 FBI Suit");
	disguises[DISGUISE_FBI][1] = playerSpriteSheet->getSprite("Alien Walk 2 FBI Suit");
	disguises[DISGUISE_FBI][2] = playerSpriteSheet->getSprite("Alien Walk 3 FBI Suit");
	disguises[DISGUISE_FBI][3] = playerSpriteSheet->getSprite("Alien Grab Low 1 FBI Suit");
	disguises[DISGUISE_FBI][4] = playerSpriteSheet->getSprite("Alien Grab Low 2 FBI Suit");
	disguises[DISGUISE_FBI][5] = playerSpriteSheet->getSprite("Alien Grab Low 3 FBI Suit");
	disguises[DISGUISE_FBI][6] = playerSpriteSheet->getSprite("Alien Grab High 1 FBI Suit");
	disguises[DISGUISE_FBI][7] = playerSpriteSheet->getSprite("Alien Grab High 2 FBI Suit");
	disguises[DISGUISE_FBI][8] = playerSpriteSheet->getSprite("Alien Grab High 3 FBI Suit");
	disguises[DISGUISE_FBI][9] = playerSpriteSheet->getSprite("Alien Jump 1 FBI Suit");
	disguises[DISGUISE_FBI][10] = playerSpriteSheet->getSprite("Alien Jump 2 FBI Suit");
	disguises[DISGUISE_FBI][11] = playerSpriteSheet->getSprite("Alien Jump 3 FBI Suit");

	disguises[DISGUISE_BOX][0] = playerSpriteSheet->getSprite("Alien Walk 1 Box");
	disguises[DISGUISE_BOX][1] = playerSpriteSheet->getSprite("Alien Walk 2 Box");
	disguises[DISGUISE_BOX][2] = playerSpriteSheet->getSprite("Alien Walk 3 Box");
	disguises[DISGUISE_BOX][3] = playerSpriteSheet->getSprite("Alien Grab Low 1 Box");
	disguises[DISGUISE_BOX][4] = playerSpriteSheet->getSprite("Alien Grab Low 2 Box");
	disguises[DISGUISE_BOX][5] = playerSpriteSheet->getSprite("Alien Grab Low 3 Box");
	disguises[DISGUISE_BOX][6] = playerSpriteSheet->getSprite("Alien Grab High 1 Box");
	disguises[DISGUISE_BOX][7] = playerSpriteSheet->getSprite("Alien Grab High 2 Box");
	disguises[DISGUISE_BOX][8] = playerSpriteSheet->getSprite("Alien Grab High 3 Box");
	disguises[DISGUISE_BOX][9] = playerSpriteSheet->getSprite("Alien Jump 1 Box");
	disguises[DISGUISE_BOX][10] = playerSpriteSheet->getSprite("Alien Jump 2 Box");
	disguises[DISGUISE_BOX][11] = playerSpriteSheet->getSprite("Alien Jump 3 Box");

	disguises[DISGUISE_BOX][0] = playerSpriteSheet->getSprite("Alien Walk 1 BioHazard Suit");
	disguises[DISGUISE_BOX][1] = playerSpriteSheet->getSprite("Alien Walk 2 BioHazard Suit");
	disguises[DISGUISE_BOX][2] = playerSpriteSheet->getSprite("Alien Walk 3 BioHazard Suit");
	disguises[DISGUISE_BOX][3] = playerSpriteSheet->getSprite("Alien Grab Low 1 BioHazard Suit");
	disguises[DISGUISE_BOX][4] = playerSpriteSheet->getSprite("Alien Grab Low 2 BioHazard Suit");
	disguises[DISGUISE_BOX][5] = playerSpriteSheet->getSprite("Alien Grab Low 3 BioHazard Suit");
	disguises[DISGUISE_BOX][6] = playerSpriteSheet->getSprite("Alien Grab High 1 BioHazard Suit");
	disguises[DISGUISE_BOX][7] = playerSpriteSheet->getSprite("Alien Grab High 2 BioHazard Suit");
	disguises[DISGUISE_BOX][8] = playerSpriteSheet->getSprite("Alien Grab High 3 BioHazard Suit");
	disguises[DISGUISE_BOX][9] = playerSpriteSheet->getSprite("Alien Jump 1 BioHazard Suit");
	disguises[DISGUISE_BOX][10] = playerSpriteSheet->getSprite("Alien Jump 2 BioHazard Suit");
	disguises[DISGUISE_BOX][11] = playerSpriteSheet->getSprite("Alien Jump 3 BioHazard Suit");
}

void Player::tick() {
	D3DXVECTOR2 lastPosition = position;
	D3DXVECTOR2 lastBottomCenter(getX(), position.y + getHeight());
	playerColour = D3DCOLOR_RGBA(255, 255, 255, 255);

	jumpTime -= (float) timeElapsed;
	if(jumpTime < 0 && velocity.x == 0 && velocity.y == 0) {
		jumpTime = 0;
		isJumping = false;
	}

	position.x += (isMoving) ? (((movementDirection >= 0) ? 1 : -1) * (float) (velocityStep * timeElapsed * 10)) : 0;
	position.y += (float) (-velocity.y * (timeElapsed * 10));
	velocity.y -= (float) (Constants::GRAVITY * (timeElapsed * 10));

//	playerSprite = playerSpriteSheet->getSprite(movingAnimationSequence / movingAnimationInterval);
//	disguiseSprite = playerSpriteSheet->getSprite(3 + (movingAnimationSequence / movingAnimationInterval));

//	playerSprite = ;
//	disguiseSprite = disguises[disguise][(movingAnimationSequence / movingAnimationInterval)];

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
		}
	}

	/*if(isJumping) {
		if(velocity.y > 0.5) {
			playerSprite = playerSpriteSheet->getSprite(24);
			disguiseSprite = playerSpriteSheet->getSprite(27);
		}
		else if(velocity.y <= 0.5 && velocity.y >= -0.5) {
			playerSprite = playerSpriteSheet->getSprite(25);
			disguiseSprite = playerSpriteSheet->getSprite(28);
		}
		else if(velocity.y < -0.5) {
			playerSprite = playerSpriteSheet->getSprite(26);
			disguiseSprite = playerSpriteSheet->getSprite(29);
		}
	}

	if(isGrabbing) {
		if(item->getY() >= frontOfPlayer.y) {
			playerSprite = playerSpriteSheet->getSprite(8 + grabAnimationSequence / grabAnimationInterval);
			disguiseSprite = playerSpriteSheet->getSprite(11 + (grabAnimationSequence / grabAnimationInterval));
		}
		else {
			playerSprite = playerSpriteSheet->getSprite(16 + grabAnimationSequence / grabAnimationInterval);
			disguiseSprite = playerSpriteSheet->getSprite(11 + (grabAnimationSequence / grabAnimationInterval));
		}
	}*/

	D3DXVECTOR2 intersection;
	D3DXVECTOR2 bottomCenter(getX(), position.y + getHeight());
#if _DEBUG
	playerNewPosition = bottomCenter;
	playerLastPosition = lastBottomCenter;
#endif
	if(level.checkCollision(lastBottomCenter, bottomCenter, intersection)) {
		position.y = intersection.y - size.y - 0.1f;
		position.x = intersection.x - offset.x;
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

	if(getX() + getOffsetX() - spacing + (velocityStep * timeElapsed * 10) > windowWidth) {
		position.x = windowWidth - (getOffsetX() * 2) + spacing;
	}

	frontOfPlayer = D3DXVECTOR2((movementDirection >= 0) ? getX() + offset.x - spacing : getX() - offset.x + spacing, getY() + 15);
}

void Player::draw(LPDIRECT3DDEVICE9 d3dDevice) {
	if(movementDirection >= 0) {
		playerSprite->drawBackwards(&scale, &offset, 0, NULL, &position, d3dDevice);
		if(disguise > 0) {
			disguiseSprite->drawBackwards(&scale, &offset, 0, NULL, &position, d3dDevice);
		}
	}
	else {
		playerSprite->draw(&scale, &offset, 0, NULL, &position, d3dDevice);
		if(disguise > 0) {
			disguiseSprite->draw(&scale, &offset, 0, NULL, &position, d3dDevice);
		}
	}

#if _DEBUG
//	testDrawEmptyBox(d3dDevice, getX(), getY(), offset.x, offset.y);
//	testDrawPoint(d3dDevice, getX(), getY());

//	testDrawPoint(d3dDevice, frontOfPlayer.x, frontOfPlayer.y);
//	testDrawEmptyCircle(d3dDevice, frontOfPlayer.x, frontOfPlayer.y, grabRadius, grabRadius);
#endif
}

void Player::moveLeft() {
	movementDirection = -1;
//	position.x -= (float) (velocityStep * timeElapsed * 10);
}

void Player::moveRight() {
	movementDirection = 1;
//	position.x += (float) (velocityStep * timeElapsed * 10);
}

void Player::jump() {
	if(!isJumping) {
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

	/*if(item->getY() < getY()) {
		prompt("%s above", item->getName());
	}
	else if(item->getY() >= getY()) {
		prompt("%s below", item->getName());
	}*/
}
