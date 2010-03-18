#include "Player.h"

Player::Player(float x,
			   float y,
			   int xBoundary,
			   int yBoundary,
			   Variables * settings,
			   LPDIRECT3DDEVICE9 d3dDevice)
				: movementSpeed(2.5),
				  maxJumpHeight(27),
				  isJumping(false),
				  isMoving(0),
				  movingAnimationSequence(0),
				  movingAnimationInterval(15),
				  movingAnimationEnd(movingAnimationInterval * 2) {
	this->settings = settings;
	
	playerSpriteSheetImage = new Sprite("Alien.png", settings->getValue("Sprite Directory"), d3dDevice);
	playerSpriteSheet = new SpriteSheet(playerSpriteSheetImage, 1, 1, 126, 126, 128, 128, true, 8, 8);
	playerSprite = playerSpriteSheet->getSprite(5);
	disguiseSprite = playerSpriteSheet->getSprite(35);

	this->boundary = D3DXVECTOR2((float) xBoundary, (float) yBoundary);
	this->scale = D3DXVECTOR2(1, 1);
	this->offset = D3DXVECTOR2(playerSprite->getWidth() / 2.0f, playerSprite->getHeight() / 2.0f);
	this->position = D3DXVECTOR2(x, y - (playerSprite->getHeight() * scale.y));
}

Player::~Player() {
	delete playerSpriteSheet;
	delete playerSpriteSheetImage;
}

void Player::tick() {
	if(isJumping) {
		position.y = boundary.y - (playerSprite->getHeight() * scale.y) - (-(jumpTick * jumpTick)) - maxJumpHeight;
		jumpTick += 0.3f;
		if(position.y > boundary.y - (playerSprite->getHeight() * scale.y)) {
			isJumping = false;
			position.y = boundary.y - (playerSprite->getHeight() * scale.y);
		}
	}
	playerColour = D3DCOLOR_RGBA(255, 255, 255, 255);

	if(isMoving != 0) {
		playerSprite = playerSpriteSheet->getSprite(5 + (movingAnimationSequence / movingAnimationInterval));
		disguiseSprite = playerSpriteSheet->getSprite(35 + (movingAnimationSequence / movingAnimationInterval));
		
		movingAnimationSequence++;
		if(movingAnimationSequence > movingAnimationEnd) {
			movingAnimationSequence = 0;
		}
	}
}

void Player::draw(LPDIRECT3DDEVICE9 d3dDevice) {
	if(isMoving >= 0) {
		playerSprite->draw(&scale, &offset, 0, NULL, &position, d3dDevice);
		disguiseSprite->draw(&scale, &offset, 0, NULL, &position, d3dDevice);
	}
	else {
		playerSprite->drawBackwards(&scale, &offset, 0, NULL, &position, d3dDevice);
		disguiseSprite->drawBackwards(&scale, &offset, 0, NULL, &position, d3dDevice);
	}
}

void Player::moveLeft() {
	if(position.x - movementSpeed < 0) {
		position.x = 0;
	}
	else {
		position.x -= movementSpeed;
	}
}

void Player::moveRight() {
	if(position.x + movementSpeed > boundary.x - (playerSprite->getWidth() * scale.x)) {
		position.x = boundary.x - (playerSprite->getWidth() * scale.x);
	}
	else {
		position.x += movementSpeed;
	}
}

void Player::jump() {
	if(!isJumping) {
		isJumping = true;
		jumpTick = -5.15f;
	}
}
