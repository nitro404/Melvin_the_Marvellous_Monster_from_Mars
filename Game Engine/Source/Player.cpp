#include "Player.h"

Player::Player(float xPos,
			   float yPos,
			   int windowWidth,
			   int windowHeight,
			   Variables * settings,
			   LPDIRECT3DDEVICE9 d3dDevice)
				: maxJumpHeight(27),
				  isJumping(false),
				  isMoving(0),
				  movingAnimationSequence(0),
				  movingAnimationInterval(12),
				  movingAnimationEnd(movingAnimationInterval * 3),
				  disguise(1) {
	this->windowHeight = windowHeight;
	this->windowWidth = windowWidth;
	this->settings = settings;
	
	Sprite * playerSpriteSheetImage = new Sprite("Alien.png", settings->getValue("Sprite Directory"), d3dDevice);
	this->playerSpriteSheet = new SpriteSheet(playerSpriteSheetImage, 1, 1, 126, 126, 128, 128, true, 8, 8);
	this->playerSprite = playerSpriteSheet->getSprite(0);
	this->disguiseSprite = playerSpriteSheet->getSprite(3);

	this->scale = D3DXVECTOR2(1, 1);
	this->size = D3DXVECTOR2(playerSprite->getWidth() * scale.x, playerSprite->getHeight() * scale.y);
	this->offset = D3DXVECTOR2((playerSprite->getWidth() / 2.0f) * scale.x, (playerSprite->getHeight() / 2.0f) * scale.y);
	this->position = D3DXVECTOR2(xPos - offset.x, yPos - offset.y);
	this->velocity = D3DXVECTOR2(0, 0);
	this->velocityStep = 2.5;
}

Player::~Player() {
	delete playerSpriteSheet;
}

void Player::tick() {
	if(isJumping) {
		position.y = windowHeight - getHeight() - (-(jumpTick * jumpTick)) - maxJumpHeight;
		jumpTick += 0.3f;
		if(position.y + getHeight() > windowHeight - getHeight()) {
			isJumping = false;
			position.y = windowHeight - getHeight();
		}
	}
	playerColour = D3DCOLOR_RGBA(255, 255, 255, 255);

	if(isMoving != 0) {
		playerSprite = playerSpriteSheet->getSprite(movingAnimationSequence / movingAnimationInterval);
		disguiseSprite = playerSpriteSheet->getSprite(3 + (movingAnimationSequence / movingAnimationInterval));
		
		movingAnimationSequence++;
		if(movingAnimationSequence >= movingAnimationEnd) {
			movingAnimationSequence = 0;
		}
	}
}

void Player::draw(LPDIRECT3DDEVICE9 d3dDevice) {
	if(isMoving >= 0) {
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
#ifdef _DEBUG
	testDrawPoint(d3dDevice, position.x, position.y);
	testDrawPoint(d3dDevice, getX(), getY());
#endif
}

void Player::moveLeft() {
	if(position.x - velocityStep < 0) {
		position.x = 0;
	}
	else {
		position.x -= velocityStep;
	}
}

void Player::moveRight() {
	if(position.x + getWidth() + velocityStep > windowWidth) {
		position.x = windowWidth - getWidth();
	}
	else {
		position.x += velocityStep;
	}
}

void Player::jump() {
	if(!isJumping) {
		isJumping = true;
		jumpTick = -5.15f;
	}
}
