#include "Player.h"

Player::Player(float xPos,
			   float yPos,
			   int windowWidth,
			   int windowHeight,
			   double & externalTimeElapsed,
			   Variables * settings,
			   SpriteSheets * spriteSheets,
			   LPDIRECT3DDEVICE9 d3dDevice)
				: timeElapsed(externalTimeElapsed),
				  isJumping(false),
				  jumpVelocity(32),
				  jumpCooldown(0.94f),
				  jumpTime(0),
				  isMoving(false),
				  movementDirection(0),
				  movingAnimationSequence(0),
				  movingAnimationInterval(11),
				  movingAnimationEnd(movingAnimationInterval * 3),
				  disguise(0) {
	this->windowHeight = windowHeight;
	this->windowWidth = windowWidth;
	this->settings = settings;
	
	playerSpriteSheet = spriteSheets->getSpriteSheet("Alien");
	this->playerSprite = playerSpriteSheet->getSprite(0);
	this->disguiseSprite = playerSpriteSheet->getSprite(3);

	this->scale = D3DXVECTOR2(1, 1);
	this->size = D3DXVECTOR2(playerSprite->getWidth() * scale.x, playerSprite->getHeight() * scale.y);
	this->offset = D3DXVECTOR2((playerSprite->getWidth() / 2.0f) * scale.x, (playerSprite->getHeight() / 2.0f) * scale.y);
	this->position = D3DXVECTOR2(xPos - offset.x, yPos - offset.y);
	this->velocity = D3DXVECTOR2(0, 0);
	this->velocityStep = 2.5f;
}

Player::~Player() { }

void Player::tick() {
	playerColour = D3DCOLOR_RGBA(255, 255, 255, 255);

	if(isJumping) {
		position.y += (float) (-velocity.y * (timeElapsed * 10));
	}
	jumpTime -= (float) timeElapsed;
	if(jumpTime < 0) { jumpTime = 0; }

	velocity.y -= (float) (Constants::GRAVITY * (timeElapsed * 10));
	if(position.y + getOffsetY() > windowHeight ) {
		isJumping = false;
		position.y = windowHeight - getOffsetY();
		velocity.y = 0;
	}

	playerSprite = playerSpriteSheet->getSprite(movingAnimationSequence / movingAnimationInterval);
	disguiseSprite = playerSpriteSheet->getSprite(3 + (movingAnimationSequence / movingAnimationInterval));

	if(isMoving) {
		movingAnimationSequence++;
		if(movingAnimationSequence >= movingAnimationEnd) {
			movingAnimationSequence = 0;
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
}

void Player::draw(LPDIRECT3DDEVICE9 d3dDevice) {
	if(movementDirection >= 0) {
		playerSprite->drawBackwardsCentered(&scale, &offset, 0, NULL, &position, d3dDevice);
		if(disguise > 0) {
			disguiseSprite->drawBackwardsCentered(&scale, &offset, 0, NULL, &position, d3dDevice);
		}
	}
	else {
		playerSprite->drawCentered(&scale, &offset, 0, NULL, &position, d3dDevice);
		if(disguise > 0) {
			disguiseSprite->drawCentered(&scale, &offset, 0, NULL, &position, d3dDevice);
		}
	}
}

void Player::moveLeft() {
	movementDirection = -1;
	if(position.x - getOffsetX() - velocityStep < 0) {
		position.x = getOffsetX();
	}
	else {
		position.x -= velocityStep;
	}
}

void Player::moveRight() {
	movementDirection = 1;
	if(position.x + getOffsetX() + velocityStep > windowWidth) {
		position.x = windowWidth - getOffsetX();
	}
	else {
		position.x += velocityStep;
	}
}

void Player::jump() {
	if(!isJumping && jumpTime == 0) {
		isJumping = true;
		velocity.y = jumpVelocity;
		jumpTime = jumpCooldown;
	}
}

void Player::grab(Object * o) {
	if(o == NULL) { return; }

	if(o->getX() < getX()) {
		
	}
	else if(o->getX() >= getX()) {
		
	}
}
