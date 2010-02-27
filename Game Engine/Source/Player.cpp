#include "Player.h"

int Player::movementSpeed = 6;
float Player::maxJumpHeight = 27;

Player::Player(int x, int y, int xBoundary, int yBoundary, LPDIRECT3DDEVICE9 d3dDevice) : vertexBuffer(NULL) {
	this->position = Point(x, y);
	this->boundary = Point(xBoundary - 100, yBoundary - 100);
	this->isJumping = false;

	if(!init(d3dDevice)) {
		MessageBoxA(NULL, "Error initializing player.", "Error", MB_OK);
		PostQuitMessage(0);
	}
}

Player::~Player() {
	if(playerTexture != NULL) { playerTexture->Release(); }
	if(playerSprite != NULL) { playerSprite->Release(); }
}

void Player::tick() {
	if(isJumping) {
		position.y = boundary.y - (-(jumpTick * jumpTick)) - maxJumpHeight;
		jumpTick += 0.3f;
		if(position.y > boundary.y) {
			isJumping = false;
			position.y = boundary.y;
		}
	}
	playerColour = D3DCOLOR_RGBA(255, 255, 255, 255);
}

void Player::draw(LPDIRECT3DDEVICE9 d3dDevice) {
	D3DXMATRIX transformation;
	D3DXVECTOR2 translation = D3DXVECTOR2((float) position.x, (float) position.y);
	D3DXVECTOR2 scaling(0.4f, 0.4f);

	D3DXMatrixTransformation2D(&transformation, NULL, 0, &scaling, NULL, 0, &translation);
	playerSprite->SetTransform(&transformation);

	playerSprite->Begin(D3DXSPRITE_ALPHABLEND);

	playerSprite->Draw(playerTexture, NULL, NULL, NULL, playerColour);

	playerSprite->End();
}

bool Player::init(LPDIRECT3DDEVICE9 d3dDevice) {
	D3DXCreateSprite(d3dDevice, &playerSprite);
	
	if(FAILED(D3DXCreateTextureFromFile(d3dDevice, L"../Sprites/Alien.PNG", &playerTexture))) {
		return false;
	}
	
	return true;
}

void Player::moveLeft() {
	if(position.x - movementSpeed < 0) { position.x = 0; }
	else { position.x -= movementSpeed; }
}

void Player::moveRight() {
	if(position.x + movementSpeed > boundary.x) { position.x = boundary.x; }
	else { position.x += movementSpeed; }
}

void Player::jump() {
	if(!isJumping) {
		isJumping = true;
		jumpTick = -5.15f;
	}
}

void Player::moveUp() {
	if(position.y - movementSpeed < 0) { position.y = 0; }
	else { position.y -= movementSpeed; }
}

void Player::moveDown() {
	if(position.y + movementSpeed > boundary.y) { position.y = boundary.y; }
	else { position.y += movementSpeed; }
}
