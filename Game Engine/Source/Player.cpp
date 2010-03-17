#include "Player.h"

int Player::movementSpeed = 6;
float Player::maxJumpHeight = 27;

Player::Player(float x,
			   float y,
			   int xBoundary,
			   int yBoundary,
			   Variables * settings,
			   LPDIRECT3DDEVICE9 d3dDevice)
				: isJumping(false) {
	this->settings = settings;
	
	int _x = 8,		_y = 8,		_s = _x * _y;
	int _w = 126,	_h = 126,	_xi = _w + 2,	_yi = _h + 2;
	int _sx = 1,	_sy = 1,	_c = 0;
	SpriteSheetOffset * offsets = new SpriteSheetOffset[_s];
	for(int i=0;i<_x;i++,_sx=1) {
		for(int j=0;j<_y;j++) {
			offsets[_c].x = _sx;
			offsets[_c].y = _sy;
			offsets[_c].w = _w;
			offsets[_c].h = _h;
			_sx += _xi;
			_c++;
		}
		_sy += _yi;
	}
	playerSpriteSheet = new Sprite("Alien.png", settings->getValue("Sprite Directory"), d3dDevice);
	playerSprites = new SpriteSheet(playerSpriteSheet, offsets, _s);
	delete [] offsets;
	playerSprite = playerSprites->getSprite(5);

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
