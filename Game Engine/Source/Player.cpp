#include "Player.h"
#include "Level.h"

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
				  isJumping(false),
				  jumpVelocity(70), //32
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
				  spacing(25),
				  disguise(1) {
	this->windowHeight = windowHeight;
	this->windowWidth = windowWidth;
	
	this->playerSpriteSheet = spriteSheets->getSpriteSheet("Alien");
	this->playerSprite = playerSpriteSheet->getSprite(0);
	this->disguiseSprite = playerSpriteSheet->getSprite(3);

	this->scale = D3DXVECTOR2(1, 1);
	this->size = D3DXVECTOR2(playerSprite->getWidth() * scale.x, playerSprite->getHeight() * scale.y);
	this->offset = D3DXVECTOR2(((playerSprite->getWidth() / 2.0f) * scale.x) + 20, (playerSprite->getHeight() / 2.0f) * scale.y);
	this->position = D3DXVECTOR2(xPos, yPos);
	this->velocity = D3DXVECTOR2(0, 0);
	this->maxVelocity = 2;
	this->velocityStep = 12.0f;
}

Player::~Player() { }

void Player::tick() {
	D3DXVECTOR2 lastPosition = position;
	D3DXVECTOR2 lastBottomCenter(getX(), position.y + getHeight());
	playerColour = D3DCOLOR_RGBA(255, 255, 255, 255);

	jumpTime -= (float) timeElapsed;
	if(jumpTime < 0 && velocity.x == 0 && velocity.y == 0) {
		jumpTime = 0;
		isJumping = false;
	}

	position.y += (float) (-velocity.y * (timeElapsed * 10));
	velocity.y -= (float) (Constants::GRAVITY * (timeElapsed * 10));

	playerSprite = playerSpriteSheet->getSprite(movingAnimationSequence / movingAnimationInterval);
	disguiseSprite = playerSpriteSheet->getSprite(3 + (movingAnimationSequence / movingAnimationInterval));

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

	if(isJumping) {
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
			disguiseSprite = playerSpriteSheet->getSprite(19 + (grabAnimationSequence / grabAnimationInterval));
		}
	}

	D3DXVECTOR2 intersection;
	D3DXVECTOR2 bottomCenter(getX(), position.y + getHeight());
	if(level.checkCollision(lastBottomCenter, bottomCenter, intersection)) {
		position.y = intersection.y - size.y - 0.1f;
		position.x = intersection.x - offset.x;
		velocity.x = 0;
		velocity.y = 0;
	}

	/*D3DXVECTOR2 bottomCenter(getX(), position.y + getHeight());
	if(level.checkCollision(bottomCenter, 25)) {
		position = lastPosition;
		velocity.x = 0;
		velocity.y = 0;
	}*/

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
	testDrawEmptyBox(d3dDevice, getX(), getY(), offset.x, offset.y);
//	testDrawEmptyCircle(d3dDevice, getX(), getY(), 25, 25);
	testDrawPoint(d3dDevice, getX(), getY());

	testDrawPoint(d3dDevice, frontOfPlayer.x, frontOfPlayer.y);
	testDrawEmptyCircle(d3dDevice, frontOfPlayer.x, frontOfPlayer.y, grabRadius, grabRadius);
#endif
}

void Player::moveLeft() {
	movementDirection = -1;
	position.x -= (float) (velocityStep * timeElapsed * 10);
}

void Player::moveRight() {
	movementDirection = 1;
	position.x += (float) (velocityStep * timeElapsed * 10);
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
