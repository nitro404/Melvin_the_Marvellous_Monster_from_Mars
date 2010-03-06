#include "Player.h"

int Player::movementSpeed = 6;
float Player::maxJumpHeight = 27;

Player::Player(float x,
			   float y,
			   int xBoundary,
			   int yBoundary,
			   Sprite * sprite)
				: playerSprite(sprite),
				  isJumping(false) {
	this->boundary = D3DXVECTOR2((float) xBoundary, (float) yBoundary);
	this->scale = D3DXVECTOR2(1, 1);
	this->offset = D3DXVECTOR2(playerSprite->getWidth() / 2.0f, playerSprite->getHeight() / 2.0f);
	this->position = D3DXVECTOR2(x, y - (playerSprite->getHeight() * scale.y));
}

Player::~Player() { }

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
}

void Player::draw(LPDIRECT3DDEVICE9 d3dDevice) {
	playerSprite->draw(&scale, &offset, 0, NULL, &position, d3dDevice);
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
